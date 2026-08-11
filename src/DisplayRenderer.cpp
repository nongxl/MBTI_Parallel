#include "DisplayRenderer.h"
#include "RadarChart.h"
#include <cstdio>

#ifdef ARDUINO
#include <M5Cardputer.h>

static M5Canvas canvas(&M5Cardputer.Display);
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
static void drawHeader(const char* title) {
    canvas.fillScreen(BLACK);
    canvas.setTextColor(YELLOW, BLACK);
    canvas.setTextSize(2);
    canvas.setCursor(5, 4);
    canvas.print(title);
    canvas.drawLine(0, 23, 240, 23, GREEN);
}

static void drawFooter(const char* hint) {
    canvas.setTextColor(LIGHTGREY, BLACK);
    canvas.setTextSize(1);
    canvas.setCursor(5, 123);
    canvas.print(hint);
}
#endif

void renderUI(const UIContext& ctx) {
#ifdef ARDUINO
    switch (ctx.state) {
        case AppState::HOME: {
            canvas.fillScreen(BLACK);
            
            // 绘制中央科技感六边形图案
            RadarData homeData = { 75.0f, 90.0f, 60.0f, 85.0f, 40.0f, 70.0f };
            drawRadarChart(canvas, 120, 52, 30, homeData, GREEN, DARKCYAN, false);

            canvas.setTextColor(YELLOW, BLACK);
            canvas.setTextSize(2);
            canvas.setCursor(72, 92);
            canvas.print("PARALLEL");

            canvas.setTextColor(CYAN, BLACK);
            canvas.setTextSize(1);
            canvas.setCursor(62, 114);
            canvas.print("PRESS ENTER TO START");
            break;
        }

        case AppState::BUILDER_TYPE:
        case AppState::BUILDER_MOTIVATION:
        case AppState::BUILDER_CONCERN:
        case AppState::BUILDER_INTENSITY:
        case AppState::BUILDER_PRIORITY: {
            const char* title = "STEP";
            if (ctx.state == AppState::BUILDER_TYPE) title = "1. DECISION TYPE";
            else if (ctx.state == AppState::BUILDER_MOTIVATION) title = "2. MOTIVATION";
            else if (ctx.state == AppState::BUILDER_CONCERN) title = "3. MAIN CONCERN";
            else if (ctx.state == AppState::BUILDER_INTENSITY) title = "4. INTENSITY";
            else if (ctx.state == AppState::BUILDER_PRIORITY) title = "5. PRIORITY";

            drawHeader(title);

            // 显示 4~6 个带几何图标符号的科技感选项列表
            int count = 0;
            const char* items[8];
            const char* icons[] = { "◇", "○", "△", "□", "☆", "⬡" };

            if (ctx.state == AppState::BUILDER_TYPE) {
                count = 6;
                items[0]="GET"; items[1]="GO"; items[2]="DO"; items[3]="SAY"; items[4]="CHOOSE"; items[5]="CHANGE";
            } else if (ctx.state == AppState::BUILDER_MOTIVATION) {
                count = 5;
                items[0]="WANT"; items[1]="NEED"; items[2]="CURIOUS"; items[3]="FUN"; items[4]="OPPORTUNITY";
            } else if (ctx.state == AppState::BUILDER_CONCERN) {
                count = 6;
                items[0]="RISK"; items[1]="COST"; items[2]="TIME"; items[3]="EFFORT"; items[4]="UNKNOWN"; items[5]="NONE";
            } else if (ctx.state == AppState::BUILDER_INTENSITY) {
                count = 3;
                items[0]="LOW"; items[1]="MEDIUM"; items[2]="HIGH";
            } else if (ctx.state == AppState::BUILDER_PRIORITY) {
                count = 4;
                items[0]="EXPERIENCE"; items[1]="PRACTICAL"; items[2]="PEOPLE"; items[3]="SAFETY";
            }

            int visibleStart = 0;
            if (ctx.selectedMenuIndex >= 3) {
                visibleStart = ctx.selectedMenuIndex - 2;
            }

            for (int i = visibleStart; i < count && i < visibleStart + 3; ++i) {
                canvas.setTextSize(2);
                canvas.setCursor(10, 32 + (i - visibleStart) * 28);
                if (i == ctx.selectedMenuIndex) {
                    canvas.setTextColor(CYAN, BLACK);
                    canvas.printf("> %s %s", icons[i % 6], items[i]);
                } else {
                    canvas.setTextColor(WHITE, BLACK);
                    canvas.printf("  %s %s", icons[i % 6], items[i]);
                }
            }
            drawFooter("[UP/DN] Move [ENTER] Select [ESC] Back");
            break;
        }

        case AppState::BUILDER_PREVIEW: {
            drawHeader("SCENARIO PREVIEW");
            canvas.setTextSize(1);
            canvas.setTextColor(WHITE, BLACK);
            canvas.setCursor(10, 32);
            canvas.printf("Type    : %s\n", getDecisionTypeName(ctx.currentSelection.decisionType));
            canvas.setCursor(10, 48);
            canvas.printf("Motiv   : %s\n", getMotivationName(ctx.currentSelection.motivation));
            canvas.setCursor(10, 64);
            canvas.printf("Concern : %s (%s)\n", getConcernName(ctx.currentSelection.concern), getIntensityName(ctx.currentSelection.intensity));
            canvas.setCursor(10, 80);
            canvas.printf("Priority: %s\n", getPriorityName(ctx.currentSelection.priority));

            // 右边画一个预览抽象雷达轮廓
            RadarData prevData = {
                ctx.currentScenario.risk,
                ctx.currentScenario.novelty,
                ctx.currentScenario.practicalValue,
                ctx.currentScenario.social,
                100.0f - ctx.currentScenario.uncertainty,
                ctx.currentScenario.practicalValue
            };
            drawRadarChart(canvas, 185, 65, 26, prevData, CYAN, DARKCYAN, false);

            drawFooter("[ENTER] SIMULATE  [ESC] BACK");
            break;
        }

        case AppState::SIMULATING: {
            drawHeader("SIMULATING...");
            canvas.setTextSize(2);
            canvas.setTextColor(GREEN, BLACK);
            canvas.setCursor(20, 36);
            canvas.print("16 BRANCHES");

            // 绘制辐射线框与进度点阵动画
            int barWidth = (int)(ctx.animProgress * 1.9f);
            canvas.drawRect(20, 72, 200, 16, GREEN);
            canvas.fillRect(22, 74, barWidth > 196 ? 196 : barWidth, 12, CYAN);

            canvas.setTextSize(1);
            canvas.setTextColor(YELLOW, BLACK);
            canvas.setCursor(65, 96);
            canvas.printf("ANALYZING %d%%...", ctx.animProgress);
            break;
        }

        case AppState::SUMMARY: {
            drawHeader("SIMULATION RESULT");
            canvas.setTextSize(2);
            
            canvas.setTextColor(GREEN, BLACK);
            canvas.setCursor(10, 35);
            canvas.printf("YES   : %2d", ctx.summary.yesCount);

            canvas.setTextColor(RED, BLACK);
            canvas.setCursor(10, 60);
            canvas.printf("NO    : %2d", ctx.summary.noCount);

            canvas.setTextColor(YELLOW, BLACK);
            canvas.setCursor(10, 85);
            canvas.printf("MAYBE : %2d", ctx.summary.maybeCount);

            // 右侧示意柱状占比图
            canvas.fillRect(150, 38, ctx.summary.yesCount * 6, 12, GREEN);
            canvas.fillRect(150, 63, ctx.summary.noCount * 6, 12, RED);
            canvas.fillRect(150, 88, ctx.summary.maybeCount * 6, 12, YELLOW);

            drawFooter("[ENTER] BIGGEST SPLIT >");
            break;
        }

        case AppState::BIGGEST_SPLIT: {
            drawHeader("BIGGEST SPLIT");
            
            // 左边：YES 侧 MBTI 雷达图
            const PersonalityProfile& yesProf = getMBTIProfile(ctx.splitYesType);
            RadarData yesData = { yesProf.risk, yesProf.novelty, yesProf.logic, yesProf.social, yesProf.planning, yesProf.practicality };
            drawRadarChart(canvas, 55, 66, 25, yesData, GREEN, DARKCYAN, false);

            canvas.setTextSize(2);
            canvas.setTextColor(GREEN, BLACK);
            canvas.setCursor(15, 102);
            canvas.printf("%s YES", getMBTIName(ctx.splitYesType));

            // 中间 VS
            canvas.setTextColor(YELLOW, BLACK);
            canvas.setCursor(110, 60);
            canvas.print("VS");

            // 右边：NO 侧 MBTI 雷达图
            const PersonalityProfile& noProf = getMBTIProfile(ctx.splitNoType);
            RadarData noData = { noProf.risk, noProf.novelty, noProf.logic, noProf.social, noProf.planning, noProf.practicality };
            drawRadarChart(canvas, 185, 66, 25, noData, RED, DARKCYAN, false);

            canvas.setTextColor(RED, BLACK);
            canvas.setCursor(150, 102);
            canvas.printf("%s NO", getMBTIName(ctx.splitNoType));

            drawFooter("[ENTER] EXPLORE DETAILS >");
            break;
        }

        case AppState::EXPLORE: {
            const DecisionResult& res = ctx.results[ctx.exploreIndex];
            char headerTitle[32];
            snprintf(headerTitle, sizeof(headerTitle), "< %s > (%d/16)", getMBTIName(res.personality), ctx.exploreIndex + 1);
            drawHeader(headerTitle);

            // 右侧(x=175, y=65): 实时画出该 MBTI 专属的六维极坐标雷达图形状！
            const PersonalityProfile& pProf = getMBTIProfile(res.personality);
            RadarData mbtiData = { pProf.risk, pProf.novelty, pProf.logic, pProf.social, pProf.planning, pProf.practicality };
            drawRadarChart(canvas, 175, 66, 28, mbtiData, CYAN, DARKCYAN, true);

            // 左侧信息
            canvas.setTextSize(2);
            if (res.decision == Decision::YES) {
                canvas.setTextColor(GREEN, BLACK);
            } else if (res.decision == Decision::NO) {
                canvas.setTextColor(RED, BLACK);
            } else {
                canvas.setTextColor(YELLOW, BLACK);
            }
            canvas.setCursor(10, 32);
            canvas.printf("Choice: %s", getDecisionName(res.decision));

            canvas.setTextSize(1);
            canvas.setTextColor(WHITE, BLACK);
            canvas.setCursor(10, 56);
            canvas.printf("Score: %.1f", res.score);

            canvas.setCursor(10, 72);
            canvas.setTextColor(CYAN, BLACK);
            canvas.print("Reason:");
            canvas.setCursor(10, 86);
            canvas.setTextColor(LIGHTGREY, BLACK);
            canvas.print(res.reason);

            drawFooter("[L/R] Switch  [ENTER] Your Choice");
            break;
        }

        case AppState::YOUR_CHOICE: {
            drawHeader("WHAT WOULD YOU DO?");
            const char* choices[] = { "YES", "NO", "MAYBE" };
            for (int i = 0; i < 3; ++i) {
                canvas.setTextSize(2);
                canvas.setCursor(10, 38 + i * 26);
                if (i == ctx.selectedMenuIndex) {
                    canvas.setTextColor(GREEN, BLACK);
                    canvas.print("> ");
                } else {
                    canvas.setTextColor(WHITE, BLACK);
                    canvas.print("  ");
                }
                canvas.print(choices[i]);
            }
            drawFooter("[UP/DN] Move  [ENTER] Confirm");
            break;
        }

        case AppState::YOUR_MATCH: {
            drawHeader("YOUR DECISION PROFILE");
            
            // 左边(x=60, y=66): 绘制用户在此次选择下的决策轮廓雷达图！
            RadarData uData = {
                ctx.userProfile.risk,
                ctx.userProfile.novelty,
                ctx.userProfile.logic,
                ctx.userProfile.social,
                ctx.userProfile.planning,
                ctx.userProfile.practicality
            };
            drawRadarChart(canvas, 60, 66, 28, uData, GREEN, DARKGREEN, true);

            // 右边(x=125): 显示最具相近度的 MBTI 结果！
            canvas.setTextSize(1);
            canvas.setTextColor(WHITE, BLACK);
            canvas.setCursor(125, 36);
            canvas.print("CLOSEST MATCH:");

            canvas.setTextSize(2);
            canvas.setTextColor(YELLOW, BLACK);
            canvas.setCursor(125, 52);
            canvas.printf("%s", getMBTIName(ctx.closestMBTI));

            canvas.setTextSize(1);
            canvas.setTextColor(CYAN, BLACK);
            canvas.setCursor(125, 75);
            canvas.printf("Similarity: %.1f%%", ctx.matchSimilarity);

            canvas.setTextColor(LIGHTGREY, BLACK);
            canvas.setCursor(125, 92);
            canvas.printf("Choice: %s", getDecisionName(ctx.userChoice));

            drawFooter("[ENTER] BACK TO HOME");
            break;
        }
    }

    // 离屏画布单次推送，彻底消除闪烁
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
