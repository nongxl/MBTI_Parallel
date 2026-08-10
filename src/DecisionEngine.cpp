#include "DecisionEngine.h"
#include <cmath>
#include <cstdio>
#include <cstring>

const char* getDecisionName(Decision decision) {
    switch (decision) {
        case Decision::YES: return "YES";
        case Decision::NO: return "NO";
        case Decision::MAYBE: return "MAYBE";
        default: return "UNKNOWN";
    }
}

DecisionResult simulate(const Scenario& scenario, MBTIType personality) {
    const PersonalityProfile& profile = getMBTIProfile(personality);

    // 1. 风险与不确定性拉锯计算
    float scenarioRiskTotal = scenario.risk * 0.6f + scenario.uncertainty * 0.4f;
    float profileRiskTolerance = profile.risk * 0.6f + (100.0f - profile.planning) * 0.4f;
    float deltaRisk = (profileRiskTolerance - scenarioRiskTotal) * 0.35f;

    // 2. 新奇/已知偏好计算
    float deltaNovelty = 0.0f;
    if (scenario.novelty > 50.0f) {
        deltaNovelty = (profile.novelty - 50.0f) * (scenario.novelty / 100.0f) * 0.30f;
    } else {
        deltaNovelty = (50.0f - profile.novelty) * ((100.0f - scenario.novelty) / 100.0f) * 0.25f;
    }

    // 3. 社交与情感因素计算
    float profileEmotion = 100.0f - profile.logic;
    float deltaSocial = (profile.social - 50.0f) * (scenario.social / 100.0f) * 0.25f;
    float deltaEmotion = (profileEmotion - 50.0f) * (scenario.emotionalValue / 100.0f) * 0.25f;
    float deltaSocialEmotion = deltaSocial + deltaEmotion;

    // 4. 实用收益与代价阻力计算
    float profilePractical = profile.practicality * 0.6f + profile.logic * 0.4f;
    float deltaPracticalGain = (profilePractical - 50.0f) * (scenario.practicalValue / 100.0f) * 0.30f;
    
    float scenarioBurden = scenario.cost * 0.4f + scenario.effort * 0.4f + scenario.time * 0.2f;
    float deltaBurdenPenalty = 0.0f;
    if (scenarioBurden > 30.0f) {
        deltaBurdenPenalty = -1.0f * (profilePractical / 100.0f) * (scenarioBurden - 30.0f) * 0.25f;
    }
    float deltaPracticalCost = deltaPracticalGain + deltaBurdenPenalty;

    // 5. 汇总计算得分
    float totalScore = 50.0f + deltaRisk + deltaNovelty + deltaSocialEmotion + deltaPracticalCost;

    // Clamp 到 [0, 100]
    if (totalScore > 100.0f) totalScore = 100.0f;
    if (totalScore < 0.0f) totalScore = 0.0f;

    // 判断 Decision 与 Confidence
    Decision decision = Decision::MAYBE;
    if (totalScore >= YES_THRESHOLD) {
        decision = Decision::YES;
    } else if (totalScore <= NO_THRESHOLD) {
        decision = Decision::NO;
    }

    float confidence = std::abs(totalScore - 50.0f);

    // 6. 简短 Template-based Reason 生成
    // 找出影响最显著的偏好方向
    const char* riskPosStr = "Accepts risk & uncertainty";
    const char* riskNegStr = "Prefers safety & certainty";
    const char* novPosStr  = "Drawn to novel ideas";
    const char* novNegStr  = "Prefers routine & familiarity";
    const char* socPosStr  = "Values social & emotional gain";
    const char* socNegStr  = "Low social/emotional interest";
    const char* prcPosStr  = "High practical benefit";
    const char* prcNegStr  = "Cost/effort exceeds practical value";

    const char* primaryReason = nullptr;
    const char* secondaryReason = nullptr;

    // 判断主导因子
    float absRisk = std::abs(deltaRisk);
    float absNov = std::abs(deltaNovelty);
    float absSoc = std::abs(deltaSocialEmotion);
    float absPrc = std::abs(deltaPracticalCost);

    // 查找最大和第二大因子
    float firstMax = -1.0f;
    float secondMax = -1.0f;

    auto checkFactor = [&](float val, const char* pos, const char* neg) {
        float absVal = std::abs(val);
        const char* str = (val >= 0.0f) ? pos : neg;
        if (absVal > firstMax) {
            secondMax = firstMax;
            secondaryReason = primaryReason;
            firstMax = absVal;
            primaryReason = str;
        } else if (absVal > secondMax) {
            secondMax = absVal;
            secondaryReason = str;
        }
    };

    checkFactor(deltaRisk, riskPosStr, riskNegStr);
    checkFactor(deltaNovelty, novPosStr, novNegStr);
    checkFactor(deltaSocialEmotion, socPosStr, socNegStr);
    checkFactor(deltaPracticalCost, prcPosStr, prcNegStr);

    DecisionResult result;
    result.personality = personality;
    result.decision = decision;
    result.score = totalScore;
    result.confidence = confidence;

    if (primaryReason && secondaryReason && secondMax >= 5.0f) {
        snprintf(result.reason, sizeof(result.reason), "%s; %s", primaryReason, secondaryReason);
    } else if (primaryReason) {
        snprintf(result.reason, sizeof(result.reason), "%s", primaryReason);
    } else {
        snprintf(result.reason, sizeof(result.reason), "Balanced factors");
    }

    // 确保长度 <= 60 字符
    result.reason[63] = '\0';

    return result;
}

void simulateAll(const Scenario& scenario, DecisionResult results[MBTI_COUNT]) {
    for (int i = 0; i < MBTI_COUNT; ++i) {
        MBTIType type = static_cast<MBTIType>(i);
        results[i] = simulate(scenario, type);
    }
}

DecisionSummary summarizeResults(const DecisionResult results[MBTI_COUNT]) {
    DecisionSummary summary;
    summary.yesCount = 0;
    summary.noCount = 0;
    summary.maybeCount = 0;

    float highestScore = -1.0f;
    float lowestScore = 101.0f;
    summary.strongestYes = MBTIType::ISTJ;
    summary.strongestNo = MBTIType::ISTJ;

    for (int i = 0; i < MBTI_COUNT; ++i) {
        if (results[i].decision == Decision::YES) {
            summary.yesCount++;
        } else if (results[i].decision == Decision::NO) {
            summary.noCount++;
        } else {
            summary.maybeCount++;
        }

        if (results[i].score > highestScore) {
            highestScore = results[i].score;
            summary.strongestYes = results[i].personality;
        }

        if (results[i].score < lowestScore) {
            lowestScore = results[i].score;
            summary.strongestNo = results[i].personality;
        }
    }

    return summary;
}

void findBiggestSplit(const DecisionResult results[MBTI_COUNT], MBTIType& yesType, MBTIType& noType) {
    DecisionSummary summary = summarizeResults(results);
    yesType = summary.strongestYes;
    noType = summary.strongestNo;
}
