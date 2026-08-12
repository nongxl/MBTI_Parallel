#ifndef DECISION_ENGINE_H
#define DECISION_ENGINE_H

#include "Scenario.h"
#include "MBTIProfile.h"

enum class Decision {
    YES,
    NO,
    MAYBE
};

struct DecisionResult {
    MBTIType personality;
    Decision decision;
    float score; // 0 ~ 100 意愿得分
    const char* reason;
};

struct DecisionSummary {
    int yesCount;
    int noCount;
    int maybeCount;
};

// 模拟单个 MBTI 对场景的决策
DecisionResult simulateMBTI(MBTIType personality, const Scenario& scenario);

// 模拟所有 16 种 MBTI
void simulateAll(const Scenario& scenario, DecisionResult results[MBTI_COUNT]);

// 汇总统计结果
DecisionSummary summarizeResults(const DecisionResult results[MBTI_COUNT]);

// 查找分歧最大的两个人格
void findBiggestSplit(const DecisionResult results[MBTI_COUNT], MBTIType& yesType, MBTIType& noType);

// 获取决策英文与中文名称
const char* getDecisionName(Decision decision);
const char* getDecisionNameCN(Decision decision);

// 【Phase 6B 具象化升级】获取特定 MBTI 针对具体故事因素的考量依据
const char* getDecisionReasonCN(const char* fallbackReason, MBTIType personality, Decision decision, DecisionType scenarioType = DecisionType::GO);

#endif // DECISION_ENGINE_H
