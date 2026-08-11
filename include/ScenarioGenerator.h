#ifndef SCENARIO_GENERATOR_H
#define SCENARIO_GENERATOR_H

#ifdef LOW
#undef LOW
#endif
#ifdef HIGH
#undef HIGH
#endif
#ifdef CHANGE
#undef CHANGE
#endif

#include <cstdint>
#include "Scenario.h"

enum class ScenarioCategory {
    SOCIAL = 0,
    MONEY,
    EXPERIENCE,
    RISK,
    CHANGE,
    CREATIVITY,
    TIME,
    UNCERTAINTY,
    OPPORTUNITY,
    COMFORT,
    RELATIONSHIP,
    SELF
};

enum class DecisionConflict {
    SAFETY_VS_NOVELTY = 0,
    COMFORT_VS_EXPERIENCE,
    MONEY_VS_ENJOYMENT,
    PLANNING_VS_IMPROVISATION,
    LOGIC_VS_IMPULSE,
    CERTAINTY_VS_OPPORTUNITY,
    SOCIAL_VS_SOLITUDE,
    EFFICIENCY_VS_EXPERIENCE,
    CONTROL_VS_FREEDOM,
    FAMILIAR_VS_UNKNOWN
};

struct ScenarioDNA {
    ScenarioCategory category;
    DecisionConflict conflict;

    uint8_t riskLevel;         // 0 ~ 100
    uint8_t noveltyLevel;      // 0 ~ 100
    uint8_t socialLevel;       // 0 ~ 100
    uint8_t uncertaintyLevel; // 0 ~ 100
    uint8_t timePressure;     // 0 ~ 100
    uint8_t financialImpact;  // 0 ~ 100

    uint8_t contextId;
    uint8_t wordingVariant;
};

struct GeneratedScenario {
    ScenarioDNA dna;
    char title[32];
    char description[80];
    Scenario scenario;
    float qualityScore;
};

constexpr int RECENT_DNA_HISTORY_SIZE = 10;
constexpr int CANDIDATE_POOL_SIZE = 5;

// 相似度计算 (0% ~ 100%)
float calculateDNASimilarity(const ScenarioDNA& a, const ScenarioDNA& b);

// 离线程序化场景生成核心接口 (从 5 个 Candidates 中根据多样性与 MBTI 分歧度选优)
GeneratedScenario generateProceduralScenario(const ScenarioDNA recentDNAs[10], int historyCount);

#endif // SCENARIO_GENERATOR_H
