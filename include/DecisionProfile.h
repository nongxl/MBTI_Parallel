#ifndef DECISION_PROFILE_H
#define DECISION_PROFILE_H

#include "Scenario.h"
#include "MBTIProfile.h"
#include "DecisionEngine.h"

struct DecisionProfile {
    float risk;         // 0 ~ 100
    float novelty;      // 0 ~ 100
    float logic;        // 0 ~ 100
    float social;       // 0 ~ 100
    float planning;     // 0 ~ 100
    float practicality; // 0 ~ 100
};

// 根据 Scenario 与用户的选择计算用户在该场景下的决策倾向 Profile
DecisionProfile calculateDecisionProfile(const Scenario& scenario, Decision choice);

// 计算 DecisionProfile 与 MBTI PersonalityProfile 之间的六维欧氏相似度 (0% ~ 100%)
float calculateSimilarity(const DecisionProfile& dProfile, const PersonalityProfile& pProfile);

// 查找匹配度最高的人格类型
MBTIType findClosestMBTI(const DecisionProfile& dProfile, float& outSimilarity);

#endif // DECISION_PROFILE_H
