#include "CustomBuilder.h"
#include <cstring>

const char* getIntentNameCN(DecisionIntent intent) {
    switch (intent) {
        case DecisionIntent::ACCEPT_OR_DECLINE: return "我该不该答应？";
        case DecisionIntent::BUY_OR_NOT:        return "我该不该买？";
        case DecisionIntent::GO_OR_NOT:         return "我该不该去？";
        case DecisionIntent::CHANGE_OR_KEEP:    return "我该不该改变？";
        case DecisionIntent::TRY_OR_WAIT:       return "我该不该尝试？";
        case DecisionIntent::OTHER:             return "其他日常微抉择";
    }
    return "未知意图";
}

const char* getContextNameCN(DecisionIntent intent, int index) {
    static const char* defaultContexts[] = { "关于朋友/老同学", "关于同事/工作项目", "关于旧数码设备/物品", "关于个人日常休息/约会" };
    if (index >= 0 && index < 4) {
        return defaultContexts[index];
    }
    return "通用情境";
}

const char* getTensionNameCN(DecisionIntent intent, int index) {
    if (intent == DecisionIntent::BUY_OR_NOT) {
        static const char* buyTensions[] = {
            "旧的还能用，买新的要花不少钱",
            "很想体验新特性，但性价比不高",
            "活动限时打折，过了就没有优惠",
            "纯粹一时冲动，不知道买来值不值"
        };
        if (index >= 0 && index < 4) return buyTensions[index];
    } else if (intent == DecisionIntent::GO_OR_NOT || intent == DecisionIntent::ACCEPT_OR_DECLINE) {
        static const char* goTensions[] = {
            "其实想去，但会耽误原本居家计划",
            "不太想去，但对方热情不好拒绝",
            "需要额外的花费和精力开销",
            "对活动细节不了解，不知道值不值"
        };
        if (index >= 0 && index < 4) return goTensions[index];
    }

    static const char* defaultTensions[] = {
        "其实很想做，但担心耽误既定安排",
        "顾及对方感受，觉得有些不好拒绝",
        "要投入不少资金或精力成本",
        "存在不确定性，不知道是否划算"
    };
    if (index >= 0 && index < 4) return defaultTensions[index];
    return "犹豫不决";
}

void buildCustomScenarioState(const CustomDecisionState& customState, ScenarioState& outState, ScenarioCategory& outCategory, ArchetypeID& outArchetype) {
    outState.reset();

    // 1. 动态确定 Category 与 Archetype
    if (customState.intent == DecisionIntent::BUY_OR_NOT) {
        outCategory = ScenarioCategory::PURCHASE;
        outArchetype = (customState.tensionIndex == 0) ? ArchetypeID::DUPLICATE_PURCHASE : ArchetypeID::LIMITED_TIME_PURCHASE;
        outState.costType = CostType::BUDGET;
    } else if (customState.intent == DecisionIntent::GO_OR_NOT || customState.intent == DecisionIntent::ACCEPT_OR_DECLINE) {
        if (customState.contextIndex == 1) { // 同事 / 工作
            outCategory = ScenarioCategory::WORK;
            outArchetype = ArchetypeID::UNEXPECTED_REQUEST;
            outState.existingPlan = ExistingPlan::WORK;
        } else { // 朋友 / 旅行
            outCategory = ScenarioCategory::TRAVEL;
            outArchetype = ArchetypeID::LAST_MINUTE_OPPORTUNITY;
            outState.existingPlan = ExistingPlan::STAY_HOME;
        }
    } else {
        outCategory = ScenarioCategory::WORK;
        outArchetype = ArchetypeID::KNOWN_VS_UNKNOWN;
        outState.existingPlan = ExistingPlan::REST;
    }

    // 2. 将选定的 Tension 全量映射至标准 ScenarioState
    switch (customState.tensionIndex) {
        case 0: // 耽误原计划 / 机会成本
            outState.isPlanLocked = true;
            if (outState.existingPlan == ExistingPlan::NONE) {
                outState.existingPlan = ExistingPlan::STAY_HOME;
            }
            break;

        case 1: // 不好拒绝 / 人际期待与边界
            outState.isPlanLocked = true;
            outState.existingPlan = ExistingPlan::REST;
            break;

        case 2: // 旧的还能用 / 花不少钱 / 预算控制
            outState.isCostLocked = true;
            outState.costType = CostType::BUDGET;
            break;

        case 3: // 对细节不了解 / 不知道值不值得 / 不确定性
            outState.isCostLocked = true;
            outState.costType = CostType::TIME;
            break;
    }
}
