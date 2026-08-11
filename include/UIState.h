#ifndef UI_STATE_H
#define UI_STATE_H

#include <cstdint>
#include "MBTIProfile.h"
#include "Scenario.h"
#include "DecisionEngine.h"
#include "ScenarioBuilder.h"
#include "DecisionProfile.h"
#include "RadarChart.h"

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

struct UIContext {
    AppState state;
    UserSelection currentSelection;
    Scenario currentScenario;
    
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
