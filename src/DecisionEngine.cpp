#include "DecisionEngine.h"
#include <cmath>
#include <algorithm>
#include <cstring>

static float clampValue(float val, float minV = 0.0f, float maxV = 100.0f) {
    return std::max(minV, std::min(maxV, val));
}

DecisionResult evaluateDecision(const PersonalityProfile& profile, const Scenario& scenario) {
    DecisionResult res;
    res.personality = profile.type;

    // 六维综合权重加权算法
    float score = 50.0f; // 基准中立分

    score += (profile.risk - 50.0f) * (scenario.risk / 100.0f) * 0.25f;
    score += (profile.novelty - 50.0f) * (scenario.novelty / 100.0f) * 0.25f;
    score += (profile.social - 50.0f) * (scenario.social / 100.0f) * 0.20f;
    score += (profile.logic - 50.0f) * (scenario.practicalValue / 100.0f) * 0.15f;
    score -= (profile.planning - 50.0f) * (scenario.uncertainty / 100.0f) * 0.20f;
    score += (profile.practicality - 50.0f) * (scenario.practicalValue / 100.0f) * 0.15f;

    res.score = clampValue(score);

    // 确定 Decision 结果与决策依据
    if (res.score >= 58.0f) {
        res.decision = Decision::YES;
        if (profile.novelty >= 75.0f && scenario.novelty >= 50.0f) {
            res.reason = "NOVELTY SEEKING INSTINCT";
        } else if (profile.risk >= 70.0f && scenario.risk >= 50.0f) {
            res.reason = "HIGH RISK ACCEPTANCE";
        } else if (profile.social >= 70.0f && scenario.social >= 50.0f) {
            res.reason = "SOCIAL INVOLVEMENT PREFERENCE";
        } else if (profile.logic >= 70.0f) {
            res.reason = "ANALYTICAL LOGIC MATCH";
        } else {
            res.reason = "POSITIVE VALUE ALIGNMENT";
        }
    } else if (res.score <= 42.0f) {
        res.decision = Decision::NO;
        if (profile.planning >= 70.0f && scenario.uncertainty >= 40.0f) {
            res.reason = "STRUCTURED PLANNING NEEDED";
        } else if (profile.risk <= 35.0f && scenario.risk >= 50.0f) {
            res.reason = "CAUTIOUS RISK AVOIDANCE";
        } else if (profile.social <= 35.0f && scenario.social >= 60.0f) {
            res.reason = "SOLITUDE & FREEDOM PREFERRED";
        } else {
            res.reason = "STABILITY & SAFETY FIRST";
        }
    } else {
        res.decision = Decision::MAYBE;
        res.reason = "BALANCED WEIGHING OPTION";
    }

    return res;
}

void simulateAll(const Scenario& scenario, DecisionResult outResults[MBTI_COUNT]) {
    for (int i = 0; i < MBTI_COUNT; ++i) {
        const PersonalityProfile& prof = getMBTIProfile(static_cast<MBTIType>(i));
        outResults[i] = evaluateDecision(prof, scenario);
    }
}

DecisionSummary summarizeResults(const DecisionResult results[MBTI_COUNT]) {
    DecisionSummary summary = {0, 0, 0};
    for (int i = 0; i < MBTI_COUNT; ++i) {
        if (results[i].decision == Decision::YES) summary.yesCount++;
        else if (results[i].decision == Decision::NO) summary.noCount++;
        else summary.maybeCount++;
    }
    return summary;
}

void findBiggestSplit(const DecisionResult results[MBTI_COUNT], MBTIType& yesType, MBTIType& noType) {
    float maxYesScore = -1.0f;
    float minNoScore = 999.0f;
    yesType = MBTIType::ENTP;
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
        case Decision::YES: return "YES";
        case Decision::NO: return "NO";
        case Decision::MAYBE: return "MAYBE";
    }
    return "UNKNOWN";
}

const char* getDecisionNameCN(Decision decision) {
    switch (decision) {
        case Decision::YES: return "同意";
        case Decision::NO: return "拒绝";
        case Decision::MAYBE: return "犹豫";
    }
    return "未知";
}

const char* getDecisionReasonCN(const char* reasonEN) {
    if (!reasonEN) return "综合价值对齐";

    if (strstr(reasonEN, "NOVELTY SEEKING")) return "极度渴望新奇体验";
    if (strstr(reasonEN, "HIGH RISK ACCEPTANCE")) return "高度偏好高风险尝试";
    if (strstr(reasonEN, "SOCIAL INVOLVEMENT")) return "追求积极社交参与";
    if (strstr(reasonEN, "ANALYTICAL LOGIC")) return "符合严密理性分析";
    if (strstr(reasonEN, "POSITIVE VALUE")) return "符合积极价值倾向";
    if (strstr(reasonEN, "STRUCTURED PLANNING")) return "依赖清晰计划保障";
    if (strstr(reasonEN, "CAUTIOUS RISK AVOIDANCE")) return "倾向规避未知风险";
    if (strstr(reasonEN, "SOLITUDE & FREEDOM")) return "偏好独立自由空间";
    if (strstr(reasonEN, "STABILITY & SAFETY")) return "安全稳定第一要务";
    if (strstr(reasonEN, "BALANCED WEIGHING")) return "权衡利弊犹豫不决";
    if (strstr(reasonEN, "PRACTICAL VALUE")) return "优先考虑现实实用价值";

    return "综合权衡个人偏好";
}
