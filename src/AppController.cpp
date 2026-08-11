#include "AppController.h"
#include "ScenarioMapper.h"
#include "DisplayRenderer.h"
#include "DecisionProfile.h"

#ifdef ARDUINO
#include <Arduino.h>
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

static void startRadarAnimation(UIContext& ctx, const RadarData& targetData, uint32_t currentMillis) {
    ctx.startRadar = ctx.currentRadar;
    ctx.endRadar = targetData;
    ctx.radarAnimStartTime = currentMillis;
    ctx.isRadarAnimActive = true;
}

void initApp(UIContext& ctx) {
    ctx.state = AppState::HOME;
    ctx.selectedMenuIndex = 0;
    ctx.exploreIndex = 0;
    ctx.userChoice = Decision::YES;
    ctx.animStartTime = 0;
    ctx.animProgress = 0;

    // 雷达插值变量初始化
    ctx.isRadarAnimActive = false;
    ctx.radarAnimStartTime = 0;
    ctx.currentRadar = { 75.0f, 90.0f, 60.0f, 85.0f, 40.0f, 70.0f };
    ctx.startRadar = ctx.currentRadar;
    ctx.endRadar = ctx.currentRadar;

    // 默认初始 Selection
    ctx.currentSelection.decisionType = DecisionType::GET;
    ctx.currentSelection.motivation = Motivation::WANT;
    ctx.currentSelection.concern = Concern::COST;
    ctx.currentSelection.intensity = Intensity::MEDIUM;
    ctx.currentSelection.priority = Priority::EXPERIENCE;

    ctx.currentScenario = buildScenario(ctx.currentSelection);
    ctx.userProfile = calculateDecisionProfile(ctx.currentScenario, ctx.userChoice);
    ctx.closestMBTI = findClosestMBTI(ctx.userProfile, ctx.matchSimilarity);
}

void updateApp(UIContext& ctx, uint32_t currentMillis) {
    // 1. 处理模拟分支进度条动效
    if (ctx.state == AppState::SIMULATING) {
        if (ctx.animStartTime == 0) {
            ctx.animStartTime = currentMillis;
        }

        uint32_t elapsed = currentMillis - ctx.animStartTime;
        ctx.animProgress = (elapsed * 100) / 1000; // 1000ms 持续时间动画

        if (ctx.animProgress >= 100) {
            ctx.animProgress = 100;
            // 引擎计算 16 人格模拟结果
            simulateAll(ctx.currentScenario, ctx.results);
            ctx.summary = summarizeResults(ctx.results);
            findBiggestSplit(ctx.results, ctx.splitYesType, ctx.splitNoType);
            
            ctx.state = AppState::SUMMARY;
            ctx.selectedMenuIndex = 0;
            renderUI(ctx); // 自动完成后，立即强制重绘，呈现 SUMMARY 汇总界面
        }
    }

    // 2. 处理六维雷达极坐标平滑补间形变插值 (250ms 形变)
    if (ctx.isRadarAnimActive) {
        uint32_t elapsed = currentMillis - ctx.radarAnimStartTime;
        float t = elapsed / 250.0f;
        if (t >= 1.0f) {
            t = 1.0f;
            ctx.isRadarAnimActive = false;
        }

        ctx.currentRadar.risk = ctx.startRadar.risk + (ctx.endRadar.risk - ctx.startRadar.risk) * t;
        ctx.currentRadar.novelty = ctx.startRadar.novelty + (ctx.endRadar.novelty - ctx.startRadar.novelty) * t;
        ctx.currentRadar.logic = ctx.startRadar.logic + (ctx.endRadar.logic - ctx.startRadar.logic) * t;
        ctx.currentRadar.social = ctx.startRadar.social + (ctx.endRadar.social - ctx.startRadar.social) * t;
        ctx.currentRadar.planning = ctx.startRadar.planning + (ctx.endRadar.planning - ctx.startRadar.planning) * t;
        ctx.currentRadar.practicality = ctx.startRadar.practicality + (ctx.endRadar.practicality - ctx.startRadar.practicality) * t;
    }
}

