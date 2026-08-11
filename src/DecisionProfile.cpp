#include "DecisionProfile.h"
#include <cmath>
#include <algorithm>

static float clampVal(float v) {
    if (v < 0.0f) return 0.0f;
    if (v > 100.0f) return 100.0f;
    return v;
}

DecisionProfile calculateDecisionProfile(const Scenario& scenario, Decision choice) {
    DecisionProfile profile;

    if (choice == Decision::YES) {
        profile.risk = clampVal(scenario.risk * 0.7f + (100.0f - scenario.uncertainty) * 0.3f);
        profile.novelty = clampVal(scenario.novelty);
        profile.logic = clampVal(scenario.practicalValue * 0.6f + (100.0f - scenario.emotionalValue) * 0.4f);
        profile.social = clampVal(scenario.social);
        profile.planning = clampVal(100.0f - scenario.uncertainty);
        profile.practicality = clampVal(scenario.practicalValue);
    } else if (choice == Decision::NO) {
        profile.risk = clampVal((100.0f - scenario.risk) * 0.7f + scenario.uncertainty * 0.3f);
        profile.novelty = clampVal(100.0f - scenario.novelty);
        profile.logic = clampVal((100.0f - scenario.practicalValue) * 0.5f + scenario.emotionalValue * 0.5f);
        profile.social = clampVal(100.0f - scenario.social);
        profile.planning = clampVal(scenario.uncertainty * 0.8f + scenario.risk * 0.2f);
        profile.practicality = clampVal(100.0f - scenario.practicalValue);
    } else { // MAYBE
        profile.risk = 50.0f;
        profile.novelty = 50.0f;
        profile.logic = 50.0f;
        profile.social = 50.0f;
        profile.planning = 50.0f;
        profile.practicality = 50.0f;
    }

    return profile;
}

float calculateSimilarity(const DecisionProfile& dProfile, const PersonalityProfile& pProfile) {
    float diffRisk = dProfile.risk - pProfile.risk;
    float diffNov  = dProfile.novelty - pProfile.novelty;
    float diffLog  = dProfile.logic - pProfile.logic;
    float diffSoc  = dProfile.social - pProfile.social;
    float diffPlan = dProfile.planning - pProfile.planning;
    float diffPrac = dProfile.practicality - pProfile.practicality;

    float sumSq = diffRisk * diffRisk + diffNov * diffNov + diffLog * diffLog +
                  diffSoc * diffSoc + diffPlan * diffPlan + diffPrac * diffPrac;

    float meanDist = std::sqrt(sumSq / 6.0f);
    
    // 映射到 [0, 100%]
    float similarity = 100.0f - (meanDist * 1.2f);
    return std::max(0.0f, std::min(100.0f, similarity));
}

MBTIType findClosestMBTI(const DecisionProfile& dProfile, float& outSimilarity) {
    float maxSim = -1.0f;
    MBTIType closestType = MBTIType::ISTJ;

    for (int i = 0; i < MBTI_COUNT; ++i) {
        MBTIType type = static_cast<MBTIType>(i);
        const PersonalityProfile& pProfile = getMBTIProfile(type);
        float sim = calculateSimilarity(dProfile, pProfile);
        if (sim > maxSim) {
            maxSim = sim;
            closestType = type;
        }
    }

    outSimilarity = maxSim;
    return closestType;
}
