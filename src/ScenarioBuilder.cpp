#include "ScenarioBuilder.h"
#include <cstdio>
#include <cstring>

const char* getWhoName(WhoType who, bool isCN) {
    switch (who) {
        case WhoType::FRIEND:       return isCN ? "朋友" : "FRIEND";
        case WhoType::COWORKER:     return isCN ? "同事" : "COWORKER";
        case WhoType::STRANGER:     return isCN ? "陌生人" : "STRANGER";
        case WhoType::GROUP:        return isCN ? "一群人" : "GROUP";
        case WhoType::MYSELF:       return isCN ? "自己" : "MYSELF";
        case WhoType::ACQUAINTANCE: return isCN ? "熟人" : "KNOWN PEER";
    }
    return "UNKNOWN";
}

const char* getSituationName(SituationType sit, bool isCN) {
    switch (sit) {
        case SituationType::TRAVEL:     return isCN ? "出行旅游" : "TRAVEL";
        case SituationType::INVITATION: return isCN ? "聚会邀约" : "INVITATION";
        case SituationType::PURCHASE:   return isCN ? "购物消费" : "PURCHASE";
        case SituationType::HELP:       return isCN ? "寻求帮助" : "HELP NEEDED";
        case SituationType::HOBBY:      return isCN ? "兴趣尝试" : "HOBBY TRY";
        case SituationType::CHALLENGE:  return isCN ? "重大挑战" : "CHALLENGE";
    }
    return "UNKNOWN";
}

const char* getConditionName(ConditionType cond, bool isCN) {
    switch (cond) {
        case ConditionType::LAST_MINUTE: return isCN ? "临时决定" : "LAST MINUTE";
        case ConditionType::UNKNOWN:     return isCN ? "完全未知" : "UNKNOWN";
        case ConditionType::EXPENSIVE:   return isCN ? "花费较高" : "EXPENSIVE";
        case ConditionType::RISKY:       return isCN ? "高风险性" : "HIGH RISK";
        case ConditionType::NO_PLAN:     return isCN ? "毫无计划" : "NO PLAN";
        case ConditionType::UNEXPECTED:  return isCN ? "出乎意料" : "UNEXPECTED";
    }
    return "UNKNOWN";
}

const char* getTensionName(TensionType ten, bool isCN) {
    switch (ten) {
        case TensionType::SAFETY_VS_NOVELTY:   return isCN ? "安全 / 新鲜" : "SAFETY / NOVELTY";
        case TensionType::PLAN_VS_IMPROVISE:   return isCN ? "计划 / 即兴" : "PLAN / IMPROVISE";
        case TensionType::SAVE_VS_ENJOY:       return isCN ? "省钱 / 享受" : "SAVE / ENJOY";
        case TensionType::ALONE_VS_SOCIAL:     return isCN ? "独处 / 社交" : "ALONE / SOCIAL";
        case TensionType::CERTAIN_VS_UNKNOWN:  return isCN ? "确定 / 未知" : "CERTAIN / UNKNOWN";
        case TensionType::EASY_VS_CHALLENGE:   return isCN ? "轻松 / 挑战" : "EASY / CHALLENGE";
    }
    return "UNKNOWN";
}

RenderedCustomScenario renderCustomScenario(const CustomScenarioDNA& dna) {
    RenderedCustomScenario res;
    res.dna = dna;

    // 确定 Scenario 向量属性 (0 ~ 100)
    res.scenario.type = DecisionType::DO;
    res.scenario.risk = (dna.condition == ConditionType::RISKY) ? 85.0f : ((dna.tension == TensionType::SAFETY_VS_NOVELTY) ? 65.0f : 30.0f);
    res.scenario.novelty = (dna.tension == TensionType::SAFETY_VS_NOVELTY || dna.condition == ConditionType::UNKNOWN) ? 85.0f : 45.0f;
    res.scenario.social = (dna.who == WhoType::GROUP || dna.who == WhoType::FRIEND || dna.tension == TensionType::ALONE_VS_SOCIAL) ? 80.0f : 20.0f;
    res.scenario.uncertainty = (dna.condition == ConditionType::NO_PLAN || dna.condition == ConditionType::UNKNOWN) ? 85.0f : 35.0f;
    res.scenario.time = (dna.condition == ConditionType::LAST_MINUTE) ? 85.0f : 40.0f;
    res.scenario.cost = (dna.condition == ConditionType::EXPENSIVE) ? 80.0f : 30.0f;
    res.scenario.effort = 40.0f;
    res.scenario.emotionalValue = (dna.tension == TensionType::SAVE_VS_ENJOY) ? 85.0f : 45.0f;
    res.scenario.practicalValue = (dna.tension == TensionType::PLAN_VS_IMPROVISE) ? 80.0f : 40.0f;

    // 拼接场景标题与中英文句子
    snprintf(res.titleEN, sizeof(res.titleEN), "%s %s", getWhoName(dna.who, false), getSituationName(dna.situation, false));
    snprintf(res.titleCN, sizeof(res.titleCN), "%s%s场景", getWhoName(dna.who, true), getSituationName(dna.situation, true));

    // 根据 4 步组合渲染地道中英文句式
    snprintf(res.descEN, sizeof(res.descEN), "%s asks for %s under %s conditions (%s).",
             getWhoName(dna.who, false), getSituationName(dna.situation, false),
             getConditionName(dna.condition, false), getTensionName(dna.tension, false));

    snprintf(res.descCN, sizeof(res.descCN), "%s提出进行%s，面临%s情况(%s)，你同意吗？",
             getWhoName(dna.who, true), getSituationName(dna.situation, true),
             getConditionName(dna.condition, true), getTensionName(dna.tension, true));

    return res;
}
