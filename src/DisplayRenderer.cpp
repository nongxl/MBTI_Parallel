#include "DisplayRenderer.h"
#include "RadarChart.h"
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

            // 绘制大幅放大的科技感主六边形图 (centerX = 120, centerY = 46, radius = 48)
            drawRadarChart(canvas, 120, 46, 48, homeData, GREEN, DARKCYAN, false, isCN);

            // 标题文字 PARALLEL
            canvas.setFont(&fonts::Font0);
            canvas.setTextColor(YELLOW, BLACK);
            canvas.setTextSize(2);
            canvas.setCursor(72, 96);
            canvas.print("PARALLEL");

            // 开机界面语言选择控件: 中文在前并默认选中！
            canvas.setFont(&fonts::efontCN_12);
            canvas.setTextSize(1);
            if (isCN) {
                canvas.setTextColor(CYAN, BLACK);
                canvas.setCursor(35, 118);
                canvas.print("> [中文] <   [ENGLISH]");
            } else {
                canvas.setTextColor(CYAN, BLACK);
                canvas.setCursor(35, 118);
                canvas.print("  [中文]   > [ENGLISH] <");
            }

            if (ctx.totalPlays > 0) {
                canvas.setFont(&fonts::Font0);
                canvas.setTextColor(DARKGREY, BLACK);
                canvas.setCursor(185, 5);
                canvas.printf("PLS:%d", ctx.totalPlays);
            }
            break;
        }

        case AppState::BUILDER_TYPE:
        case AppState::BUILDER_MOTIVATION:
        case AppState::BUILDER_CONCERN:
        case AppState::BUILDER_INTENSITY:
        case AppState::BUILDER_PRIORITY: {
            const char* title = isCN ? "步骤" : "STEP";
            if (ctx.state == AppState::BUILDER_TYPE) title = isCN ? "1. 决策动作" : "1. DECISION TYPE";
            else if (ctx.state == AppState::BUILDER_MOTIVATION) title = isCN ? "2. 驱动动机" : "2. MOTIVATION";
            else if (ctx.state == AppState::BUILDER_CONCERN) title = isCN ? "3. 核心顾虑" : "3. MAIN CONCERN";
            else if (ctx.state == AppState::BUILDER_INTENSITY) title = isCN ? "4. 强度级别" : "4. INTENSITY";
            else if (ctx.state == AppState::BUILDER_PRIORITY) title = isCN ? "5. 优先偏好" : "5. PRIORITY";

            drawHeader(title, isCN);

            // 2D 网格排列
            int count = 0;
            const char* items[8];
            const char* itemsCN[8];
            const char* icons[] = { "◇", "○", "△", "□", "☆", "⬡" };

            if (ctx.state == AppState::BUILDER_TYPE) {
                count = 6;
                items[0]="GET"; items[1]="GO"; items[2]="DO"; items[3]="SAY"; items[4]="CHOOSE"; items[5]="CHANGE";
                itemsCN[0]="获得"; itemsCN[1]="前往"; itemsCN[2]="执行"; itemsCN[3]="表达"; itemsCN[4]="选择"; itemsCN[5]="改变";
            } else if (ctx.state == AppState::BUILDER_MOTIVATION) {
                count = 5;
                items[0]="WANT"; items[1]="NEED"; items[2]="CURIOUS"; items[3]="FUN"; items[4]="OPPORTUNITY";
                itemsCN[0]="渴望"; itemsCN[1]="刚需"; itemsCN[2]="好奇"; itemsCN[3]="乐趣"; itemsCN[4]="机遇";
            } else if (ctx.state == AppState::BUILDER_CONCERN) {
                count = 6;
                items[0]="RISK"; items[1]="COST"; items[2]="TIME"; items[3]="EFFORT"; items[4]="UNKNOWN"; items[5]="NONE";
                itemsCN[0]="风险"; itemsCN[1]="花费"; itemsCN[2]="耗时"; itemsCN[3]="精力"; itemsCN[4]="未知"; itemsCN[5]="无";
            } else if (ctx.state == AppState::BUILDER_INTENSITY) {
                count = 3;
                items[0]="LOW"; items[1]="MEDIUM"; items[2]="HIGH";
                itemsCN[0]="轻度"; itemsCN[1]="中度"; itemsCN[2]="重度";
            } else if (ctx.state == AppState::BUILDER_PRIORITY) {
                count = 4;
                items[0]="EXPERIENCE"; items[1]="PRACTICAL"; items[2]="PEOPLE"; items[3]="SAFETY";
                itemsCN[0]="体验"; itemsCN[1]="实用"; itemsCN[2]="人际"; itemsCN[3]="安全";
            }

            if (ctx.state == AppState::BUILDER_INTENSITY) {
                for (int i = 0; i < count; ++i) {
                    if (isCN) {
                        canvas.setFont(&fonts::efontCN_12);
                        canvas.setTextSize(1);
                        canvas.setCursor(6 + i * 76, 50);
                    } else {
                        canvas.setFont(&fonts::Font0);
                        canvas.setTextSize(2);
                        canvas.setCursor(6 + i * 76, 50);
                    }
                    const char* txt = isCN ? itemsCN[i] : items[i];
                    if (i == ctx.selectedMenuIndex) {
                        canvas.setTextColor(CYAN, BLACK);
                        canvas.printf(">%s", txt);
                    } else {
                        canvas.setTextColor(WHITE, BLACK);
                        canvas.printf(" %s", txt);
                    }
                }
            } else {
                for (int i = 0; i < count; ++i) {
                    int col = i % 2;
                    int row = i / 2;
                    int x = (col == 0) ? 6 : 124;
                    int y = 30 + row * 28;

                    if (isCN) {
                        canvas.setFont(&fonts::efontCN_12);
                        canvas.setTextSize(1);
                    } else {
                        canvas.setFont(&fonts::Font0);
                        canvas.setTextSize(2);
                    }

                    canvas.setCursor(x, y);
                    const char* txt = isCN ? itemsCN[i] : items[i];
                    if (i == ctx.selectedMenuIndex) {
                        canvas.setTextColor(CYAN, BLACK);
                        canvas.printf("> %s %s", icons[i % 6], txt);
                    } else {
                        canvas.setTextColor(WHITE, BLACK);
                        canvas.printf("  %s %s", icons[i % 6], txt);
                    }
                }
            }

            drawFooter(isCN ? "[方向] 2D选择 [ENTER] 确定 [ESC] 返回" : "[ARR] 2D Move [ENTER] Select [ESC] Back", isCN);
            break;
        }

        case AppState::BUILDER_PREVIEW: {
            // 【Phase 4 UX 重构原则：Decide First, Discover Later】
            // 绝不展示任何 MBTI 预判、分歧或预测。纯粹让用户凭直觉看题并做决定！
            drawHeader(isCN ? "决策场景" : "SCENARIO", isCN);
            
            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            canvas.setTextSize(1);
            canvas.setTextColor(YELLOW, BLACK);
            canvas.setCursor(6, 28);
            const char* titleTxt = isCN ? (ctx.currentScenarioTitleCN[0] ? ctx.currentScenarioTitleCN : "离线情境")
                                        : (ctx.currentScenarioTitle[0] ? ctx.currentScenarioTitle : "SCENARIO");
            canvas.print(titleTxt);

            canvas.setTextColor(WHITE, BLACK);
            canvas.setCursor(6, 44);
            // 自动折行打印场景描述
            const char* desc = isCN ? (ctx.currentScenarioDescCN[0] ? ctx.currentScenarioDescCN : "")
                                    : (ctx.currentScenarioDesc[0] ? ctx.currentScenarioDesc : "");
            int lineY = 44;
            while (*desc && lineY <= 96) {
                char buf[24] = {0};
                int takeBytes = isCN ? 18 : 18;
                strncpy(buf, desc, takeBytes);
                canvas.setCursor(6, lineY);
                canvas.print(buf);
                desc += strlen(buf);
                lineY += 14;
            }

            // 右侧抽象预览雷达图
            RadarData prevData = {
                ctx.currentScenario.risk,
                ctx.currentScenario.novelty,
                ctx.currentScenario.practicalValue,
                ctx.currentScenario.social,
                100.0f - ctx.currentScenario.uncertainty,
                ctx.currentScenario.practicalValue
            };
            drawRadarChart(canvas, 182, 65, 30, prevData, CYAN, DARKCYAN, false, isCN);

            drawFooter(isCN ? "[ENTER] 做出选择  [ESC] 首页" : "[ENTER] MAKE DECISION  [ESC] HOME", isCN);
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
