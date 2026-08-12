#include "DisplayRenderer.h"
#include "RadarChart.h"
#include "ScenarioBuilder.h"
#include "DecisionRecord.h"
#include "CategoryPatternEngine.h"
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

// 安全 UTF-8 整字对齐断行函数 (处理单行最长字节数，排除 \n 换行符)
static int getSafeUTF8Break(const char* str, int maxBytes) {
    int len = (int)strlen(str);
    int pos = 0;
    while (pos < maxBytes && pos < len) {
        if (str[pos] == '\n') {
            return pos; // 遇到显式换行符指针打断
        }
        unsigned char c = (unsigned char)str[pos];
        int charBytes = 1;
        if ((c & 0x80) == 0) charBytes = 1;
        else if ((c & 0xE0) == 0xC0) charBytes = 2;
        else if ((c & 0xF0) == 0xE0) charBytes = 3;
        else if ((c & 0xF8) == 0xF0) charBytes = 4;

        if (pos + charBytes > maxBytes) break;
        pos += charBytes;
    }
    return (pos > 0) ? pos : (len > 0 ? 1 : 0);
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

            // 【主波长雷达 (绿色分支)】
            RadarData homeData1 = {
                72.0f + 18.0f * sinf(t * 1.3f),
                82.0f + 14.0f * cosf(t * 0.9f + 1.1f),
                68.0f + 16.0f * sinf(t * 1.6f + 2.2f),
                78.0f + 15.0f * cosf(t * 1.1f + 3.3f),
                55.0f + 20.0f * sinf(t * 0.8f + 4.4f),
                74.0f + 16.0f * cosf(t * 1.4f + 5.5f)
            };

            // 【差频镜像波长雷达 (霓虹紫色分支)】
            RadarData homeData2 = {
                65.0f + 20.0f * cosf(t * 1.1f + 0.8f),
                75.0f + 16.0f * sinf(t * 1.4f + 2.5f),
                80.0f + 12.0f * cosf(t * 0.7f + 1.9f),
                60.0f + 18.0f * sinf(t * 1.5f + 3.7f),
                82.0f + 15.0f * cosf(t * 1.2f + 4.1f),
                62.0f + 19.0f * sinf(t * 0.9f + 5.0f)
            };

            // 图层 0 (最底层 Layer 0): 亮黄色 PARALLEL 大字
            canvas.setFont(&fonts::Font0);
            canvas.setTextColor(YELLOW, BLACK);
            canvas.setTextSize(3);
            canvas.setCursor(48, 38);
            canvas.print("PARALLEL");

            // 图层 1 & 2 & 3: 48px 巨幅双雷达呼吸交织 Logo (中心点 120, 55, 绿实线 vs 紫实线 + 双色 Alpha + 皇家蓝紫重叠加深)
            drawDualRadarChart(canvas, 120, 55, 48, homeData1, homeData2, nullptr, false, isCN);

            // 图层 4: 开机三选项贴底排版
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

            // 【图层 0 最底层】: Size 4 巨幅 MBTI 发光水印 (坐标 132, 52)
            canvas.setFont(&fonts::Font0);
            canvas.setTextColor(YELLOW, BLACK);
            canvas.setTextSize(4);
            canvas.setCursor(132, 52);
            canvas.print(getMBTIName(ctx.userHistory.dominantMBTI));

            // 【图层 1 & 2】: 半透明雷达图 Overlay
            drawRadarChart(canvas, 180, 68, 40, ctx.currentRadar, GREEN, DARKCYAN, true, isCN);

            // 【Phase 6 核心】: 实时计算 12 大生活领域跨情境模式
            ContextualPersonalitySummary patternSummary = computeContextualPatterns(getDecisionRecordStore());

            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            canvas.setTextSize(1);

            if (patternSummary.validPatternsCount > 0) {
                canvas.setTextColor(CYAN, BLACK);
                canvas.setCursor(6, 26);
                canvas.print(isCN ? "跨情境模式发现:" : "CONTEXTUAL PATTERNS:");

                int printed = 0;
                for (int cat = 0; cat < 12 && printed < 3; ++cat) {
                    const CategoryPattern& pat = patternSummary.patterns[cat];
                    if (pat.confidence == PatternConfidence::MEDIUM || pat.confidence == PatternConfidence::HIGH) {
                        int yPos = 44 + printed * 18;
                        const char* catName = isCN ? getCategoryNameCN(pat.category) : getCategoryNameEN(pat.category);

                        if (isCN) canvas.setFont(&fonts::efontCN_12);
                        else canvas.setFont(&fonts::Font0);

                        canvas.setTextColor(WHITE, BLACK);
                        canvas.setCursor(6, yPos);
                        canvas.printf("%.6s", catName);

                        canvas.setFont(&fonts::Font0);
                        canvas.setTextColor(YELLOW, BLACK);
                        canvas.setCursor(62, yPos + 1);
                        canvas.printf("%s", getMBTIName(pat.dominantMBTI));

                        if (isCN) canvas.setFont(&fonts::efontCN_12);
                        else canvas.setFont(&fonts::Font0);

                        canvas.setTextColor(pat.confidence == PatternConfidence::HIGH ? GREEN : 0x7BEF, BLACK);
                        canvas.setCursor(96, yPos);
                        canvas.printf("%s", isCN ? getConfidenceTextCN(pat.confidence) : getConfidenceTextEN(pat.confidence));

                        printed++;
                    }
                }

                canvas.setCursor(6, 114);
                canvas.setTextColor(0x7BEF, BLACK);
                canvas.print(isCN ? "情境决策呈现不同倾向" : "PATTERNS VARY BY CONTEXT");
            } else {
                canvas.setTextColor(WHITE, BLACK);
                canvas.setCursor(6, 28);
                canvas.print(isCN ? "收敛真实人格:" : "LONG-TERM MBTI:");

                canvas.setTextColor(CYAN, BLACK);
                canvas.setCursor(6, 48);
                canvas.printf(isCN ? "置信度: %.1f%%" : "Confid : %.1f%%", ctx.userHistory.dominantSimilarity);

                canvas.setTextColor(WHITE, BLACK);
                canvas.setCursor(6, 68);
                canvas.printf(isCN ? "已测场数: %d 场" : "Tested : %d", ctx.userHistory.totalPlays);

                canvas.setTextColor(0x7BEF, BLACK);
                canvas.setCursor(6, 88);
                canvas.printf(isCN ? "同意%d 拒绝%d 犹豫%d" : "Y:%d N:%d M:%d",
                              ctx.userHistory.yesCount, ctx.userHistory.noCount, ctx.userHistory.maybeCount);

                canvas.setCursor(6, 114);
                canvas.setTextColor(0x7BEF, BLACK);
                canvas.print(isCN ? "多玩几场可解锁情境模式" : "PLAY MORE TO UNLOCK PATTERNS");
            }
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
                int x = (col == 0) ? 8 : 124;
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
            const char* catHeaderCN = "场景预览 - 探索抉择";
            const char* catHeaderEN = "PREVIEW - DECISION";

            if (ctx.currentScenario.type == DecisionType::GET) {
                catHeaderCN = "场景预览 - 购物与理财";
                catHeaderEN = "PREVIEW - PURCHASE";
            } else if (ctx.currentScenario.type == DecisionType::DO) {
                catHeaderCN = "场景预览 - 职场与协作";
                catHeaderEN = "PREVIEW - WORK";
            } else if (ctx.currentScenario.type == DecisionType::GO) {
                catHeaderCN = "场景预览 - 旅行与探险";
                catHeaderEN = "PREVIEW - TRAVEL";
            }

            drawHeader(isCN ? catHeaderCN : catHeaderEN, isCN);
            
            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            canvas.setTextSize(1);
            canvas.setTextColor(WHITE, BLACK);
            
            const char* desc = isCN ? (ctx.currentScenarioDescCN[0] ? ctx.currentScenarioDescCN : "")
                                    : (ctx.currentScenarioDesc[0] ? ctx.currentScenarioDesc : "");
            
            int rawLineY = 26 - ctx.previewScrollY;
            const char* pDesc = desc;
            int maxLineBytes = isCN ? 57 : 36;
            bool hasMoreBelow = false;

            while (*pDesc) {
                if (*pDesc == '\n') {
                    pDesc++;
                    rawLineY += 15;
                    continue;
                }

                char buf[64] = {0};
                int takeBytes = getSafeUTF8Break(pDesc, maxLineBytes);
                if (takeBytes <= 0) break;

                strncpy(buf, pDesc, takeBytes);
                buf[takeBytes] = '\0';

                if (rawLineY >= 25 && rawLineY <= 130) {
                    canvas.setCursor(6, rawLineY);
                    canvas.print(buf);
                } else if (rawLineY > 130) {
                    hasMoreBelow = true;
                }

                pDesc += takeBytes;
                rawLineY += 15;
            }

            if (hasMoreBelow || ctx.previewScrollY > 0) {
                canvas.setFont(&fonts::Font0);
                canvas.setTextColor(CYAN, BLACK);
                canvas.setCursor(175, 5);
                canvas.print("[v SCROLL]");
            }
            break;
        }

        case AppState::YOUR_CHOICE: {
            drawHeader(isCN ? "如果是你，你怎么选？" : "WHAT WOULD YOU DO?", isCN);
            
            const char* choicesCN[] = { "1. 果断行动 (YES)", "2. 保持原状 (NO)", "3. 犹豫观望 (MAYBE)" };
            const char* choicesEN[] = { "1. YES", "2. NO", "3. MAYBE" };

            if (ctx.currentScenario.type == DecisionType::GET) {
                choicesCN[0] = "1. 果断购买 (BUY)";
                choicesCN[1] = "2. 理性克制 (PASS)";
                choicesEN[0] = "1. BUY NOW";
                choicesEN[1] = "2. PASS & SAVE";
            } else if (ctx.currentScenario.type == DecisionType::DO) {
                choicesCN[0] = "1. 留下来帮 (HELP)";
                choicesCN[1] = "2. 准时下班 (LEAVE)";
                choicesEN[0] = "1. STAY & HELP";
                choicesEN[1] = "2. LEAVE ON TIME";
            } else if (ctx.currentScenario.type == DecisionType::GO) {
                choicesCN[0] = "1. 果断前往 (GO)";
                choicesCN[1] = "2. 按原计划 (STAY)";
                choicesEN[0] = "1. ACCEPT & GO";
                choicesEN[1] = "2. STICK TO PLAN";
            }

            for (int i = 0; i < 3; ++i) {
                if (isCN) {
                    canvas.setFont(&fonts::efontCN_12);
                    canvas.setTextSize(1);
                } else {
                    canvas.setFont(&fonts::Font0);
                    canvas.setTextSize(2);
                }

                canvas.setCursor(10, 34 + i * 28);
                const char* txt = isCN ? choicesCN[i] : choicesEN[i];
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
            
            const PersonalityProfile& closestProf = getMBTIProfile(ctx.closestMBTI);
            RadarData mbtiRadar = { closestProf.novelty, closestProf.risk, closestProf.planning, closestProf.practicality, closestProf.logic, closestProf.social };

            canvas.setFont(&fonts::Font0);
            canvas.setTextColor(YELLOW, BLACK);
            canvas.setTextSize(4);
            canvas.setCursor(132, 52);
            canvas.print(getMBTIName(ctx.closestMBTI));

            drawDualRadarChart(canvas, 180, 68, 40, ctx.currentRadar, mbtiRadar, getMBTIName(ctx.closestMBTI), true, isCN);

            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            canvas.setTextSize(1);
            canvas.setTextColor(WHITE, BLACK);
            canvas.setCursor(6, 28);
            canvas.print(isCN ? "本次决定更像:" : "THIS LOOKS LIKE:");

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

            // 【彻底修复 Bug 2】: 在中文字体库下正确渲染括号内部的具体行为动词！绝对不留空白！
            canvas.setCursor(6, 76);
            canvas.setTextColor(RED, BLACK);
            const char* diffDecTxt = isCN ? getScenarioActionNameCN(ctx.biggestDiffDecision, ctx.currentScenario.type)
                                          : getScenarioActionNameEN(ctx.biggestDiffDecision, ctx.currentScenario.type);
            
            if (isCN) {
                canvas.setFont(&fonts::efontCN_12);
                canvas.printf("%s (%s)", getMBTIName(ctx.biggestDiffMBTI), diffDecTxt);
            } else {
                canvas.setFont(&fonts::Font0);
                canvas.printf("%s (%s)", getMBTIName(ctx.biggestDiffMBTI), diffDecTxt);
            }

            // 【彻底修复 Bug 1】: 你的选择同步咬合场景专属动词 (如“果断前往”、“果断购买”)！
            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            canvas.setTextSize(1);
            canvas.setTextColor(CYAN, BLACK);
            canvas.setCursor(6, 98);
            const char* myDecTxt = isCN ? getScenarioActionNameCN(ctx.userChoice, ctx.currentScenario.type)
                                        : getScenarioActionNameEN(ctx.userChoice, ctx.currentScenario.type);
            canvas.printf(isCN ? "你的选择: %s" : "YOURS: %s", myDecTxt);

            if (ctx.totalPlays > 0) {
                canvas.setFont(&fonts::Font0);
                canvas.setTextColor(0x7BEF, BLACK);
                canvas.setCursor(185, 5);
                canvas.printf("PLS:%d", ctx.totalPlays);
            }
            break;
        }

        case AppState::WHY_MATCH: {
            char titleBuf[48];
            snprintf(titleBuf, sizeof(titleBuf), isCN ? "为什么更接近 %s？" : "WHY LOOKS LIKE %s?", getMBTIName(ctx.closestMBTI));
            drawHeader(titleBuf, isCN);

            const PersonalityProfile& closestProf = getMBTIProfile(ctx.closestMBTI);
            RadarData mbtiRadar = { closestProf.novelty, closestProf.risk, closestProf.planning, closestProf.practicality, closestProf.logic, closestProf.social };

            canvas.setFont(&fonts::Font0);
            canvas.setTextColor(YELLOW, BLACK);
            canvas.setTextSize(4);
            canvas.setCursor(132, 52);
            canvas.print(getMBTIName(ctx.closestMBTI));

            drawDualRadarChart(canvas, 180, 68, 40, ctx.currentRadar, mbtiRadar, getMBTIName(ctx.closestMBTI), true, isCN);

            const char* dimNamesCN[6] = { "新鲜感", "风险度", "计划性", "实用性", "逻辑性", "社交性" };
            const char* dimNamesEN[6] = { "NOVELTY", "RISK", "PLANNING", "PRACTICAL", "LOGIC", "SOCIAL" };

            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            canvas.setTextSize(1);
            canvas.setTextColor(WHITE, BLACK);
            canvas.setCursor(6, 28);
            canvas.print(isCN ? "契合维度对比:" : "TOP MATCH AXES:");

            for (int k = 0; k < 3; ++k) {
                int dIdx = ctx.whyMatchDims[k];
                int yPos = 46 + k * 20;
                const char* dName = isCN ? dimNamesCN[dIdx] : dimNamesEN[dIdx];
                float uVal = ctx.whyMatchUserVals[k];
                float mVal = ctx.whyMatchMbtiVals[k];

                if (isCN) canvas.setFont(&fonts::efontCN_12);
                else canvas.setFont(&fonts::Font0);

                canvas.setCursor(6, yPos);
                canvas.setTextColor(CYAN, BLACK);
                canvas.printf("%d.%s", k + 1, dName);

                canvas.setFont(&fonts::Font0);
                canvas.setCursor(62, yPos + 1);
                canvas.setTextColor(GREEN, BLACK);
                canvas.printf("%.0f", uVal);

                canvas.setTextColor(WHITE, BLACK);
                canvas.print("/");

                canvas.setTextColor(MAGENTA, BLACK);
                canvas.printf("%.0f", mVal);
            }

            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            canvas.setTextColor(0x7BEF, BLACK);
            canvas.setCursor(6, 112);
            canvas.print(isCN ? "核心维度高度契合" : "ALIGNED CLOSELY");
            break;
        }

        case AppState::SUMMARY: {
            drawHeader(isCN ? "16 人格模拟分支汇总" : "PARALLEL BRANCH SUMMARY", isCN);

            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            float yesPct = (ctx.summary.yesCount * 100.0f) / 16.0f;
            float noPct = (ctx.summary.noCount * 100.0f) / 16.0f;
            float maybePct = (ctx.summary.maybeCount * 100.0f) / 16.0f;

            canvas.setCursor(6, 34);
            canvas.setTextColor(GREEN, BLACK);
            canvas.printf(isCN ? "同意 %d人" : "YES %d", ctx.summary.yesCount);
            int yesBarW = static_cast<int>(120.0f * (yesPct / 100.0f));
            canvas.fillRect(82, 34, 120, 12, DARKGREY);
            if (yesBarW > 0) canvas.fillRect(82, 34, yesBarW, 12, GREEN);
            canvas.setCursor(206, 34);
            canvas.printf("%.0f%%", yesPct);

            canvas.setCursor(6, 60);
            canvas.setTextColor(RED, BLACK);
            canvas.printf(isCN ? "拒绝 %d人" : "NO  %d", ctx.summary.noCount);
            int noBarW = static_cast<int>(120.0f * (noPct / 100.0f));
            canvas.fillRect(82, 60, 120, 12, DARKGREY);
            if (noBarW > 0) canvas.fillRect(82, 60, noBarW, 12, RED);
            canvas.setCursor(206, 60);
            canvas.printf("%.0f%%", noPct);

            canvas.setCursor(6, 86);
            canvas.setTextColor(YELLOW, BLACK);
            canvas.printf(isCN ? "犹豫 %d人" : "MAY %d", ctx.summary.maybeCount);
            int maybeBarW = static_cast<int>(120.0f * (maybePct / 100.0f));
            canvas.fillRect(82, 86, 120, 12, DARKGREY);
            if (maybeBarW > 0) canvas.fillRect(82, 86, maybeBarW, 12, YELLOW);
            canvas.setCursor(206, 86);
            canvas.printf("%.0f%%", maybePct);

            canvas.setCursor(6, 110);
            canvas.setTextColor(CYAN, BLACK);

            if (ctx.summary.yesCount >= 10) {
                canvas.print(isCN ? "主流阵营: 压倒性同意 (62%+)" : "DOMINANT: OVERWHELMING YES");
            } else if (ctx.summary.noCount >= 10) {
                canvas.print(isCN ? "主流阵营: 压倒性拒绝 (62%+)" : "DOMINANT: OVERWHELMING NO");
            } else if (ctx.summary.maybeCount >= 10) {
                canvas.print(isCN ? "主流阵营: 压倒性犹豫 (62%+)" : "DOMINANT: OVERWHELMING MAYBE");
            } else if (ctx.summary.yesCount >= 8) {
                canvas.print(isCN ? "主流阵营: 优势倾向同意" : "MAJORITY: TENDS TO YES");
            } else if (ctx.summary.noCount >= 8) {
                canvas.print(isCN ? "主流阵营: 优势倾向拒绝" : "MAJORITY: TENDS TO NO");
            } else if (ctx.summary.maybeCount >= 8) {
                canvas.print(isCN ? "主流阵营: 优势倾向犹豫" : "MAJORITY: TENDS TO MAYBE");
            } else if (ctx.summary.yesCount >= 5 && ctx.summary.noCount >= 5) {
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

            const PersonalityProfile& currentProf = getMBTIProfile(res.personality);
            RadarData currentMbtiRadar = { currentProf.novelty, currentProf.risk, currentProf.planning, currentProf.practicality, currentProf.logic, currentProf.social };

            canvas.setFont(&fonts::Font0);
            canvas.setTextColor(YELLOW, BLACK);
            canvas.setTextSize(4);
            canvas.setCursor(132, 52);
            canvas.print(getMBTIName(res.personality));

            drawDualRadarChart(canvas, 180, 68, 40, ctx.currentRadar, currentMbtiRadar, getMBTIName(res.personality), true, isCN);

            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            canvas.setTextSize(1);
            canvas.setTextColor(WHITE, BLACK);
            canvas.setCursor(6, 26);
            canvas.print(isCN ? "选择: " : "Choice: ");

            // 【动词咬合升级】: 在 EXPLORE 屏输出该 MBTI 人格针对本题的具体行为动词！
            const char* decName = isCN ? getScenarioActionNameCN(res.decision, ctx.currentScenario.type)
                                       : getScenarioActionNameEN(res.decision, ctx.currentScenario.type);

            if (isCN) canvas.setFont(&fonts::efontCN_12);
            else canvas.setFont(&fonts::Font0);

            canvas.setTextSize(1);
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
            canvas.setCursor(6, 41);
            canvas.printf(isCN ? "得分: %.1f" : "Score : %.1f", res.score);

            canvas.setCursor(6, 56);
            canvas.setTextColor(CYAN, BLACK);
            canvas.print(isCN ? "依据:" : "Reason:");

            const char* pReason = isCN ? getDecisionReasonCN(res.reason, res.personality, res.decision, ctx.currentScenario.type) : res.reason;
            int lineY = 71;
            while (*pReason && lineY <= 116) {
                char lineBuf[28] = {0};
                int maxB = isCN ? 21 : 16;
                int takeBytes = getSafeUTF8Break(pReason, maxB);
                strncpy(lineBuf, pReason, takeBytes);
                lineBuf[takeBytes] = '\0';
                pReason += takeBytes;

                canvas.setCursor(6, lineY);
                canvas.setTextColor(WHITE, BLACK);
                canvas.print(lineBuf);
                lineY += 15;
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