void handleInput(UIContext& ctx, KeyInput key) {
    if (key == KeyInput::NONE) return;
    uint32_t now = millis();

    switch (ctx.state) {
        case AppState::HOME:
            if (key == KeyInput::ENTER) {
                ctx.state = AppState::BUILDER_TYPE;
                ctx.selectedMenuIndex = 0;
            }
            break;

        case AppState::BUILDER_TYPE: {
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
                DecisionType types[] = { DecisionType::GET, DecisionType::GO, DecisionType::DO, DecisionType::SAY, DecisionType::CHOOSE, DecisionType::CHANGE };
                ctx.currentSelection.decisionType = types[idx];
                ctx.state = AppState::BUILDER_MOTIVATION;
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

        case AppState::BUILDER_MOTIVATION: {
            int idx = ctx.selectedMenuIndex;
            if (key == KeyInput::LEFT) {
                if (idx % 2 == 1) idx--;
            } else if (key == KeyInput::RIGHT) {
                if (idx % 2 == 0 && idx + 1 < 5) idx++;
            } else if (key == KeyInput::UP) {
                if (idx >= 2) idx -= 2;
            } else if (key == KeyInput::DOWN) {
                if (idx + 2 < 5) idx += 2;
            } else if (key == KeyInput::ENTER) {
                Motivation m[] = { Motivation::WANT, Motivation::NEED, Motivation::CURIOUS, Motivation::FUN, Motivation::OPPORTUNITY };
                ctx.currentSelection.motivation = m[idx];
                ctx.state = AppState::BUILDER_CONCERN;
                ctx.selectedMenuIndex = 0;
                return;
            } else if (key == KeyInput::BACK) {
                ctx.state = AppState::BUILDER_TYPE;
                ctx.selectedMenuIndex = 0;
                return;
            }
            ctx.selectedMenuIndex = idx;
            break;
        }

        case AppState::BUILDER_CONCERN: {
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
                Concern c[] = { Concern::RISK, Concern::COST, Concern::TIME, Concern::EFFORT, Concern::UNKNOWN, Concern::NONE };
                ctx.currentSelection.concern = c[idx];
                ctx.state = AppState::BUILDER_INTENSITY;
                ctx.selectedMenuIndex = 0;
                return;
            } else if (key == KeyInput::BACK) {
                ctx.state = AppState::BUILDER_MOTIVATION;
                ctx.selectedMenuIndex = 0;
                return;
            }
            ctx.selectedMenuIndex = idx;
            break;
        }

        case AppState::BUILDER_INTENSITY: {
            int idx = ctx.selectedMenuIndex;
            if (key == KeyInput::LEFT) {
                if (idx > 0) idx--;
            } else if (key == KeyInput::RIGHT) {
                if (idx < 2) idx++;
            } else if (key == KeyInput::ENTER) {
                Intensity intens[] = { Intensity::LOW, Intensity::MEDIUM, Intensity::HIGH };
                ctx.currentSelection.intensity = intens[idx];
                ctx.state = AppState::BUILDER_PRIORITY;
                ctx.selectedMenuIndex = 0;
                return;
            } else if (key == KeyInput::BACK) {
                ctx.state = AppState::BUILDER_CONCERN;
                ctx.selectedMenuIndex = 0;
                return;
            }
            ctx.selectedMenuIndex = idx;
            break;
        }

        case AppState::BUILDER_PRIORITY: {
            int idx = ctx.selectedMenuIndex;
            if (key == KeyInput::LEFT) {
                if (idx % 2 == 1) idx--;
            } else if (key == KeyInput::RIGHT) {
                if (idx % 2 == 0 && idx + 1 < 4) idx++;
            } else if (key == KeyInput::UP) {
                if (idx >= 2) idx -= 2;
            } else if (key == KeyInput::DOWN) {
                if (idx + 2 < 4) idx += 2;
            } else if (key == KeyInput::ENTER) {
                Priority p[] = { Priority::EXPERIENCE, Priority::PRACTICAL, Priority::PEOPLE, Priority::SAFETY };
                ctx.currentSelection.priority = p[idx];
                ctx.currentScenario = buildScenario(ctx.currentSelection);
                ctx.state = AppState::BUILDER_PREVIEW;
                return;
            } else if (key == KeyInput::BACK) {
                ctx.state = AppState::BUILDER_INTENSITY;
                ctx.selectedMenuIndex = 0;
                return;
            }
            ctx.selectedMenuIndex = idx;
            break;
        }

        case AppState::BUILDER_PREVIEW:
            if (key == KeyInput::ENTER) {
                ctx.state = AppState::SIMULATING;
                ctx.animStartTime = 0;
                ctx.animProgress = 0;
            } else if (key == KeyInput::BACK) {
                ctx.state = AppState::BUILDER_PRIORITY;
                ctx.selectedMenuIndex = 0;
            }
            break;

        case AppState::SIMULATING:
            // 自动流转
            break;

        case AppState::SUMMARY:
            if (key == KeyInput::ENTER) {
                ctx.state = AppState::BIGGEST_SPLIT;
            }
            break;

        case AppState::BIGGEST_SPLIT:
            if (key == KeyInput::ENTER || key == KeyInput::RIGHT) {
                ctx.state = AppState::EXPLORE;
                ctx.exploreIndex = static_cast<int>(ctx.splitYesType);

                // 初始化 Explore 屏的雷达形态
                const PersonalityProfile& pProf = getMBTIProfile(ctx.results[ctx.exploreIndex].personality);
                RadarData target = { pProf.risk, pProf.novelty, pProf.logic, pProf.social, pProf.planning, pProf.practicality };
                ctx.currentRadar = target;
                ctx.startRadar = target;
                ctx.endRadar = target;
                ctx.isRadarAnimActive = false;
            }
            break;

        case AppState::EXPLORE:
            if (key == KeyInput::LEFT || key == KeyInput::RIGHT) {
                if (key == KeyInput::LEFT) {
                    ctx.exploreIndex = (ctx.exploreIndex + 15) % 16;
                } else {
                    ctx.exploreIndex = (ctx.exploreIndex + 1) % 16;
                }
                // 启动雷达极坐标补间平滑过渡形变动画
                const PersonalityProfile& pProf = getMBTIProfile(ctx.results[ctx.exploreIndex].personality);
                RadarData target = { pProf.risk, pProf.novelty, pProf.logic, pProf.social, pProf.planning, pProf.practicality };
                startRadarAnimation(ctx, target, now);
            } else if (key == KeyInput::ENTER) {
                ctx.state = AppState::YOUR_CHOICE;
                ctx.selectedMenuIndex = 0;
            } else if (key == KeyInput::BACK) {
                ctx.state = AppState::SUMMARY;
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

                // 计算用户的决策轮廓并匹配相近度最高的人格
                ctx.userProfile = calculateDecisionProfile(ctx.currentScenario, ctx.userChoice);
                ctx.closestMBTI = findClosestMBTI(ctx.userProfile, ctx.matchSimilarity);

                // 启动用户 Profile 雷达图的过渡呈现
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
                ctx.state = AppState::EXPLORE;
            }
            break;

        case AppState::YOUR_MATCH:
            if (key == KeyInput::ENTER) {
                ctx.state = AppState::HOME;
                ctx.selectedMenuIndex = 0;
            }
            break;
    }
}
