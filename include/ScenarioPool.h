#ifndef SCENARIO_POOL_H
#define SCENARIO_POOL_H

#ifdef CHANGE
#undef CHANGE
#endif

#include "Scenario.h"

enum class ScenarioCategory {
    TRAVEL = 0,
    MONEY = 1,
    PURCHASE = 2,
    SOCIAL = 3,
    FRIENDSHIP = 4,
    WORK = 5,
    PLANNING = 6,
    RISK = 7,
    OPPORTUNITY = 8,
    TIME = 9,
    COMFORT = 10,
    ADVENTURE = 11
};

struct ConcreteScenario {
    const char* id;           // 例如 "TRAVEL_001"
    ScenarioCategory category;
    const char* titleCN;
    const char* descCN;
    const char* titleEN;
    const char* descEN;
    const char* choiceA_CN;   // 选项 A 中文 (如 "立即出发")
    const char* choiceB_CN;   // 选项 B 中文 (如 "婉言拒绝")
    const char* choiceA_EN;
    const char* choiceB_EN;
    const char* tags[4];      // 多维内部标签
    Scenario scenario;        // 6 维权重结构
};

constexpr int SCENARIO_BANK_SIZE = 60;

// 获取 ScenarioCategory 的中文/英文文本名称
const char* getCategoryNameCN(ScenarioCategory cat);
const char* getCategoryNameEN(ScenarioCategory cat);

// 从 60+ 高质量生活场景库中随机挑选场景 (带 5 次 Category 调配与防重复算法)
const ConcreteScenario& getNextBankScenario(ScenarioCategory recentCats[5], int recentCount);

#endif // SCENARIO_POOL_H
