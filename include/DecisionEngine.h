#ifndef DECISION_ENGINE_H
#define DECISION_ENGINE_H

#include "MBTIProfile.h"
#include "Scenario.h"

enum class Decision {
    YES,
    NO,
    MAYBE
};

struct DecisionResult {
    MBTIType personality;
    Decision decision;
    float score; // 0 ~ 100 综合决策倾向分
    const char* reason;
};

struct DecisionSummary {
    int yesCount;
    int noCount;
    int maybeCount;
};

// 计算单个 MBTI 对 Scenario 的决策
DecisionResult evaluateDecision(const PersonalityProfile& profile, const Scenario& scenario);

// 模拟所有 16 种 MBTI
void simulateAll(const Scenario& scenario, DecisionResult outResults[MBTI_COUNT]);

// 汇总统计结果
DecisionSummary summarizeResults(const DecisionResult results[MBTI_COUNT]);

// 寻找分歧最大的性格对比 (产生相反决策的最典型两极 MBTI)
void findBiggestSplit(const DecisionResult results[MBTI_COUNT], MBTIType& yesType, MBTIType& noType);

// 获取 Decision 的英文/中文文本名称
const char* getDecisionName(Decision decision);
const char* getDecisionNameCN(Decision decision);

// 获取 Decision 依据理由的中文翻译 (具备 16 种 MBTI 个性化心理学依据细化引擎)
const char* getDecisionReasonCN(const char* reasonEN, MBTIType personality = MBTIType::INTP, Decision decision = Decision::YES);

#endif // DECISION_ENGINE_H
