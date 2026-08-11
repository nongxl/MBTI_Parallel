#include "DisplayRenderer.h"
#include "RadarChart.h"
#include "ScenarioBuilder.h"
#include <cstdio>
#include <cstring>
#include <cmath>

#ifdef ARDUINO
#include <Arduino.h>
#include <M5Cardputer.h>

static M5Canvas canvas(&M5Cardputer.Display);
#else
static uint32_t millis() { return 0; }
#endif

void initDisplay() {
#ifdef ARDUINO
    M5Cardputer.Display.begin();
    M5Cardputer.Display.setRotation(1);
    canvas.createSprite(240, 135);
    canvas.setTextColor(WHITE, BLACK);
    canvas.setTextSize(1);
#endif
}

#ifdef ARDUINO
static void drawHeader(const char* title, bool isCN) {
    canvas.fillScreen(BLACK);
    canvas.setTextColor(YELLOW, BLACK);
    if (isCN) {
        canvas.setFont(&fonts::efontCN_12);
        canvas.setTextSize(1);
        canvas.setCursor(5, 4);
    } else {
        canvas.setFont(&fonts::Font0);
        canvas.setTextSize(2);
        canvas.setCursor(5, 3);
    }
    canvas.print(title);
    canvas.drawLine(0, 22, 240, 22, GREEN);
}

static void drawFooter(const char* hint, bool isCN) {
    canvas.setTextColor(LIGHTGREY, BLACK);
    if (isCN) {
        canvas.setFont(&fonts::efontCN_12);
        canvas.setTextSize(1);
        canvas.setCursor(5, 122);
    } else {
        canvas.setFont(&fonts::Font0);
        canvas.setTextSize(1);
        canvas.setCursor(5, 123);
    }
    canvas.print(hint);
}
#endif

