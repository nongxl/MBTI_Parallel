#ifndef SCENARIO_BUILDER_H
#define SCENARIO_BUILDER_H

#ifdef LOW
#undef LOW
#endif
#ifdef HIGH
#undef HIGH
#endif
#ifdef CHANGE
#undef CHANGE
#endif

#include "Scenario.h"

enum class Motivation {
    WANT,
    NEED,
    CURIOUS,
    OPPORTUNITY,
    FUN,
    PEOPLE,
    CHANGE
};

enum class Concern {
    RISK,
    COST,
    TIME,
    EFFORT,
    PEOPLE,
    UNKNOWN,
    NONE
};

enum class Intensity {
    LOW,        // 对应 20
    MEDIUM,     // 对应 50
    HIGH        // 对应 80
};

enum class Priority {
    EXPERIENCE,
    PRACTICAL,
    PEOPLE,
    SAFETY
};

struct UserSelection {
    DecisionType decisionType;
    Motivation motivation;
    Concern concern;
    Intensity intensity;
    Priority priority;
};

struct RandomPreset {
    const char* title;
    const char* description;
    Scenario scenario;
};

constexpr int RANDOM_PRESET_COUNT = 10;

const char* getDecisionTypeName(DecisionType type);
const char* getMotivationName(Motivation motivation);
const char* getConcernName(Concern concern);
const char* getIntensityName(Intensity intensity);
const char* getPriorityName(Priority priority);

const RandomPreset& getRandomPreset(int index);

#endif // SCENARIO_BUILDER_H
