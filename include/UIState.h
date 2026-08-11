#ifndef UI_STATE_H
#define UI_STATE_H

#include <cstdint>
#include "MBTIProfile.h"
#include "Scenario.h"
#include "DecisionEngine.h"
#include "ScenarioBuilder.h"
#include "DecisionProfile.h"
#include "RadarChart.h"
#include "ScenarioGenerator.h"

enum class AppState {
    HOME,
    BUILDER_WHO,        // Step 1: WHO
    BUILDER_SITUATION,  // Step 2: SITUATION
    BUILDER_CONDITION,  // Step 3: CONDITION
    BUILDER_TENSION,    // Step 4: TENSION
    BUILDER_PREVIEW,
    SIMULATING,
    SUMMARY,
    BIGGEST_SPLIT,
    EXPLORE,
    YOUR_CHOICE,
    YOUR_MATCH
};

enum class KeyInput {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    ENTER,
    BACK
};

enum class MatchType {
    MATCH,
    OUTLIER,
    MAJORITY,
    SPLIT
};

enum class Language {
    ENGLISH = 0,
    CHINESE = 1
};

struct UIContext {
    AppState state;
    Language lang; // 当前语言设置 (ENGLISH / CHINESE)

    // 自定义决策场景 4 步 DNA
    CustomScenarioDNA customDNA;

    UserSelection currentSelection;
    Scenario currentScenario;
    char currentScenarioTitle[32];
    char currentScenarioDesc[80];
    char currentScenarioTitleCN[32];
    char currentScenarioDescCN[80];

    DecisionResult results[MBTI_COUNT];
    DecisionSummary summary;
    MBTIType splitYesType;
    MBTIType splitNoType;

    int selectedMenuIndex;
    int exploreIndex;
    Decision userChoice;

    // 用户本题决策轮廓与匹配算法
    DecisionProfile userProfile;
    MBTIType closestMBTI;
    float matchSimilarity;
    MatchType matchType;

    // 本题分歧最大的人格与决策结果
    MBTIType biggestDiffMBTI;
    Decision biggestDiffDecision;

    // 程序化场景生成 DNA 10 次防重复历史
    ScenarioDNA recentDNAs[RECENT_DNA_HISTORY_SIZE];
    int dnaHistoryCount;

    // 畅玩闭环与把玩统计
    int totalPlays;

    uint32_t animStartTime;
    uint32_t animProgress; // 0 ~ 100

    // 雷达图平滑形变插值动效变量
    RadarData startRadar;
    RadarData endRadar;
    RadarData currentRadar;
    uint32_t radarAnimStartTime;
    bool isRadarAnimActive;
};

#endif // UI_STATE_H