void renderUI(const UIContext& ctx) {
#ifdef ARDUINO
    bool isCN = (ctx.lang == Language::CHINESE);
    if (isCN) {
        canvas.setFont(&fonts::efontCN_12);
        canvas.setTextSize(1);
    } else {
        canvas.setFont(&fonts::Font0);
        canvas.setTextSize(1);
    }

    switch (ctx.state) {
        case AppState::LANGUAGE_SELECT: {
            // 首次开机语言选择屏幕 (只在初次烧录或未配置时弹出)
            canvas.fillScreen(BLACK);
            
            drawHeader("SELECT LANGUAGE / 选择语言", true);

            canvas.setFont(&fonts::efontCN_12);
            canvas.setTextSize(1);

            // 中文选项选框
            if (ctx.selectedMenuIndex == 0) {
                canvas.setTextColor(CYAN, BLACK);
                canvas.setCursor(30, 48);
                canvas.print("> 1. 中文 (Simplified Chinese)");
                canvas.setTextColor(WHITE, BLACK);
                canvas.setCursor(30, 78);
                canvas.print("  2. English (US)");
            } else {
                canvas.setTextColor(WHITE, BLACK);
                canvas.setCursor(30, 48);
                canvas.print("  1. 中文 (Simplified Chinese)");
                canvas.setTextColor(CYAN, BLACK);
                canvas.setCursor(30, 78);
                canvas.print("> 2. English (US)");
            }

            drawFooter("[左右/上下] 移动  [ENTER] 确认并保存", true);
            break;
        }

        case AppState::HOME: {
            canvas.fillScreen(BLACK);
            
            // 基于运行时间计算 6 维正弦波律动波形 (Sinusoidal Morphing Wave)
            uint32_t now = millis();
            float t = now / 1000.0f;
            RadarData homeData = {
                72.0f + 18.0f * sinf(t * 1.3f),
                82.0f + 14.0f * cosf(t * 0.9f + 1.1f),
                68.0f + 16.0f * sinf(t * 1.6f + 2.2f),
                78.0f + 15.0f * cosf(t * 1.1f + 3.3f),
                55.0f + 20.0f * sinf(t * 0.8f + 4.4f),
                74.0f + 16.0f * cosf(t * 1.4f + 5.5f)
            };

            // 图层 1: 【背景大标题 PARALLEL】(先在背景层打印暗浅文本，形成穿透透视)
            canvas.setFont(&fonts::Font0);
            canvas.setTextColor(DARKGREY, BLACK);
            canvas.setTextSize(3);
            canvas.setCursor(48, 30);
            canvas.print("PARALLEL");

            // 图层 2: 【前景六边形发光雷达图】(直接叠加在 PARALLEL 大标题上，穿透立体感极强)
            drawRadarChart(canvas, 120, 44, 46, homeData, GREEN, DARKCYAN, false, isCN);

            // 图层 3: 开机双模式切选菜单 (1. 🎲 随机模式 / 2. 🛠️ 自定义模式)
            if (isCN) {
                canvas.setFont(&fonts::efontCN_12);
                canvas.setTextSize(1);

                if (ctx.bootMenuMode == 0) {
                    canvas.setTextColor(CYAN, BLACK);
                    canvas.setCursor(32, 85);
                    canvas.print("> 1. 🎲 随机场景模式");
                    canvas.setTextColor(LIGHTGREY, BLACK);
                    canvas.setCursor(32, 101);
                    canvas.print("  2. 🛠️ 自定义场景构造");
                } else {
                    canvas.setTextColor(LIGHTGREY, BLACK);
                    canvas.setCursor(32, 85);
                    canvas.print("  1. 🎲 随机场景模式");
                    canvas.setTextColor(CYAN, BLACK);
                    canvas.setCursor(32, 101);
                    canvas.print("> 2. 🛠️ 自定义场景构造");
                }
            } else {
                canvas.setFont(&fonts::Font0);
                canvas.setTextSize(1);

                if (ctx.bootMenuMode == 0) {
                    canvas.setTextColor(CYAN, BLACK);
                    canvas.setCursor(32, 85);
                    canvas.print("> 1. RANDOM SCENARIO");
                    canvas.setTextColor(LIGHTGREY, BLACK);
                    canvas.setCursor(32, 101);
                    canvas.print("  2. CREATE SCENARIO");
                } else {
                    canvas.setTextColor(LIGHTGREY, BLACK);
                    canvas.setCursor(32, 85);
                    canvas.print("  1. RANDOM SCENARIO");
                    canvas.setTextColor(CYAN, BLACK);
                    canvas.setCursor(32, 101);
                    canvas.print("> 2. CREATE SCENARIO");
                }
            }

            // 图层 4: 底栏 Footer 引导
            const char* homeFooter = isCN ? "[上下] 切换模式  [ENTER] 开始测试" : "[UP/DN] Select Mode  [ENTER] Start";
            drawFooter(homeFooter, isCN);

            if (ctx.totalPlays > 0) {
                canvas.setFont(&fonts::Font0);
                canvas.setTextColor(DARKGREY, BLACK);
                canvas.setCursor(185, 5);
                canvas.printf("PLS:%d", ctx.totalPlays);
            }
            break;
        }

        case AppState::BUILDER_WHO:
        case AppState::BUILDER_SITUATION:
        case AppState::BUILDER_CONDITION:
        case AppState::BUILDER_TENSION: {
            const char* title = isCN ? "构造场景" : "CREATE SCENARIO";
            if (ctx.state == AppState::BUILDER_WHO) title = isCN ? "1. 谁？ (WHO)" : "1. WHO?";
            else if (ctx.state == AppState::BUILDER_SITUATION) title = isCN ? "2. 什么情境？ (SITUATION)" : "2. SITUATION?";
            else if (ctx.state == AppState::BUILDER_CONDITION) title = isCN ? "3. 特殊条件？ (CONDITION)" : "3. CONDITION?";
            else if (ctx.state == AppState::BUILDER_TENSION) title = isCN ? "4. 纠结什么？ (TENSION)" : "4. WHAT'S AT STAKE?";

            drawHeader(title, isCN);

            int count = 6;
            const char* items[6];
            const char* icons[] = { "◇", "○", "△", "□", "☆", "⬡" };

            if (ctx.state == AppState::BUILDER_WHO) {
                for (int i = 0; i < 6; ++i) items[i] = getWhoName(static_cast<WhoType>(i), isCN);
            } else if (ctx.state == AppState::BUILDER_SITUATION) {
                for (int i = 0; i < 6; ++i) items[i] = getSituationName(static_cast<SituationType>(i), isCN);
            } else if (ctx.state == AppState::BUILDER_CONDITION) {
                for (int i = 0; i < 6; ++i) items[i] = getConditionName(static_cast<ConditionType>(i), isCN);
            } else if (ctx.state == AppState::BUILDER_TENSION) {
                for (int i = 0; i < 6; ++i) items[i] = getTensionName(static_cast<TensionType>(i), isCN);
            }

            for (int i = 0; i < count; ++i) {
                int col = i % 2;
                int row = i / 2;
                int x = (col == 0) ? 6 : 122;
                int y = 30 + row * 28;

                if (isCN) {
                    canvas.setFont(&fonts::efontCN_12);
                    canvas.setTextSize(1);
                } else {
                    canvas.setFont(&fonts::Font0);
                    canvas.setTextSize(1);
                }

                canvas.setCursor(x, y);
                if (i == ctx.selectedMenuIndex) {
                    canvas.setTextColor(CYAN, BLACK);
                    canvas.printf("> %s %s", icons[i % 6], items[i]);
                } else {
                    canvas.setTextColor(WHITE, BLACK);
                    canvas.printf("  %s %s", icons[i % 6], items[i]);
                }
            }

            drawFooter(isCN ? "[方向] 2D选择 [ENTER] 确定 [ESC] 返回" : "[ARR] 2D Move [ENTER] Select [ESC] Back", isCN);
            break;
        }

        case AppState::BUILDER_PREVIEW: {
            // 【Phase 4 规范：Decide First, Discover Later】
            // 纯粹、全屏宽大字展现场景题目与描述！彻底移除混淆的右侧雷达图！
            drawHeader(isCN ? "场景预览" : "SCENARIO PREVIEW", isCN);
            
            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            canvas.setTextSize(1);
            canvas.setTextColor(YELLOW, BLACK);
            canvas.setCursor(8, 28);
            const char* titleTxt = isCN ? (ctx.currentScenarioTitleCN[0] ? ctx.currentScenarioTitleCN : "离线情境")
                                        : (ctx.currentScenarioTitle[0] ? ctx.currentScenarioTitle : "SCENARIO");
            canvas.print(titleTxt);

            canvas.setTextColor(WHITE, BLACK);
            canvas.setCursor(8, 46);
            
            // 占用全屏宽 (X = 8 ~ 232) 自动折行打印场景描述
            const char* desc = isCN ? (ctx.currentScenarioDescCN[0] ? ctx.currentScenarioDescCN : "")
                                    : (ctx.currentScenarioDesc[0] ? ctx.currentScenarioDesc : "");
            int lineY = 46;
            while (*desc && lineY <= 102) {
                char buf[36] = {0};
                int takeBytes = isCN ? 32 : 32;
                strncpy(buf, desc, takeBytes);
                canvas.setCursor(8, lineY);
                canvas.print(buf);
                desc += strlen(buf);
                lineY += 15;
            }

            drawFooter(isCN ? "[ENTER] 做出选择  [ESC] 修改/返回" : "[ENTER] MAKE DECISION  [ESC] EDIT", isCN);
            break;
        }

        case AppState::YOUR_CHOICE: {
            drawHeader(isCN ? "如果是你，你怎么选？" : "WHAT WOULD YOU DO?", isCN);
            const char* choices[] = { "YES", "NO", "MAYBE" };
            const char* choicesCN[] = { "同意 (YES)", "拒绝 (NO)", "犹豫 (MAYBE)" };
            for (int i = 0; i < 3; ++i) {
                if (isCN) {
                    canvas.setFont(&fonts::efontCN_12);
                    canvas.setTextSize(1);
                } else {
                    canvas.setFont(&fonts::Font0);
                    canvas.setTextSize(2);
                }

                canvas.setCursor(10, 38 + i * 26);
                const char* txt = isCN ? choicesCN[i] : choices[i];
                if (i == ctx.selectedMenuIndex) {
                    canvas.setTextColor(GREEN, BLACK);
                    canvas.printf("> %s", txt);
                } else {
                    canvas.setTextColor(WHITE, BLACK);
                    canvas.printf("  %s", txt);
                }
            }
            drawFooter(isCN ? "[上下] 移动  [ENTER] 确认选择" : "[UP/DN] Move  [ENTER] Confirm", isCN);
            break;
        }

        case AppState::YOUR_MATCH: {
            // 【Phase 4 Reveal 揭晓屏】：同时优雅呈现【本题决策雷达图】+【最吻合人格】+【最大分歧人格】
            drawHeader(isCN ? "你的决策轮廓" : "YOUR DECISION PROFILE", isCN);
            
            // 左侧绘制用户本题决策轮廓雷达图 (centerX = 62, centerY = 68, radius = 35)
            drawRadarChart(canvas, 62, 68, 35, ctx.currentRadar, GREEN, DARKGREEN, true, isCN);

            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            // 右侧区域 (X = 124 ~ 238) 精密双栏布局
            // 1. 最吻合 MBTI (MOST LIKE)
            canvas.setTextSize(1);
            canvas.setTextColor(WHITE, BLACK);
            canvas.setCursor(124, 28);
            canvas.print(isCN ? "最像人格:" : "MOST LIKE:");

            canvas.setFont(&fonts::Font0);
            canvas.setTextSize(1);
            canvas.setTextColor(YELLOW, BLACK);
            canvas.setCursor(124, 42);
            canvas.printf("%s (%.1f%%)", getMBTIName(ctx.closestMBTI), ctx.matchSimilarity);

            // 2. 最大分歧 MBTI (BIGGEST DIFFERENCE)
            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            canvas.setTextSize(1);
            canvas.setTextColor(LIGHTGREY, BLACK);
            canvas.setCursor(124, 62);
            canvas.print(isCN ? "最大分歧:" : "BIGGEST DIFF:");

            canvas.setFont(&fonts::Font0);
            canvas.setTextSize(1);
            canvas.setTextColor(RED, BLACK);
            canvas.setCursor(124, 76);
            const char* diffDecTxt = isCN ? getDecisionNameCN(ctx.biggestDiffDecision) : getDecisionName(ctx.biggestDiffDecision);
            canvas.printf("%s (%s)", getMBTIName(ctx.biggestDiffMBTI), diffDecTxt);

            // 3. 用户选择
            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            canvas.setTextSize(1);
            canvas.setTextColor(CYAN, BLACK);
            canvas.setCursor(124, 96);
            const char* myDecTxt = isCN ? getDecisionNameCN(ctx.userChoice) : getDecisionName(ctx.userChoice);
            canvas.printf(isCN ? "你的选择: %s" : "YOURS: %s", myDecTxt);

            if (ctx.totalPlays > 0) {
                canvas.setFont(&fonts::Font0);
                canvas.setTextColor(DARKGREY, BLACK);
                canvas.setCursor(185, 5);
                canvas.printf("PLS:%d", ctx.totalPlays);
            }

            drawFooter(isCN ? "[ENTER] 探索 16 人格 >  [ESC] 首页" : "[ENTER] EXPLORE 16 MBTI >  [ESC] HOME", isCN);
            break;
        }

        case AppState::EXPLORE: {
            const DecisionResult& res = ctx.results[ctx.exploreIndex];
            char headerTitle[32];
            snprintf(headerTitle, sizeof(headerTitle), "< %s > (%d/16)", getMBTIName(res.personality), ctx.exploreIndex + 1);
            drawHeader(headerTitle, isCN);

            // 右侧区域(centerX = 180, centerY = 68, radius = 35) 带防越界卡死保护与双语轴标签
            drawRadarChart(canvas, 180, 68, 35, ctx.currentRadar, CYAN, DARKCYAN, true, isCN);

            // 左侧区域
            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            canvas.setTextSize(1);
            canvas.setTextColor(WHITE, BLACK);
            canvas.setCursor(6, 28);
            canvas.print(isCN ? "选择: " : "Choice: ");

            // 决策结果 YES / NO / MAYBE 中英文翻译
            const char* decName = isCN ? getDecisionNameCN(res.decision) : getDecisionName(res.decision);

            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            canvas.setTextSize(isCN ? 1 : 2);
            if (res.decision == Decision::YES) {
                canvas.setTextColor(GREEN, BLACK);
            } else if (res.decision == Decision::NO) {
                canvas.setTextColor(RED, BLACK);
            } else {
                canvas.setTextColor(YELLOW, BLACK);
            }
            canvas.print(decName);

            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            canvas.setTextSize(1);
            canvas.setTextColor(WHITE, BLACK);
            canvas.setCursor(6, 50);
            canvas.printf(isCN ? "得分: %.1f" : "Score : %.1f", res.score);

            canvas.setCursor(6, 66);
            canvas.setTextColor(CYAN, BLACK);
            canvas.print(isCN ? "依据:" : "Reason:");

            // 决策依据理由双语映射 (英文原因转换为地道中文)
            const char* pReason = isCN ? getDecisionReasonCN(res.reason) : res.reason;
            int lineY = 79;
            while (*pReason && lineY <= 105) {
                char lineBuf[22] = {0};
                int maxChars = isCN ? 10 : 16;
                int len = (int)strlen(pReason);
                if (len <= maxChars) {
                    strcpy(lineBuf, pReason);
                    pReason += len;
                } else {
                    int breakPos = maxChars;
                    if (!isCN) {
                        for (int k = maxChars; k >= 4; --k) {
                            if (pReason[k] == ' ' || pReason[k] == ';' || pReason[k] == '+') {
                                breakPos = k;
                                break;
                            }
                        }
                    }
                    strncpy(lineBuf, pReason, breakPos);
                    lineBuf[breakPos] = '\0';
                    pReason += breakPos;
                    if (!isCN && (*pReason == ' ' || *pReason == ';')) pReason++;
                }
                canvas.setCursor(6, lineY);
                canvas.setTextColor(LIGHTGREY, BLACK);
                canvas.print(lineBuf);
                lineY += 13;
            }

            drawFooter(isCN ? "[左右] 切换  [ENTER] 下一个场景" : "[L/R] Switch  [ENTER] NEXT SCENARIO", isCN);
            break;
        }

        case AppState::SIMULATING:
        case AppState::SUMMARY:
        case AppState::BIGGEST_SPLIT:
            break;
    }

    // 离屏画布单次推送
    canvas.pushSprite(0, 0);
#else
    // 降级控制台调试打印
    static AppState lastState = (AppState)-1;
    if (lastState != ctx.state) {
        lastState = ctx.state;
        printf("\n[UI RENDER] State changed to: %d\n", static_cast<int>(ctx.state));
    }
#endif
}
