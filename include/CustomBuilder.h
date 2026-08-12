#ifndef CUSTOM_BUILDER_H
#define CUSTOM_BUILDER_H

#include "ScenarioState.h"
#include "DecisionMechanism.h"
#include "Scenario.h"
#include "ScenarioPool.h"
#include "DecisionArchetype.h"
#include <cstdint>

enum class DecisionIntent {
    ACCEPT_OR_DECLINE = 0, // 我该不该答应？
    BUY_OR_NOT,            // 我该不该买？
    GO_OR_NOT,             // 我该不该去？
    CHANGE_OR_KEEP,        // 我该不该改变？
    TRY_OR_WAIT,           // 我该不该尝试？
    OTHER                  // 一句话描述 / 其他
};

struct CustomDecisionState {
    DecisionIntent intent;
    int contextIndex;  // 对象领域索引: 0(朋友), 1(同事), 2(设备/物品), 3(日常/个人)
    int tensionIndex;  // 痛点索引: 0(耽误原计划), 1(不好拒绝), 2(旧的还能用/花钱), 3(不知道值不值得)

    ScenarioCategory derivedCategory;
    DecisionMechanism derivedMechanism;
    ScenarioFact confirmedFact;

    void reset() {
        intent = DecisionIntent::ACCEPT_OR_DECLINE;
        contextIndex = 0;
        tensionIndex = 0;
        derivedCategory = ScenarioCategory::TRAVEL;
        derivedMechanism = DecisionMechanism::OPPORTUNITY_COST;
        confirmedFact = { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE };
    }
};

const char* getIntentNameCN(DecisionIntent intent);
const char* getContextNameCN(DecisionIntent intent, int index);
const char* getTensionNameCN(DecisionIntent intent, int index);

// 将用户的 3 步极简选择转化为标准的 ScenarioState 与 Scenario
void buildCustomScenarioState(const CustomDecisionState& customState, ScenarioState& outState, ScenarioCategory& outCategory, ArchetypeID& outArchetype);

#endif // CUSTOM_BUILDER_H
