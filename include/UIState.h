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
#include "CustomBuilder.h"

enum class AppState {
    LANGUAGE_SELECT,            // 首次开机语言选择屏
    HOME,
    MY_PROFILE,                 // 真实长效 MBTI 人格画像看板
    MY_PROFILE_CLEAR_CONFIRM,   // 清空历史确认二次弹窗屏
    BUILDER_INTENT,             // Step 1: 决策意图 (我该不该买/去/答应？)
    BUILDER_CONTEXT,            // Step 2: 关联对象 (朋友/同事/旧设备)
    BUILDER_TENSION,            // Step 3: 犹豫痛点 (耽误原计划/不好拒绝/旧的还能用)
    BUILDER_PREVIEW,
    SIMULATING,
    SUMMARY,
    BIGGEST_SPLIT,
    EXPLORE,
    YOUR_CHOICE,
    YOUR_MATCH,
    WHY_MATCH                   // 【Phase 6A】最相似 3 维度契合解析屏
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
    Language lang;      // 当前语言设置 (ENGLISH / CHINESE)
    int bootMenuMode;   // 开机选框模式: 0 (随机模式 RANDOM), 1 (自定义模式 CREATE), 2 (真实人格 MY_PROFILE)
    bool isFirstLaunch; // 标记是否为首次烧录/未配置语言

    // Phase 6E 极简 3 步 Custom Decision State
    CustomDecisionState customDecision;

    // 自定义决策场景 4 步 DNA (兼容)
    CustomScenarioDNA customDNA;

    UserSelection currentSelection;
    Scenario currentScenario;
    char currentScenarioTitle[64];
    char currentScenarioDesc[512];   // 512 字节全量 4 段故事
    char currentScenarioTitleCN[64];
    char currentScenarioDescCN[512]; // 512 字节彻底杜绝溢出截断

    int previewScrollY; // 场景预览长文本上下滚动偏移量

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

    // 【Phase 6A】WHY_MATCH 解析：契合度最高的 3 个维度索引与具体分值
    int whyMatchDims[3];       // 0: Novelty, 1: Risk, 2: Planning, 3: Practicality, 4: Logic, 5: Social
    float whyMatchUserVals[3];
    float whyMatchMbtiVals[3];

    // 本题分歧最大的人格与决策结果
    MBTIType biggestDiffMBTI;
    Decision biggestDiffDecision;

    // 用户长效 ESP32 NVS 决策历史持久化与统计均值
    UserHistoryStats userHistory;

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
