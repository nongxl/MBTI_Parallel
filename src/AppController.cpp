#include "AppController.h"
#include "ScenarioMapper.h"
#include "DisplayRenderer.h"
#include "DecisionProfile.h"
#include "ScenarioGenerator.h"
#include "ScenarioBuilder.h"
#include <cstdio>
#include <cstring>
#include <cmath>

#ifdef ARDUINO
#include <Arduino.h>
#include <Preferences.h>
static Preferences prefs;
#else
static uint32_t millis() { return 0; }
#endif

#ifdef LOW
#undef LOW
#endif
#ifdef HIGH
#undef HIGH
#endif
#ifdef CHANGE
#undef CHANGE
#endif

static float quinticEaseOut(float t) {
    float f = 1.0f - t;
    return 1.0f - f * f * f * f * f;
}

static void startRadarAnimation(UIContext& ctx, const RadarData& targetData, uint32_t currentMillis) {
    ctx.startRadar = ctx.currentRadar;
    ctx.endRadar = targetData;
    ctx.radarAnimStartTime = currentMillis;
    ctx.isRadarAnimActive = true;
}

static MatchType determineMatchType(const UIContext& ctx) {
    int sameChoiceCount = 0;
    for (int i = 0; i < MBTI_COUNT; ++i) {
        if (ctx.results[i].decision == ctx.userChoice) {
            sameChoiceCount++;
        }
    }
    if (sameChoiceCount == 0) {
        return MatchType::OUTLIER;
    } else if (sameChoiceCount >= 10) {
        return MatchType::MAJORITY;
    } else if (ctx.summary.yesCount >= 6 && ctx.summary.noCount >= 6) {
        return MatchType::SPLIT;
    }
    return MatchType::MATCH;
}

static void findBiggestDifferenceMBTI(const UIContext& ctx, MBTIType& outDiffMBTI, Decision& outDiffDecision) {
    float maxDist = -1.0f;
    outDiffMBTI = MBTIType::ESTJ;
    outDiffDecision = Decision::NO;

    for (int i = 0; i < MBTI_COUNT; ++i) {
        const PersonalityProfile& prof = getMBTIProfile(static_cast<MBTIType>(i));
        
        float dRisk = (ctx.userProfile.risk - prof.risk);
        float dNov  = (ctx.userProfile.novelty - prof.novelty);
        float dLog  = (ctx.userProfile.logic - prof.logic);
        float dSoc  = (ctx.userProfile.social - prof.social);
        float dPla  = (ctx.userProfile.planning - prof.planning);
        float dPra  = (ctx.userProfile.practicality - prof.practicality);

        float dist = sqrtf(dRisk*dRisk + dNov*dNov + dLog*dLog + dSoc*dSoc + dPla*dPla + dPra*dPra);

        if (ctx.results[i].decision != ctx.userChoice) {
            dist += 40.0f;
        }

        if (dist > maxDist) {
            maxDist = dist;
            outDiffMBTI = static_cast<MBTIType>(i);
            outDiffDecision = ctx.results[i].decision;
        }
    }
}

static void triggerRandomScenario(UIContext& ctx) {
    GeneratedScenario gen = generateProceduralScenario(ctx.recentDNAs, ctx.dnaHistoryCount);
    ctx.currentScenario = gen.scenario;
    snprintf(ctx.currentScenarioTitle, sizeof(ctx.currentScenarioTitle), "%s", gen.title);
    snprintf(ctx.currentScenarioDesc, sizeof(ctx.currentScenarioDesc), "%s", gen.description);
    snprintf(ctx.currentScenarioTitleCN, sizeof(ctx.currentScenarioTitleCN), "%s", gen.titleCN);
    snprintf(ctx.currentScenarioDescCN, sizeof(ctx.currentScenarioDescCN), "%s", gen.descriptionCN);

    for (int i = RECENT_DNA_HISTORY_SIZE - 1; i > 0; --i) {
        ctx.recentDNAs[i] = ctx.recentDNAs[i - 1];
    }
    ctx.recentDNAs[0] = gen.dna;
    if (ctx.dnaHistoryCount < RECENT_DNA_HISTORY_SIZE) {
        ctx.dnaHistoryCount++;
    }
}

static void applyCustomScenario(UIContext& ctx) {
    RenderedCustomScenario rendered = renderCustomScenario(ctx.customDNA);
    ctx.currentScenario = rendered.scenario;
    snprintf(ctx.currentScenarioTitle, sizeof(ctx.currentScenarioTitle), "%s", rendered.titleEN);
    snprintf(ctx.currentScenarioDesc, sizeof(ctx.currentScenarioDesc), "%s", rendered.descEN);
    snprintf(ctx.currentScenarioTitleCN, sizeof(ctx.currentScenarioTitleCN), "%s", rendered.titleCN);
    snprintf(ctx.currentScenarioDescCN, sizeof(ctx.currentScenarioDescCN), "%s", rendered.descCN);
}

