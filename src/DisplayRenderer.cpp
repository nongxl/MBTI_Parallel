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
            canvas.fillScreen(BLACK);
            drawHeader("SELECT LANGUAGE / 选择语言", true);

            canvas.setFont(&fonts::efontCN_12);
            canvas.setTextSize(1);

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

            // 图层 1: 【背景大标题 PARALLEL】
            canvas.setFont(&fonts::Font0);
            canvas.setTextColor(DARKGREY, BLACK);
            canvas.setTextSize(3);
            canvas.setCursor(48, 25);
            canvas.print("PARALLEL");

            // 图层 2: 【前景六边形发光雷达图】
            drawRadarChart(canvas, 120, 38, 42, homeData, GREEN, DARKCYAN, false, isCN);

            // 图层 3: 开机三大模式切选菜单 (1. 🎲 随机模式 / 2. 🛠️ 自定义模式 / 3. 👤 真实人格画像)
            if (isCN) {
                canvas.setFont(&fonts::efontCN_12);
                canvas.setTextSize(1);

                const char* m0 = "  1. 🎲 随机场景模式";
                const char* m1 = "  2. 🛠️ 自定义场景构造";
                const char* m2 = "  3. 👤 真实人格画像";

                if (ctx.bootMenuMode == 0) m0 = "> 1. 🎲 随机场景模式";
                else if (ctx.bootMenuMode == 1) m1 = "> 2. 🛠️ 自定义场景构造";
                else m2 = "> 3. 👤 真实人格画像";

                canvas.setTextColor((ctx.bootMenuMode == 0) ? CYAN : LIGHTGREY, BLACK);
                canvas.setCursor(32, 75);
                canvas.print(m0);

                canvas.setTextColor((ctx.bootMenuMode == 1) ? CYAN : LIGHTGREY, BLACK);
                canvas.setCursor(32, 90);
                canvas.print(m1);

                canvas.setTextColor((ctx.bootMenuMode == 2) ? CYAN : LIGHTGREY, BLACK);
                canvas.setCursor(32, 105);
                canvas.print(m2);
            } else {
                canvas.setFont(&fonts::Font0);
                canvas.setTextSize(1);

                const char* m0 = "  1. RANDOM SCENARIO";
                const char* m1 = "  2. CREATE SCENARIO";
                const char* m2 = "  3. MY MBTI PROFILE";

                if (ctx.bootMenuMode == 0) m0 = "> 1. RANDOM SCENARIO";
                else if (ctx.bootMenuMode == 1) m1 = "> 2. CREATE SCENARIO";
                else m2 = "> 3. MY MBTI PROFILE";

                canvas.setTextColor((ctx.bootMenuMode == 0) ? CYAN : LIGHTGREY, BLACK);
                canvas.setCursor(32, 75);
                canvas.print(m0);

                canvas.setTextColor((ctx.bootMenuMode == 1) ? CYAN : LIGHTGREY, BLACK);
                canvas.setCursor(32, 90);
                canvas.print(m1);

                canvas.setTextColor((ctx.bootMenuMode == 2) ? CYAN : LIGHTGREY, BLACK);
                canvas.setCursor(32, 105);
                canvas.print(m2);
            }

            // 图层 4: 底栏 Footer 引导
            const char* homeFooter = isCN ? "[上下] 模式  [ENTER] 开始" : "[UP/DN] Mode  [ENTER] Start";
            drawFooter(homeFooter, isCN);

            if (ctx.totalPlays > 0) {
                canvas.setFont(&fonts::Font0);
                canvas.setTextColor(DARKGREY, BLACK);
                canvas.setCursor(185, 5);
                canvas.printf("PLS:%d", ctx.totalPlays);
            }
            break;
        }

        case AppState::MY_PROFILE: {
            // 【规范要求：左侧显示文字统计，右侧显示六边形雷达图】
            drawHeader(isCN ? "真实长效 MBTI 画板" : "MY LONG-TERM PROFILE", isCN);

            // 右侧绘制 NVS 累积极坐标发光雷达图 (centerX = 180, centerY = 68, radius = 35)
            drawRadarChart(canvas, 180, 68, 35, ctx.currentRadar, GREEN, DARKCYAN, true, isCN);

            // 左侧信息区域 (X = 6 ~ 124)
            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            canvas.setTextSize(1);
            canvas.setTextColor(WHITE, BLACK);
            canvas.setCursor(6, 28);
            canvas.print(isCN ? "收敛真实人格:" : "LONG-TERM MBTI:");

            canvas.setFont(&fonts::Font0);
            canvas.setTextSize(2);
            canvas.setTextColor(YELLOW, BLACK);
            canvas.setCursor(6, 43);
            canvas.print(getMBTIName(ctx.userHistory.dominantMBTI));

            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            canvas.setTextSize(1);
            canvas.setTextColor(CYAN, BLACK);
            canvas.setCursor(6, 64);
            canvas.printf(isCN ? "置信度: %.1f%%" : "Confid : %.1f%%", ctx.userHistory.dominantSimilarity);

            canvas.setTextColor(WHITE, BLACK);
            canvas.setCursor(6, 80);
            canvas.printf(isCN ? "已测场数: %d 场" : "Tested : %d", ctx.userHistory.totalPlays);

            canvas.setTextColor(LIGHTGREY, BLACK);
            canvas.setCursor(6, 96);
            canvas.printf(isCN ? "同意%d 拒绝%d 犹豫%d" : "Y:%d N:%d M:%d",
                          ctx.userHistory.yesCount, ctx.userHistory.noCount, ctx.userHistory.maybeCount);

            drawFooter(isCN ? "[ENTER/ESC] 返回  [上下] 清空" : "[ENTER/ESC] Back  [UP/DN] Clear", isCN);
            break;
        }

        case AppState::MY_PROFILE_CLEAR_CONFIRM: {
            // 清空历史二次确认警告弹窗
            drawHeader(isCN ? "⚠️ 清空历史确认" : "⚠️ CLEAR HISTORY", isCN);

            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            canvas.setTextSize(1);
            canvas.setTextColor(RED, BLACK);
            canvas.setCursor(10, 32);
            canvas.print(isCN ? "确定擦除所有决策历史与真实人格吗？" : "Clear all decision history & profile?");

            // 二级选项 1. 清空 / 2. 取消
            if (ctx.selectedMenuIndex == 0) {
                canvas.setTextColor(RED, BLACK);
                canvas.setCursor(20, 60);
                canvas.print(isCN ? "> [ 1. ⚠️ 清空历史 (CLEAR) ]" : "> [ 1. CLEAR ALL HISTORY ]");
                canvas.setTextColor(WHITE, BLACK);
                canvas.setCursor(20, 88);
                canvas.print(isCN ? "  [ 2. 🛡️ 取消保留 (CANCEL) ]" : "  [ 2. CANCEL ]");
            } else {
                canvas.setTextColor(WHITE, BLACK);
                canvas.setCursor(20, 60);
                canvas.print(isCN ? "  [ 1. ⚠️ 清空历史 (CLEAR) ]" : "  [ 1. CLEAR ALL HISTORY ]");
                canvas.setTextColor(GREEN, BLACK);
                canvas.setCursor(20, 88);
                canvas.print(isCN ? "> [ 2. 🛡️ 取消保留 (CANCEL) ]" : "> [ 2. CANCEL ]");
            }

            drawFooter(isCN ? "[方向] 选择  [ENTER] 确认执行" : "[ARR] Select  [ENTER] Confirm", isCN);
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
            drawHeader(isCN ? "你的决策轮廓" : "YOUR DECISION PROFILE", isCN);
            
            drawRadarChart(canvas, 62, 68, 35, ctx.currentRadar, GREEN, DARKGREEN, true, isCN);

            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            // 右侧区域 (X = 124 ~ 238)
            canvas.setTextSize(1);
            canvas.setTextColor(WHITE, BLACK);
            canvas.setCursor(124, 28);
            canvas.print(isCN ? "最像人格:" : "MOST LIKE:");

            canvas.setFont(&fonts::Font0);
            canvas.setTextSize(1);
            canvas.setTextColor(YELLOW, BLACK);
            canvas.setCursor(124, 42);
            canvas.printf("%s (%.1f%%)", getMBTIName(ctx.closestMBTI), ctx.matchSimilarity);

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

            drawRadarChart(canvas, 180, 68, 35, ctx.currentRadar, CYAN, DARKCYAN, true, isCN);

            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            canvas.setTextSize(1);
            canvas.setTextColor(WHITE, BLACK);
            canvas.setCursor(6, 28);
            canvas.print(isCN ? "选择: " : "Choice: ");

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

    canvas.pushSprite(0, 0);
#else
    static AppState lastState = (AppState)-1;
    if (lastState != ctx.state) {
        lastState = ctx.state;
        printf("\n[UI RENDER] State changed to: %d\n", static_cast<int>(ctx.state));
    }
#endif
}
