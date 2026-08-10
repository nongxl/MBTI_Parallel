#ifndef DECISION_ENGINE_H
#define DECISION_ENGINE_H

#include "MBTIProfile.h"
#include "Scenario.h"

enum class Decision {
    YES,
    NO,
    MAYBE
};

constexpr float YES_THRESHOLD = 65.0f;
constexpr float NO_THRESHOLD = 35.0f;

struct DecisionResult {
    MBTIType personality;
    Decision decision;
    float score;        // 0 ~ 100
    float confidence;   // 0 ~ 50
    char reason[64];    // <= 60 字符的简短解释
};

struct DecisionSummary {
    int yesCount;
    int noCount;
    int maybeCount;

    MBTIType strongestYes;
    MBTIType strongestNo;
};

// 单个人格决策模拟
DecisionResult simulate(const Scenario& scenario, MBTIType personality);

// 批量模拟 16 人格
void simulateAll(const Scenario& scenario, DecisionResult results[MBTI_COUNT]);

// 汇总统计模拟结果
DecisionSummary summarizeResults(const DecisionResult results[MBTI_COUNT]);

// 寻找最大分歧人格 (Highest Score vs Lowest Score)
void findBiggestSplit(const DecisionResult results[MBTI_COUNT], MBTIType& yesType, MBTIType& noType);

// 辅助方法：决策枚举转字符串
const char* getDecisionName(Decision decision);

#endif // DECISION_ENGINE_H