void initApp(UIContext& ctx) {
    ctx.bootMenuMode = 0;
    ctx.selectedMenuIndex = 0;
    ctx.exploreIndex = 0;
    ctx.userChoice = Decision::YES;
    ctx.animStartTime = 0;
    ctx.animProgress = 0;
    ctx.dnaHistoryCount = 0;

#ifdef ARDUINO
    // 读取 ESP32 NVS Flash 语言持久化偏好
    prefs.begin("mbti_config", false);
    bool hasConfig = prefs.getBool("configured", false);
    if (hasConfig) {
        int savedLang = prefs.getInt("lang", 1);
        ctx.lang = static_cast<Language>(savedLang);
        ctx.state = AppState::HOME;
    } else {
        ctx.lang = Language::CHINESE;
        ctx.state = AppState::LANGUAGE_SELECT;
    }
#else
    ctx.lang = Language::CHINESE;
    ctx.state = AppState::HOME;
#endif

    // 从 NVS 加载用户累积决策历史
    loadUserHistoryFromNVS(ctx.userHistory);
    ctx.totalPlays = ctx.userHistory.totalPlays;

    // 默认自定义 4 步 DNA
    ctx.customDNA.who = WhoType::FRIEND;
    ctx.customDNA.situation = SituationType::TRAVEL;
    ctx.customDNA.condition = ConditionType::LAST_MINUTE;
    ctx.customDNA.tension = TensionType::SAFETY_VS_NOVELTY;

    // 雷达插值变量初始化
    ctx.isRadarAnimActive = false;
    ctx.radarAnimStartTime = 0;
    ctx.currentRadar = { 75.0f, 90.0f, 60.0f, 85.0f, 40.0f, 70.0f };
    ctx.startRadar = ctx.currentRadar;
    ctx.endRadar = ctx.currentRadar;

    applyCustomScenario(ctx);

    ctx.userProfile = calculateDecisionProfile(ctx.currentScenario, ctx.userChoice);
    ctx.closestMBTI = findClosestMBTI(ctx.userProfile, ctx.matchSimilarity);
    ctx.biggestDiffMBTI = MBTIType::ESTJ;
    ctx.biggestDiffDecision = Decision::NO;
    ctx.matchType = MatchType::MATCH;
}

void updateApp(UIContext& ctx, uint32_t currentMillis) {
    if (ctx.state == AppState::SIMULATING) {
        if (ctx.animStartTime == 0) {
            ctx.animStartTime = currentMillis;
        }

        uint32_t elapsed = currentMillis - ctx.animStartTime;
        ctx.animProgress = (elapsed * 100) / 1000;

        if (ctx.animProgress >= 100) {
            ctx.animProgress = 100;
            simulateAll(ctx.currentScenario, ctx.results);
            ctx.summary = summarizeResults(ctx.results);
            findBiggestSplit(ctx.results, ctx.splitYesType, ctx.splitNoType);
            
            ctx.state = AppState::SUMMARY;
            ctx.selectedMenuIndex = 0;
            renderUI(ctx);
        }
    }

    if (ctx.isRadarAnimActive) {
        uint32_t elapsed = currentMillis - ctx.radarAnimStartTime;
        float rawT = elapsed / 400.0f;
        if (rawT >= 1.0f) {
            rawT = 1.0f;
            ctx.isRadarAnimActive = false;
        }

        float easeT = quinticEaseOut(rawT);

        ctx.currentRadar.risk = ctx.startRadar.risk + (ctx.endRadar.risk - ctx.startRadar.risk) * easeT;
        ctx.currentRadar.novelty = ctx.startRadar.novelty + (ctx.endRadar.novelty - ctx.startRadar.novelty) * easeT;
        ctx.currentRadar.logic = ctx.startRadar.logic + (ctx.endRadar.logic - ctx.startRadar.logic) * easeT;
        ctx.currentRadar.social = ctx.startRadar.social + (ctx.endRadar.social - ctx.startRadar.social) * easeT;
        ctx.currentRadar.planning = ctx.startRadar.planning + (ctx.endRadar.planning - ctx.startRadar.planning) * easeT;
        ctx.currentRadar.practicality = ctx.startRadar.practicality + (ctx.endRadar.practicality - ctx.startRadar.practicality) * easeT;
    }
}

