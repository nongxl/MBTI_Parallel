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
    BUILDER_TYPE,
    BUILDER_MOTIVATION,
    BUILDER_CONCERN,
    BUILDER_INTENSITY,
    BUILDER_PRIORITY,
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

struct UIContext {
    AppState state;
    UserSelection currentSelection;
    Scenario currentScenario;
    char currentScenarioTitle[32];
    char currentScenarioDesc[80];

    DecisionResult results[MBTI_COUNT];
    DecisionSummary summary;
    MBTIType splitYesType;
    MBTIType splitNoType;

    int selectedMenuIndex;
    int exploreIndex;
    Decision userChoice;

    DecisionProfile userProfile;
    MBTIType closestMBTI;
    float matchSimilarity;
    MatchType matchType;

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
