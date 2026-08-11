#ifndef SCENARIO_POOL_H
#define SCENARIO_POOL_H

#ifdef CHANGE
#undef CHANGE
#endif

#include "Scenario.h"

enum class ScenarioCategory {
    SOCIAL,
    RISK,
    EXPERIENCE,
    MONEY,
    CHANGE,
    CREATIVITY,
    TIME,
    UNCERTAINTY
};

struct ScenarioTemplate {
    int id;
    const char* title;
    const char* description;
    ScenarioCategory category;
    Scenario scenario;
};

constexpr int SCENARIO_POOL_SIZE = 20;

// 获取随机防重复场景 (避开最近 3 次出过的 id)
const ScenarioTemplate& getRandomScenario(int recentIds[3]);

#endif // SCENARIO_POOL_H