void handleInput(UIContext& ctx, KeyInput key) {
    if (key == KeyInput::NONE) return;
    uint32_t now = millis();

    switch (ctx.state) {
        case AppState::LANGUAGE_SELECT: {
            if (key == KeyInput::LEFT || key == KeyInput::UP) {
                ctx.selectedMenuIndex = 0;
            } else if (key == KeyInput::RIGHT || key == KeyInput::DOWN) {
                ctx.selectedMenuIndex = 1;
            } else if (key == KeyInput::ENTER) {
                ctx.lang = (ctx.selectedMenuIndex == 0) ? Language::CHINESE : Language::ENGLISH;
#ifdef ARDUINO
                prefs.putBool("configured", true);
                prefs.putInt("lang", static_cast<int>(ctx.lang));
#endif
                ctx.state = AppState::HOME;
                ctx.selectedMenuIndex = 0;
            }
            break;
        }

        case AppState::HOME:
            if (key == KeyInput::UP || key == KeyInput::DOWN) {
                ctx.bootMenuMode = 1 - ctx.bootMenuMode;
            } else if (key == KeyInput::ENTER) {
                if (ctx.bootMenuMode == 0) {
                    triggerRandomScenario(ctx);
                    ctx.state = AppState::BUILDER_PREVIEW;
                } else {
                    ctx.state = AppState::BUILDER_WHO;
                    ctx.selectedMenuIndex = 0;
                }
            }
            break;

        case AppState::BUILDER_WHO: { // Step 1: WHO
            int idx = ctx.selectedMenuIndex;
            if (key == KeyInput::LEFT) {
                if (idx % 2 == 1) idx--;
            } else if (key == KeyInput::RIGHT) {
                if (idx % 2 == 0 && idx + 1 < 6) idx++;
            } else if (key == KeyInput::UP) {
                if (idx >= 2) idx -= 2;
            } else if (key == KeyInput::DOWN) {
                if (idx + 2 < 6) idx += 2;
            } else if (key == KeyInput::ENTER) {
                ctx.customDNA.who = static_cast<WhoType>(idx);
                ctx.state = AppState::BUILDER_SITUATION;
                ctx.selectedMenuIndex = 0;
                return;
            } else if (key == KeyInput::BACK) {
                ctx.state = AppState::HOME;
                ctx.selectedMenuIndex = 0;
                return;
            }
            ctx.selectedMenuIndex = idx;
            break;
        }

        case AppState::BUILDER_SITUATION: { // Step 2: SITUATION
            int idx = ctx.selectedMenuIndex;
            if (key == KeyInput::LEFT) {
                if (idx % 2 == 1) idx--;
            } else if (key == KeyInput::RIGHT) {
                if (idx % 2 == 0 && idx + 1 < 6) idx++;
            } else if (key == KeyInput::UP) {
                if (idx >= 2) idx -= 2;
            } else if (key == KeyInput::DOWN) {
                if (idx + 2 < 6) idx += 2;
            } else if (key == KeyInput::ENTER) {
                ctx.customDNA.situation = static_cast<SituationType>(idx);
                ctx.state = AppState::BUILDER_CONDITION;
                ctx.selectedMenuIndex = 0;
                return;
            } else if (key == KeyInput::BACK) {
                ctx.state = AppState::BUILDER_WHO;
                ctx.selectedMenuIndex = 0;
                return;
            }
            ctx.selectedMenuIndex = idx;
            break;
        }

        case AppState::BUILDER_CONDITION: { // Step 3: CONDITION
            int idx = ctx.selectedMenuIndex;
            if (key == KeyInput::LEFT) {
                if (idx % 2 == 1) idx--;
            } else if (key == KeyInput::RIGHT) {
                if (idx % 2 == 0 && idx + 1 < 6) idx++;
            } else if (key == KeyInput::UP) {
                if (idx >= 2) idx -= 2;
            } else if (key == KeyInput::DOWN) {
                if (idx + 2 < 6) idx += 2;
            } else if (key == KeyInput::ENTER) {
                ctx.customDNA.condition = static_cast<ConditionType>(idx);
                ctx.state = AppState::BUILDER_TENSION;
                ctx.selectedMenuIndex = 0;
                return;
            } else if (key == KeyInput::BACK) {
                ctx.state = AppState::BUILDER_SITUATION;
                ctx.selectedMenuIndex = 0;
                return;
            }
            ctx.selectedMenuIndex = idx;
            break;
        }

        case AppState::BUILDER_TENSION: { // Step 4: TENSION
            int idx = ctx.selectedMenuIndex;
            if (key == KeyInput::LEFT) {
                if (idx % 2 == 1) idx--;
            } else if (key == KeyInput::RIGHT) {
                if (idx % 2 == 0 && idx + 1 < 6) idx++;
            } else if (key == KeyInput::UP) {
                if (idx >= 2) idx -= 2;
            } else if (key == KeyInput::DOWN) {
                if (idx + 2 < 6) idx += 2;
            } else if (key == KeyInput::ENTER) {
                ctx.customDNA.tension = static_cast<TensionType>(idx);
                applyCustomScenario(ctx);
                ctx.state = AppState::BUILDER_PREVIEW;
                return;
            } else if (key == KeyInput::BACK) {
                ctx.state = AppState::BUILDER_CONDITION;
                ctx.selectedMenuIndex = 0;
                return;
            }
            ctx.selectedMenuIndex = idx;
            break;
        }

        case AppState::BUILDER_PREVIEW:
            if (key == KeyInput::ENTER) {
                ctx.state = AppState::YOUR_CHOICE;
                ctx.selectedMenuIndex = 0;
            } else if (key == KeyInput::BACK) {
                if (ctx.bootMenuMode == 1) {
                    ctx.state = AppState::BUILDER_TENSION;
                } else {
                    ctx.state = AppState::HOME;
                }
                ctx.selectedMenuIndex = 0;
            }
            break;

        case AppState::YOUR_CHOICE:
            if (key == KeyInput::UP) {
                ctx.selectedMenuIndex = (ctx.selectedMenuIndex + 2) % 3;
            } else if (key == KeyInput::DOWN) {
                ctx.selectedMenuIndex = (ctx.selectedMenuIndex + 1) % 3;
            } else if (key == KeyInput::ENTER) {
                Decision choices[] = { Decision::YES, Decision::NO, Decision::MAYBE };
                ctx.userChoice = choices[ctx.selectedMenuIndex];

                simulateAll(ctx.currentScenario, ctx.results);
                ctx.summary = summarizeResults(ctx.results);

                ctx.userProfile = calculateDecisionProfile(ctx.currentScenario, ctx.userChoice);
                ctx.closestMBTI = findClosestMBTI(ctx.userProfile, ctx.matchSimilarity);
                findBiggestDifferenceMBTI(ctx, ctx.biggestDiffMBTI, ctx.biggestDiffDecision);
                ctx.matchType = determineMatchType(ctx);

                // 【记录并 0ms 持久化存入 ESP32 NVS Flash】
                recordUserDecisionToHistory(ctx.userHistory, ctx.userProfile, ctx.userChoice);
                ctx.totalPlays = ctx.userHistory.totalPlays;

                RadarData target = {
                    ctx.userProfile.risk,
                    ctx.userProfile.novelty,
                    ctx.userProfile.logic,
                    ctx.userProfile.social,
                    ctx.userProfile.planning,
                    ctx.userProfile.practicality
                };
                startRadarAnimation(ctx, target, now);

                ctx.state = AppState::YOUR_MATCH;
            } else if (key == KeyInput::BACK) {
                ctx.state = AppState::BUILDER_PREVIEW;
            }
            break;

        case AppState::YOUR_MATCH:
            if (key == KeyInput::ENTER) {
                ctx.state = AppState::EXPLORE;
                ctx.exploreIndex = static_cast<int>(ctx.closestMBTI);

                const PersonalityProfile& pProf = getMBTIProfile(ctx.results[ctx.exploreIndex].personality);
                RadarData target = { pProf.risk, pProf.novelty, pProf.logic, pProf.social, pProf.planning, pProf.practicality };
                startRadarAnimation(ctx, target, now);
            } else if (key == KeyInput::BACK) {
                ctx.state = AppState::HOME;
                ctx.selectedMenuIndex = 0;
            }
            break;

        case AppState::EXPLORE:
            if (key == KeyInput::LEFT || key == KeyInput::RIGHT) {
                if (key == KeyInput::LEFT) {
                    ctx.exploreIndex = (ctx.exploreIndex + 15) % 16;
                } else {
                    ctx.exploreIndex = (ctx.exploreIndex + 1) % 16;
                }
                const PersonalityProfile& pProf = getMBTIProfile(ctx.results[ctx.exploreIndex].personality);
                RadarData target = { pProf.risk, pProf.novelty, pProf.logic, pProf.social, pProf.planning, pProf.practicality };
                startRadarAnimation(ctx, target, now);
            } else if (key == KeyInput::ENTER) {
                if (ctx.bootMenuMode == 1) {
                    ctx.state = AppState::BUILDER_WHO;
                } else {
                    triggerRandomScenario(ctx);
                    ctx.state = AppState::BUILDER_PREVIEW;
                }
            } else if (key == KeyInput::BACK) {
                ctx.state = AppState::YOUR_MATCH;
            }
            break;

        case AppState::SIMULATING:
        case AppState::SUMMARY:
        case AppState::BIGGEST_SPLIT:
            if (key == KeyInput::ENTER) {
                ctx.state = AppState::EXPLORE;
            }
            break;
    }
}
