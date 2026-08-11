#ifndef DECISION_PROFILE_H
#define DECISION_PROFILE_H

#include "Scenario.h"
#include "DecisionEngine.h"
#include "MBTIProfile.h"
#include <cstdint>

struct DecisionProfile {
    float risk;         // 0 ~ 100
    float novelty;      // 0 ~ 100
    float logic;        // 0 ~ 100
    float social;       // 0 ~ 100
    float planning;     // 0 ~ 100
    float practicality; // 0 ~ 100
};

// 长效用户决策历史统计结构
struct UserHistoryStats {
    uint16_t totalPlays;        // 累计做过的决策总数
    uint16_t yesCount;          // 选择 YES 次数
    uint16_t noCount;           // 选择 NO 次数
    uint16_t maybeCount;        // 选择 MAYBE 次数

    // 6 维长效累积极坐标均值 (0.0 ~ 100.0)
    float cumulativeRisk;
    float cumulativeNovelty;
    float cumulativeLogic;
    float cumulativeSocial;
    float cumulativePlanning;
    float cumulativePracticality;

    MBTIType dominantMBTI;      // 长效收敛得出的最吻合 MBTI 人格
    float dominantSimilarity;   // 长效 MBTI 匹配度 (%)
};

// 单次决策轮廓计算
DecisionProfile calculateDecisionProfile(const Scenario& scenario, Decision userChoice);

// 计算 6D Profile 与 16 MBTI 的欧氏距离，寻找最相近的人格
MBTIType findClosestMBTI(const DecisionProfile& userProfile, float& outSimilarity);

// NVS 决策历史持久化读写与移动平均收敛算法
void loadUserHistoryFromNVS(UserHistoryStats& stats);
void saveUserHistoryToNVS(const UserHistoryStats& stats);
void recordUserDecisionToHistory(UserHistoryStats& stats, const DecisionProfile& currentProfile, Decision userChoice);

#endif // DECISION_PROFILE_H
