#ifndef SCENARIO_BUILDER_H
#define SCENARIO_BUILDER_H

#include "Scenario.h"

// 旧版本选词类型 (保持兼容)
enum class Motivation { WANT, NEED, CURIOUS, FUN, OPPORTUNITY, PEOPLE, CHANGE };
enum class Concern { RISK, COST, TIME, EFFORT, PEOPLE, UNKNOWN, NONE };
enum class Intensity { LOW, MEDIUM, HIGH };
enum class Priority { EXPERIENCE, PRACTICAL, PEOPLE, SAFETY };

struct UserSelection {
    DecisionType decisionType;
    Motivation motivation;
    Concern concern;
    Intensity intensity;
    Priority priority;
};

// Phase 4.x 四步构造器类型
// Step 1: WHO (谁？)
enum class WhoType {
    FRIEND = 0,
    COWORKER,
    STRANGER,
    GROUP,
    MYSELF,
    ACQUAINTANCE
};

// Step 2: SITUATION (情境？)
enum class SituationType {
    TRAVEL = 0,
    INVITATION,
    PURCHASE,
    HELP,
    HOBBY,
    CHALLENGE
};

// Step 3: CONDITION (特殊条件？)
enum class ConditionType {
    LAST_MINUTE = 0,
    UNKNOWN,
    EXPENSIVE,
    RISKY,
    NO_PLAN,
    UNEXPECTED
};

// Step 4: TENSION (纠结什么？/ 冲突)
enum class TensionType {
    SAFETY_VS_NOVELTY = 0,
    PLAN_VS_IMPROVISE,
    SAVE_VS_ENJOY,
    ALONE_VS_SOCIAL,
    CERTAIN_VS_UNKNOWN,
    EASY_VS_CHALLENGE
};

struct CustomScenarioDNA {
    WhoType who;
    SituationType situation;
    ConditionType condition;
    TensionType tension;
};

struct RenderedCustomScenario {
    CustomScenarioDNA dna;
    char titleEN[32];
    char descEN[80];
    char titleCN[32];
    char descCN[80];
    Scenario scenario;
};

// 根据 4 步选择构造 CustomScenario
RenderedCustomScenario renderCustomScenario(const CustomScenarioDNA& dna);

// 选项双语名称 Getter
const char* getWhoName(WhoType who, bool isCN);
const char* getSituationName(SituationType sit, bool isCN);
const char* getConditionName(ConditionType cond, bool isCN);
const char* getTensionName(TensionType ten, bool isCN);

#endif // SCENARIO_BUILDER_H
