#include "AppController.h"
#include "ScenarioMapper.h"
#include "DisplayRenderer.h"
#include "DecisionProfile.h"

void initApp(UIContext& ctx) {
    ctx.state = AppState::HOME;
    ctx.selectedMenuIndex = 0;
    ctx.exploreIndex = 0;
    ctx.userChoice = Decision::YES;
    ctx.animStartTime = 0;
    ctx.animProgress = 0;

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
}

void handleInput(UIContext& ctx, KeyInput key) {
    if (key == KeyInput::NONE) return;

    switch (ctx.state) {
        case AppState::HOME:
            if (key == KeyInput::ENTER) {
                ctx.state = AppState::BUILDER_TYPE;
                ctx.selectedMenuIndex = 0;
            }
            break;

        case AppState::BUILDER_TYPE:
            if (key == KeyInput::UP) {
                ctx.selectedMenuIndex = (ctx.selectedMenuIndex + 5) % 6;
            } else if (key == KeyInput::DOWN) {
                ctx.selectedMenuIndex = (ctx.selectedMenuIndex + 1) % 6;
            } else if (key == KeyInput::ENTER) {
                DecisionType types[] = { DecisionType::GET, DecisionType::GO, DecisionType::DO, DecisionType::SAY, DecisionType::CHOOSE, DecisionType::CHANGE };
                ctx.currentSelection.decisionType = types[ctx.selectedMenuIndex];
                ctx.state = AppState::BUILDER_MOTIVATION;
                ctx.selectedMenuIndex = 0;
            } else if (key == KeyInput::BACK) {
                ctx.state = AppState::HOME;
                ctx.selectedMenuIndex = 0;
            }
            break;

        case AppState::BUILDER_MOTIVATION:
            if (key == KeyInput::UP) {
                ctx.selectedMenuIndex = (ctx.selectedMenuIndex + 4) % 5;
            } else if (key == KeyInput::DOWN) {
                ctx.selectedMenuIndex = (ctx.selectedMenuIndex + 1) % 5;
            } else if (key == KeyInput::ENTER) {
                Motivation m[] = { Motivation::WANT, Motivation::NEED, Motivation::CURIOUS, Motivation::FUN, Motivation::OPPORTUNITY };
                ctx.currentSelection.motivation = m[ctx.selectedMenuIndex];
                ctx.state = AppState::BUILDER_CONCERN;
                ctx.selectedMenuIndex = 0;
            } else if (key == KeyInput::BACK) {
                ctx.state = AppState::BUILDER_TYPE;
                ctx.selectedMenuIndex = 0;
            }
            break;

        case AppState::BUILDER_CONCERN:
            if (key == KeyInput::UP) {
                ctx.selectedMenuIndex = (ctx.selectedMenuIndex + 5) % 6;
            } else if (key == KeyInput::DOWN) {
                ctx.selectedMenuIndex = (ctx.selectedMenuIndex + 1) % 6;
            } else if (key == KeyInput::ENTER) {
                Concern c[] = { Concern::RISK, Concern::COST, Concern::TIME, Concern::EFFORT, Concern::UNKNOWN, Concern::NONE };
                ctx.currentSelection.concern = c[ctx.selectedMenuIndex];
                ctx.state = AppState::BUILDER_INTENSITY;
                ctx.selectedMenuIndex = 0;
            } else if (key == KeyInput::BACK) {
                ctx.state = AppState::BUILDER_MOTIVATION;
                ctx.selectedMenuIndex = 0;
            }
            break;

        case AppState::BUILDER_INTENSITY:
            if (key == KeyInput::UP) {
                ctx.selectedMenuIndex = (ctx.selectedMenuIndex + 2) % 3;
            } else if (key == KeyInput::DOWN) {
                ctx.selectedMenuIndex = (ctx.selectedMenuIndex + 1) % 3;
            } else if (key == KeyInput::ENTER) {
                Intensity intens[] = { Intensity::LOW, Intensity::MEDIUM, Intensity::HIGH };
                ctx.currentSelection.intensity = intens[ctx.selectedMenuIndex];
                ctx.state = AppState::BUILDER_PRIORITY;
                ctx.selectedMenuIndex = 0;
            } else if (key == KeyInput::BACK) {
                ctx.state = AppState::BUILDER_CONCERN;
                ctx.selectedMenuIndex = 0;
            }
            break;

        case AppState::BUILDER_PRIORITY:
            if (key == KeyInput::UP) {
                ctx.selectedMenuIndex = (ctx.selectedMenuIndex + 3) % 4;
            } else if (key == KeyInput::DOWN) {
                ctx.selectedMenuIndex = (ctx.selectedMenuIndex + 1) % 4;
            } else if (key == KeyInput::ENTER) {
                Priority p[] = { Priority::EXPERIENCE, Priority::PRACTICAL, Priority::PEOPLE, Priority::SAFETY };
                ctx.currentSelection.priority = p[ctx.selectedMenuIndex];
                ctx.currentScenario = buildScenario(ctx.currentSelection);
                ctx.state = AppState::BUILDER_PREVIEW;
            } else if (key == KeyInput::BACK) {
                ctx.state = AppState::BUILDER_INTENSITY;
                ctx.selectedMenuIndex = 0;
            }
            break;

        case AppState::BUILDER_PREVIEW:
            if (key == KeyInput::ENTER) {
                ctx.state = AppState::SIMULATING;
                ctx.animStartTime = 0; // 标记在 updateApp 中自动设为当下的 currentMillis
                ctx.animProgress = 0;
            } else if (key == KeyInput::BACK) {
                ctx.state = AppState::BUILDER_PRIORITY;
                ctx.selectedMenuIndex = 0;
            }
            break;

        case AppState::SIMULATING:
            // 自动流转，不响应该阶段按键输入
            break;

        case AppState::SUMMARY:
            if (key == KeyInput::ENTER) {
                ctx.state = AppState::BIGGEST_SPLIT;
            }
            break;

        case AppState::BIGGEST_SPLIT:
            if (key == KeyInput::ENTER || key == KeyInput::RIGHT) {
                ctx.state = AppState::EXPLORE;
                ctx.exploreIndex = static_cast<int>(ctx.splitYesType); // 优先定位 Biggest Split 的 YES 型
            }
            break;

        case AppState::EXPLORE:
            if (key == KeyInput::LEFT) {
                ctx.exploreIndex = (ctx.exploreIndex + 15) % 16;
            } else if (key == KeyInput::RIGHT) {
                ctx.exploreIndex = (ctx.exploreIndex + 1) % 16;
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
