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

// 安全 UTF-8 整字对齐断行函数
static int getSafeUTF8Break(const char* str, int maxBytes) {
    int len = (int)strlen(str);
    if (len <= maxBytes) return len;

    int pos = 0;
    while (pos < maxBytes) {
        unsigned char c = (unsigned char)str[pos];
        int charBytes = 1;
        if ((c & 0x80) == 0) charBytes = 1;
        else if ((c & 0xE0) == 0xC0) charBytes = 2;
        else if ((c & 0xF0) == 0xE0) charBytes = 3;
        else if ((c & 0xF8) == 0xF0) charBytes = 4;

        if (pos + charBytes > maxBytes) break;
        pos += charBytes;
    }
    return (pos > 0) ? pos : maxBytes;
}

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
                canvas.setCursor(30, 52);
                canvas.print("> 1. 中文 (Simplified Chinese)");
                canvas.setTextColor(WHITE, BLACK);
                canvas.setCursor(30, 85);
                canvas.print("  2. English (US)");
            } else {
                canvas.setTextColor(WHITE, BLACK);
                canvas.setCursor(30, 52);
                canvas.print("  1. 中文 (Simplified Chinese)");
                canvas.setTextColor(CYAN, BLACK);
                canvas.setCursor(30, 85);
                canvas.print("> 2. English (US)");
            }
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

            // 图层 0 (最底层 Layer 0): 亮黄色 PARALLEL 大字
            canvas.setFont(&fonts::Font0);
            canvas.setTextColor(YELLOW, BLACK);
            canvas.setTextSize(3);
            canvas.setCursor(48, 38);
            canvas.print("PARALLEL");

            // 图层 1 & 2: 48px 巨幅雷达 Logo (中心点 120, 55, 统一亮绿色连线 GREEN)
            drawRadarChart(canvas, 120, 55, 48, homeData, GREEN, DARKCYAN, false, isCN);

            // 图层 3: 开机三选项贴底排版
            if (isCN) {
                canvas.setFont(&fonts::efontCN_12);
                canvas.setTextSize(1);

                int yPos = 118;
                if (ctx.bootMenuMode == 0) {
                    canvas.setTextColor(CYAN, BLACK);
                    canvas.setCursor(6, yPos);
                    canvas.print("> [1. 随机]");
                    canvas.setTextColor(0x7BEF, BLACK);
                    canvas.setCursor(76, yPos);
                    canvas.print("  [2. 自定义]");
                    canvas.setCursor(162, yPos);
                    canvas.print("  [3. 画像]");
                } else if (ctx.bootMenuMode == 1) {
                    canvas.setTextColor(0x7BEF, BLACK);
                    canvas.setCursor(6, yPos);
                    canvas.print("  [1. 随机]");
                    canvas.setTextColor(CYAN, BLACK);
                    canvas.setCursor(76, yPos);
                    canvas.print("> [2. 自定义]");
                    canvas.setTextColor(0x7BEF, BLACK);
                    canvas.setCursor(162, yPos);
                    canvas.print("  [3. 画像]");
                } else {
                    canvas.setTextColor(0x7BEF, BLACK);
                    canvas.setCursor(6, yPos);
                    canvas.print("  [1. 随机]");
                    canvas.setCursor(76, yPos);
                    canvas.print("  [2. 自定义]");
                    canvas.setTextColor(CYAN, BLACK);
                    canvas.setCursor(162, yPos);
                    canvas.print("> [3. 画像]");
                }
            } else {
                canvas.setFont(&fonts::Font0);
                canvas.setTextSize(1);

                int yPos = 118;
                if (ctx.bootMenuMode == 0) {
                    canvas.setTextColor(CYAN, BLACK);
                    canvas.setCursor(6, yPos);
                    canvas.print(">[1. RANDOM]");
                    canvas.setTextColor(0x7BEF, BLACK);
                    canvas.setCursor(76, yPos);
                    canvas.print(" [2. CREATE]");
                    canvas.setCursor(162, yPos);
                    canvas.print(" [3. PROFILE]");
                } else if (ctx.bootMenuMode == 1) {
                    canvas.setTextColor(0x7BEF, BLACK);
                    canvas.setCursor(6, yPos);
                    canvas.print(" [1. RANDOM]");
                    canvas.setTextColor(CYAN, BLACK);
                    canvas.setCursor(76, yPos);
                    canvas.print(">[2. CREATE]");
                    canvas.setTextColor(0x7BEF, BLACK);
                    canvas.setCursor(162, yPos);
                    canvas.print(" [3. PROFILE]");
                } else {
                    canvas.setTextColor(0x7BEF, BLACK);
                    canvas.setCursor(6, yPos);
                    canvas.print(" [1. RANDOM]");
                    canvas.setCursor(76, yPos);
                    canvas.print(" [2. CREATE]");
                    canvas.setTextColor(CYAN, BLACK);
                    canvas.setCursor(162, yPos);
                    canvas.print(">[3. PROFILE]");
                }
            }

            if (ctx.totalPlays > 0) {
                canvas.setFont(&fonts::Font0);
                canvas.setTextColor(0x7BEF, BLACK);
                canvas.setCursor(185, 5);
                canvas.printf("PLS:%d", ctx.totalPlays);
            }
            break;
        }

        case AppState::MY_PROFILE: {
            drawHeader(isCN ? "真实长效 MBTI 画板" : "MY LONG-TERM PROFILE", isCN);

            // 雷达图放大跃升至 40px
            drawRadarChart(canvas, 180, 68, 40, ctx.currentRadar, GREEN, DARKCYAN, true, isCN);

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
            canvas.setCursor(6, 82);
            canvas.printf(isCN ? "已测场数: %d 场" : "Tested : %d", ctx.userHistory.totalPlays);

            canvas.setTextColor(0x7BEF, BLACK);
            canvas.setCursor(6, 100);
            canvas.printf(isCN ? "同意%d 拒绝%d 犹豫%d" : "Y:%d N:%d M:%d",
                          ctx.userHistory.yesCount, ctx.userHistory.noCount, ctx.userHistory.maybeCount);
            break;
        }

        case AppState::MY_PROFILE_CLEAR_CONFIRM: {
            drawHeader(isCN ? "⚠️ 清空历史确认" : "⚠️ CLEAR HISTORY", isCN);

            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            canvas.setTextSize(1);
            canvas.setTextColor(RED, BLACK);
            canvas.setCursor(10, 36);
            canvas.print(isCN ? "确定擦除所有决策历史与真实人格吗？" : "Clear all decision history & profile?");

            if (ctx.selectedMenuIndex == 0) {
                canvas.setTextColor(RED, BLACK);
                canvas.setCursor(20, 66);
                canvas.print(isCN ? "> [ 1. ⚠️ 清空历史 (CLEAR) ]" : "> [ 1. CLEAR ALL HISTORY ]");
                canvas.setTextColor(WHITE, BLACK);
                canvas.setCursor(20, 96);
                canvas.print(isCN ? "  [ 2. 🛡️ 取消保留 (CANCEL) ]" : "  [ 2. CANCEL ]");
            } else {
                canvas.setTextColor(WHITE, BLACK);
                canvas.setCursor(20, 66);
                canvas.print(isCN ? "  [ 1. ⚠️ 清空历史 (CLEAR) ]" : "  [ 1. CLEAR ALL HISTORY ]");
                canvas.setTextColor(GREEN, BLACK);
                canvas.setCursor(20, 96);
                canvas.print(isCN ? "> [ 2. 🛡️ 取消保留 (CANCEL) ]" : "> [ 2. CANCEL ]");
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
                int y = 34 + row * 32;

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
            while (*desc && lineY <= 120) {
                char buf[64] = {0};
                int takeBytes = getSafeUTF8Break(desc, isCN ? 54 : 37);
                strncpy(buf, desc, takeBytes);
                buf[takeBytes] = '\0';
                canvas.setCursor(8, lineY);
                canvas.print(buf);
                desc += takeBytes;
                lineY += 15;
            }
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

                canvas.setCursor(10, 36 + i * 30);
                const char* txt = isCN ? choicesCN[i] : choices[i];
                if (i == ctx.selectedMenuIndex) {
                    canvas.setTextColor(GREEN, BLACK);
                    canvas.printf("> %s", txt);
                } else {
                    canvas.setTextColor(WHITE, BLACK);
                    canvas.printf("  %s", txt);
                }
            }
            break;
        }

        case AppState::YOUR_MATCH: {
            drawHeader(isCN ? "你的决策轮廓" : "YOUR DECISION PROFILE", isCN);
            
            // 雷达图放大跃升至 40px
            drawRadarChart(canvas, 180, 68, 40, ctx.currentRadar, GREEN, DARKGREEN, true, isCN);

            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            canvas.setTextSize(1);
            canvas.setTextColor(WHITE, BLACK);
            canvas.setCursor(6, 28);
            canvas.print(isCN ? "最像人格:" : "MOST LIKE:");

            canvas.setFont(&fonts::Font0);
            canvas.setTextSize(1);
            canvas.setTextColor(YELLOW, BLACK);
            canvas.setCursor(6, 42);
            canvas.printf("%s (%.1f%%)", getMBTIName(ctx.closestMBTI), ctx.matchSimilarity);

            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            canvas.setTextSize(1);
            canvas.setTextColor(0x7BEF, BLACK);
            canvas.setCursor(6, 62);
            canvas.print(isCN ? "最大分歧:" : "BIGGEST DIFF:");

            canvas.setFont(&fonts::Font0);
            canvas.setTextSize(1);
            canvas.setTextColor(RED, BLACK);
            canvas.setCursor(6, 76);
            const char* diffDecTxt = isCN ? getDecisionNameCN(ctx.biggestDiffDecision) : getDecisionName(ctx.biggestDiffDecision);
            canvas.printf("%s (%s)", getMBTIName(ctx.biggestDiffMBTI), diffDecTxt);

            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            canvas.setTextSize(1);
            canvas.setTextColor(CYAN, BLACK);
            canvas.setCursor(6, 98);
            const char* myDecTxt = isCN ? getDecisionNameCN(ctx.userChoice) : getDecisionName(ctx.userChoice);
            canvas.printf(isCN ? "你的选择: %s" : "YOURS: %s", myDecTxt);

            if (ctx.totalPlays > 0) {
                canvas.setFont(&fonts::Font0);
                canvas.setTextColor(0x7BEF, BLACK);
                canvas.setCursor(185, 5);
                canvas.printf("PLS:%d", ctx.totalPlays);
            }
            break;
        }

        case AppState::SUMMARY: {
            drawHeader(isCN ? "16 人格模拟分支汇总" : "PARALLEL BRANCH SUMMARY", isCN);

            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            float yesPct = (ctx.summary.yesCount * 100.0f) / 16.0f;
            float noPct = (ctx.summary.noCount * 100.0f) / 16.0f;
            float maybePct = (ctx.summary.maybeCount * 100.0f) / 16.0f;

            // 1. 同意 (YES) 柱状统计 (Y = 34)
            canvas.setCursor(6, 34);
            canvas.setTextColor(GREEN, BLACK);
            canvas.printf(isCN ? "同意 %d人" : "YES %d", ctx.summary.yesCount);
            int yesBarW = static_cast<int>(120.0f * (yesPct / 100.0f));
            canvas.fillRect(82, 34, 120, 12, DARKGREY);
            if (yesBarW > 0) canvas.fillRect(82, 34, yesBarW, 12, GREEN);
            canvas.setCursor(206, 34);
            canvas.printf("%.0f%%", yesPct);

            // 2. 拒绝 (NO) 柱状统计 (Y = 60)
            canvas.setCursor(6, 60);
            canvas.setTextColor(RED, BLACK);
            canvas.printf(isCN ? "拒绝 %d人" : "NO  %d", ctx.summary.noCount);
            int noBarW = static_cast<int>(120.0f * (noPct / 100.0f));
            canvas.fillRect(82, 60, 120, 12, DARKGREY);
            if (noBarW > 0) canvas.fillRect(82, 60, noBarW, 12, RED);
            canvas.setCursor(206, 60);
            canvas.printf("%.0f%%", noPct);

            // 3. 犹豫 (MAYBE) 柱状统计 (Y = 86)
            canvas.setCursor(6, 86);
            canvas.setTextColor(YELLOW, BLACK);
            canvas.printf(isCN ? "犹豫 %d人" : "MAY %d", ctx.summary.maybeCount);
            int maybeBarW = static_cast<int>(120.0f * (maybePct / 100.0f));
            canvas.fillRect(82, 86, 120, 12, DARKGREY);
            if (maybeBarW > 0) canvas.fillRect(82, 86, maybeBarW, 12, YELLOW);
            canvas.setCursor(206, 86);
            canvas.printf("%.0f%%", maybePct);

            // 4. 阵营分布结论概览 (Y = 110)
            canvas.setCursor(6, 110);
            canvas.setTextColor(CYAN, BLACK);
            if (ctx.summary.yesCount >= 10) {
                canvas.print(isCN ? "主流阵营: 绝对压倒性同意" : "DOMINANT: OVERWHELMING YES");
            } else if (ctx.summary.noCount >= 10) {
                canvas.print(isCN ? "主流阵营: 绝对压倒性拒绝" : "DOMINANT: OVERWHELMING NO");
            } else if (ctx.summary.yesCount >= 6 && ctx.summary.noCount >= 6) {
                canvas.print(isCN ? "阵营格局: 剧烈意见撕裂分歧" : "STATUS: INTENSELY SPLIT");
            } else {
                canvas.print(isCN ? "阵营格局: 观点分布较为多元" : "STATUS: DIVERSED VIEWS");
            }
            break;
        }

        case AppState::EXPLORE: {
            const DecisionResult& res = ctx.results[ctx.exploreIndex];
            char headerTitle[32];
            snprintf(headerTitle, sizeof(headerTitle), "< %s > (%d/16)", getMBTIName(res.personality), ctx.exploreIndex + 1);
            drawHeader(headerTitle, isCN);

            // 【雷达图放大跃升至 40px】
            drawRadarChart(canvas, 180, 68, 40, ctx.currentRadar, GREEN, DARKCYAN, true, isCN);

            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            // 【极客 15px 统一等距行高节奏】
            // 行 1: 选择 (Y = 26)
            canvas.setTextSize(1);
            canvas.setTextColor(WHITE, BLACK);
            canvas.setCursor(6, 26);
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

            // 行 2: 得分 (Y = 41, 差 15px)
            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            canvas.setTextSize(1);
            canvas.setTextColor(WHITE, BLACK);
            canvas.setCursor(6, 41);
            canvas.printf(isCN ? "得分: %.1f" : "Score : %.1f", res.score);

            // 行 3: 依据 (Y = 56, 差 15px)
            canvas.setCursor(6, 56);
            canvas.setTextColor(CYAN, BLACK);
            canvas.print(isCN ? "依据:" : "Reason:");

            // 行 4 ~ 7: 依据正文 (Y = 71, 86, 101, 116，统一 15px 行高，改为 WHITE 纯白高亮)
            const char* pReason = isCN ? getDecisionReasonCN(res.reason) : res.reason;
            int lineY = 71;
            while (*pReason && lineY <= 116) {
                char lineBuf[28] = {0};
                int maxB = isCN ? 21 : 16;
                int takeBytes = getSafeUTF8Break(pReason, maxB);
                strncpy(lineBuf, pReason, takeBytes);
                lineBuf[takeBytes] = '\0';
                pReason += takeBytes;

                canvas.setCursor(6, lineY);
                canvas.setTextColor(WHITE, BLACK); // 依据正文改为 WHITE 纯白
                canvas.print(lineBuf);
                lineY += 15; // 统一 15px 极客行高
            }
            break;
        }

        case AppState::SIMULATING:
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
