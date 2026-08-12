#ifndef DECISION_ARCHETYPE_H
#define DECISION_ARCHETYPE_H

#include "ScenarioPool.h"
#include "Scenario.h"
#include <cstdint>

// 20 个 决策困境原型 ID
enum class ArchetypeID {
    LAST_MINUTE_OPPORTUNITY = 0,
    PLAN_CHANGE,
    LIMITED_TIME_PURCHASE,
    DUPLICATE_PURCHASE,
    UNEXPECTED_REQUEST,
    SOCIAL_INVITATION,
    STRANGER_INTERACTION,
    FRIEND_CONFLICT,
    SECRET_REQUEST,
    UNCERTAIN_OPPORTUNITY,
    KNOWN_VS_UNKNOWN,
    EFFICIENCY_VS_COMFORT,
    AUTONOMY_VS_GUIDANCE,
    PLAN_VS_IMPROVISATION,
    HELP_VS_BOUNDARY,
    RISK_VS_CERTAINTY,
    NOW_VS_LATER,
    QUALITY_VS_COST,
    OPPORTUNITY_VS_REST,
    PERSISTENCE_VS_EFFICIENCY
};

constexpr int ARCHETYPE_COUNT = 20;

struct DynamicScenario {
    char id[24];
    ArchetypeID archetype;
    ScenarioCategory category;
    char titleCN[48];
    char descCN[160];
    char titleEN[48];
    char descEN[160];
    char choiceA_CN[24];
    char choiceB_CN[24];
    char choiceA_EN[24];
    char choiceB_EN[24];
    const char* tags[4];
    Scenario scenario;
    uint32_t seed;
};

// 获取 ArchetypeID 的字符串标识
const char* getArchetypeIdString(ArchetypeID id);

// 动态场景生成核心入口函数 (带约束与历史去重)
DynamicScenario generateDynamicArchetypeScenario(ArchetypeID recentArchetypes[8], int historyCount);

#endif // DECISION_ARCHETYPE_H
