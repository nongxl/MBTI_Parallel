#ifndef EVENT_FRAGMENT_H
#define EVENT_FRAGMENT_H

#include "ScenarioPool.h"
#include "Scenario.h"
#include "DecisionArchetype.h"
#include "ScenarioState.h"
#include <cstdint>

enum class FragmentType {
    SETUP = 0,
    EVENT,
    CONTEXT,
    CONSTRAINT,
    TWIST,
    DECISION_FRAME
};

struct EventFragment {
    const char* id;
    FragmentType type;
    const char* zh;
    const char* en;
    ArchetypeID compatibleArchetypes[4];
    int archetypeCount;
    const char* tags[4];
    int weight;
    int cooldown;

    // 【Phase 6C 事实元数据】
    ScenarioFact requiresFact;
    ScenarioFact providesFact;
    ScenarioFact conflictsFact;
};

enum class NarrativePatternID {
    PATTERN_SETUP_EVENT_CONSTRAINT = 0, // SETUP + EVENT + CONSTRAINT + DECISION
    PATTERN_EVENT_CONTEXT,             // EVENT + CONTEXT + DECISION
    PATTERN_FULL_STORY,                // SETUP + EVENT + CONTEXT + CONSTRAINT + DECISION
    PATTERN_EVENT_TWIST                // EVENT + TWIST + DECISION
};

struct AssembledStoryScenario {
    char scenarioId[24];
    ArchetypeID archetype;
    ScenarioCategory category;
    NarrativePatternID patternId;

    char titleCN[48];
    char bodyCN[512];
    char titleEN[48];
    char bodyEN[512];

    char choiceA_CN[24];
    char choiceB_CN[24];
    char choiceA_EN[24];
    char choiceB_EN[24];

    Scenario scenario;
    uint32_t seed;
};

// 检查某个 Fragment 是否在当前 ScenarioState 下合法兼容
bool canApplyFragment(const EventFragment& frag, const ScenarioState& state);

// 事件片段组装引擎核心公开函数
AssembledStoryScenario assembleFragmentScenario(ArchetypeID chosenArchetype);

#endif // EVENT_FRAGMENT_H
