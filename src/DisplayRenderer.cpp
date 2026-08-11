#include "DisplayRenderer.h"
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
    canvas.setCursor(5, 5);
    canvas.print(title);
    canvas.drawLine(0, 25, 240, 25, GREEN);
}

static void drawFooter(const char* hint) {
    canvas.setTextColor(LIGHTGREY, BLACK);
    canvas.setTextSize(1);
    canvas.setCursor(5, 122);
    canvas.print(hint);
}
#endif

void renderUI(const UIContext& ctx) {
#ifdef ARDUINO
    switch (ctx.state) {
        case AppState::HOME: {
            drawHeader("PARALLEL MBTI");
            const char* options[] = { "1. DECIDE (Custom)", "2. RANDOM (Preset)" };
            for (int i = 0; i < 2; ++i) {
                canvas.setTextSize(2);
                canvas.setCursor(10, 40 + i * 30);
                if (i == ctx.selectedMenuIndex) {
                    canvas.setTextColor(GREEN, BLACK);
                    canvas.print("> ");
                } else {
                    canvas.setTextColor(WHITE, BLACK);
                    canvas.print("  ");
                }
                canvas.print(options[i]);
            }
            drawFooter("[UP/DOWN] Select  [ENTER] OK");
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

            // 显示 4~6 个选项列表
            int count = 0;
            const char* items[8];
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
                canvas.setCursor(10, 35 + (i - visibleStart) * 26);
                if (i == ctx.selectedMenuIndex) {
                    canvas.setTextColor(CYAN, BLACK);
                    canvas.print("> ");
                } else {
                    canvas.setTextColor(WHITE, BLACK);
                    canvas.print("  ");
                }
                canvas.print(items[i]);
            }
            drawFooter("[UP/DN] Move [ENTER] Select [ESC] Back");
            break;
        }

        case AppState::BUILDER_PREVIEW: {
            drawHeader("SCENARIO PREVIEW");
            canvas.setTextSize(1);
            canvas.setTextColor(WHITE, BLACK);
            canvas.setCursor(10, 35);
            canvas.printf("Type    : %s\n", getDecisionTypeName(ctx.currentSelection.decisionType));
            canvas.setCursor(10, 50);
            canvas.printf("Motiv   : %s\n", getMotivationName(ctx.currentSelection.motivation));
            canvas.setCursor(10, 65);
            canvas.printf("Concern : %s (%s)\n", getConcernName(ctx.currentSelection.concern), getIntensityName(ctx.currentSelection.intensity));
            canvas.setCursor(10, 80);
            canvas.printf("Priority: %s\n", getPriorityName(ctx.currentSelection.priority));

            drawFooter("[ENTER] SIMULATE  [ESC] BACK");
            break;
        }

        case AppState::SIMULATING: {
            drawHeader("SIMULATING...");
            canvas.setTextSize(2);
            canvas.setTextColor(GREEN, BLACK);
            canvas.setCursor(20, 45);
            canvas.print("16 BRANCHES");

            // 绘制进度条
            int barWidth = (int)(ctx.animProgress * 2.0f);
            canvas.drawRect(20, 80, 200, 16, GREEN);
            canvas.fillRect(22, 82, barWidth > 196 ? 196 : barWidth, 12, CYAN);
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

            drawFooter("[ENTER] BIGGEST SPLIT >");
            break;
        }

        case AppState::BIGGEST_SPLIT: {
            drawHeader("BIGGEST SPLIT");
            canvas.setTextSize(2);
            
            // YES Side
            canvas.setTextColor(GREEN, BLACK);
            canvas.setCursor(15, 38);
            canvas.printf("%s (YES)", getMBTIName(ctx.splitYesType));

            // VS
            canvas.setTextColor(YELLOW, BLACK);
            canvas.setCursor(105, 65);
            canvas.print("VS");

            // NO Side
            canvas.setTextColor(RED, BLACK);
            canvas.setCursor(15, 92);
            canvas.printf("%s (NO)", getMBTIName(ctx.splitNoType));

            drawFooter("[ENTER] EXPLORE DETAILS >");
            break;
        }

        case AppState::EXPLORE: {
            const DecisionResult& res = ctx.results[ctx.exploreIndex];
            char headerTitle[32];
            snprintf(headerTitle, sizeof(headerTitle), "< %s > (%d/16)", getMBTIName(res.personality), ctx.exploreIndex + 1);
            drawHeader(headerTitle);

            canvas.setTextSize(2);
            if (res.decision == Decision::YES) {
                canvas.setTextColor(GREEN, BLACK);
            } else if (res.decision == Decision::NO) {
                canvas.setTextColor(RED, BLACK);
            } else {
                canvas.setTextColor(YELLOW, BLACK);
            }
            canvas.setCursor(10, 35);
            canvas.printf("Choice: %s", getDecisionName(res.decision));

            canvas.setTextSize(1);
            canvas.setTextColor(WHITE, BLACK);
            canvas.setCursor(10, 60);
            canvas.printf("Score: %.1f", res.score);

            canvas.setCursor(10, 78);
            canvas.setTextColor(CYAN, BLACK);
            canvas.print("Reason:");
            canvas.setCursor(10, 92);
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
            drawHeader("YOUR MATCH RESULT");
            canvas.setTextSize(1);
            canvas.setTextColor(WHITE, BLACK);
            canvas.setCursor(10, 35);
            canvas.print("YOU MATCHED WITH:");

            canvas.setTextSize(2);
            canvas.setTextColor(GREEN, BLACK);
            canvas.setCursor(10, 58);

            int matchCount = 0;
            for (int i = 0; i < MBTI_COUNT; ++i) {
                if (ctx.results[i].decision == ctx.userChoice) {
                    canvas.printf("%s ", getMBTIName(ctx.results[i].personality));
                    matchCount++;
                    if (matchCount % 4 == 0) {
                        canvas.setCursor(10, 58 + (matchCount / 4) * 22);
                    }
                }
            }
            if (matchCount == 0) {
                canvas.setTextColor(YELLOW, BLACK);
                canvas.print("YOU ARE THE OUTLIER!");
            }

            drawFooter("[ENTER] BACK TO HOME");
            break;
        }
    }

    // 离屏画布单次传输推送，彻底消除屏幕刷新闪烁
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
