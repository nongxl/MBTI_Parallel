#include "DecisionEngine.h"
#include <cmath>

static float clampScore(float score) {
    if (score < 0.0f) return 0.0f;
    if (score > 100.0f) return 100.0f;
    return score;
}

DecisionResult simulateMBTI(MBTIType personality, const Scenario& scenario) {
    const PersonalityProfile& profile = getMBTIProfile(personality);

    // 向量加权决策意愿算法 (全量重构对撞引擎)
    float score = 50.0f;

    // 1. 计划性与突发改变原计划对撞 (J vs P) - scenario.time 代表时间投入与原计划冲突
    score -= (profile.planning * scenario.time / 100.0f) * 0.30f;
    score += ((100.0f - profile.planning) * scenario.novelty / 100.0f) * 0.20f;

    // 2. 社交精力与个人边界对撞 (E/F vs I/T) - scenario.effort 代表精力消耗
    score += (profile.social * scenario.social / 100.0f) * 0.25f;
    score -= ((100.0f - profile.social) * scenario.effort / 100.0f) * 0.35f;

    // 3. 逻辑理性与人情道德绑架抗性 (T 型边界抗性) - scenario.social 代表人际道德包袱
    score -= (profile.logic * scenario.social / 100.0f) * 0.25f;

    // 4. 风险与不确定度考量 (避险 vs 探索)
    score += (profile.risk - scenario.risk) * 0.20f;
    score -= (scenario.uncertainty * (100.0f - profile.planning) / 100.0f) * 0.20f;

    // 5. 实用价值与花费支出 (SJ 节约 vs 消耗)
    score += (profile.practicality * scenario.practicalValue / 100.0f) * 0.25f;
    score -= ((100.0f - profile.practicality) * scenario.cost / 100.0f) * 0.30f;

    score = clampScore(score);

    Decision decision;
    if (score >= 55.0f) {
        decision = Decision::YES;
    } else if (score <= 45.0f) {
        decision = Decision::NO;
    } else {
        decision = Decision::MAYBE;
    }

    const char* reason = "综合评估个人偏好与场景风险";
    if (scenario.risk > 60.0f && profile.risk < 40.0f) {
        reason = "风险过高，超出安全边界";
    } else if (scenario.social > 70.0f && profile.logic > 70.0f) {
        reason = "坚守个人边界，拒绝无谓消耗";
    } else if (scenario.novelty > 70.0f && profile.novelty > 70.0f) {
        reason = "极为新奇，高度契合探险渴望";
    } else if (scenario.social > 70.0f && profile.social < 30.0f) {
        reason = "社交消耗过大，偏好独立自主";
    } else if (scenario.practicalValue > 70.0f && profile.practicality > 70.0f) {
        reason = "务实高效，具备极高实际价值";
    }

    return { personality, decision, score, reason };
}

void simulateAll(const Scenario& scenario, DecisionResult results[MBTI_COUNT]) {
    for (int i = 0; i < MBTI_COUNT; ++i) {
        results[i] = simulateMBTI(static_cast<MBTIType>(i), scenario);
    }
}

DecisionSummary summarizeResults(const DecisionResult results[MBTI_COUNT]) {
    DecisionSummary summary = { 0, 0, 0 };
    for (int i = 0; i < MBTI_COUNT; ++i) {
        if (results[i].decision == Decision::YES) {
            summary.yesCount++;
        } else if (results[i].decision == Decision::NO) {
            summary.noCount++;
        } else {
            summary.maybeCount++;
        }
    }
    return summary;
}

void findBiggestSplit(const DecisionResult results[MBTI_COUNT], MBTIType& yesType, MBTIType& noType) {
    float maxYesScore = -1.0f;
    float minNoScore = 101.0f;
    yesType = MBTIType::ENFP;
    noType = MBTIType::ISTJ;

    for (int i = 0; i < MBTI_COUNT; ++i) {
        if (results[i].decision == Decision::YES && results[i].score > maxYesScore) {
            maxYesScore = results[i].score;
            yesType = results[i].personality;
        }
        if (results[i].decision == Decision::NO && results[i].score < minNoScore) {
            minNoScore = results[i].score;
            noType = results[i].personality;
        }
    }
}

const char* getDecisionName(Decision decision) {
    switch (decision) {
        case Decision::YES:   return "YES";
        case Decision::NO:    return "NO";
        case Decision::MAYBE: return "MAYBE";
    }
    return "UNKNOWN";
}

const char* getScenarioActionNameCN(Decision decision, DecisionType type) {
    if (type == DecisionType::GET) {
        if (decision == Decision::YES) return "果断购买";
        if (decision == Decision::NO)  return "理性克制";
        return "犹豫观望";
    } else if (type == DecisionType::DO) {
        if (decision == Decision::YES) return "留下来帮";
        if (decision == Decision::NO)  return "准时下班";
        return "犹豫观望";
    } else if (type == DecisionType::GO) {
        if (decision == Decision::YES) return "果断前往";
        if (decision == Decision::NO)  return "按原计划";
        return "犹豫观望";
    }
    if (decision == Decision::YES) return "果断同意";
    if (decision == Decision::NO)  return "明确拒绝";
    return "犹豫观望";
}

const char* getScenarioActionNameEN(Decision decision, DecisionType type) {
    if (type == DecisionType::GET) {
        if (decision == Decision::YES) return "BUY NOW";
        if (decision == Decision::NO)  return "PASS & SAVE";
        return "MAYBE";
    } else if (type == DecisionType::DO) {
        if (decision == Decision::YES) return "STAY & HELP";
        if (decision == Decision::NO)  return "LEAVE ON TIME";
        return "MAYBE";
    } else if (type == DecisionType::GO) {
        if (decision == Decision::YES) return "ACCEPT & GO";
        if (decision == Decision::NO)  return "STICK TO PLAN";
        return "MAYBE";
    }
    if (decision == Decision::YES) return "YES";
    if (decision == Decision::NO)  return "NO";
    return "MAYBE";
}

const char* getDecisionReasonCN(const char* reasonEN, MBTIType personality, Decision decision, DecisionType type) {
    if (decision == Decision::NO) {
        if (personality == MBTIType::ISTJ || personality == MBTIType::INTJ) {
            return "严格遵守日程计划与个人时间边界";
        } else if (personality == MBTIType::ISTP || personality == MBTIType::INTP) {
            return "评估后认为精力产出性价比过低";
        }
        return "出于个人边界与精力保护做出拒绝";
    } else if (decision == Decision::YES) {
        if (personality == MBTIType::ESFJ || personality == MBTIType::ENFJ) {
            return "重视社交人际关系与他人同理期待";
        } else if (personality == MBTIType::ENFP || personality == MBTIType::ENTP) {
            return "渴望探索新奇体验与突发可能性";
        }
        return "出于积极参与和体验尝试做出的选择";
    }
    return "在多种因素权衡下处于中立犹豫观望";
}
