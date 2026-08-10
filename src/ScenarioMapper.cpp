#include "ScenarioMapper.h"
#include <algorithm>

static float clampValue(float val) {
    if (val < 0.0f) return 0.0f;
    if (val > 100.0f) return 100.0f;
    return val;
}

Scenario buildScenario(const UserSelection& selection) {
    Scenario scenario;
    scenario.type = selection.decisionType;

    // 默认基础参数 (25~30 基础分)
    scenario.risk = 25.0f;
    scenario.cost = 25.0f;
    scenario.time = 25.0f;
    scenario.novelty = 25.0f;
    scenario.social = 25.0f;
    scenario.effort = 25.0f;
    scenario.uncertainty = 25.0f;
    scenario.emotionalValue = 30.0f;
    scenario.practicalValue = 30.0f;

    // 1. Intensity 转换数值
    float intensityVal = 50.0f;
    switch (selection.intensity) {
        case Intensity::LOW: intensityVal = 20.0f; break;
        case Intensity::MEDIUM: intensityVal = 50.0f; break;
        case Intensity::HIGH: intensityVal = 80.0f; break;
    }

    // 2. 根据 Concern 设置主要阻力与顾虑项
    switch (selection.concern) {
        case Concern::RISK:
            scenario.risk = intensityVal;
            scenario.uncertainty = intensityVal * 0.8f;
            break;
        case Concern::COST:
            scenario.cost = intensityVal;
            break;
        case Concern::TIME:
            scenario.time = intensityVal;
            break;
        case Concern::EFFORT:
            scenario.effort = intensityVal;
            break;
        case Concern::PEOPLE:
            scenario.social = intensityVal;
            break;
        case Concern::UNKNOWN:
            scenario.uncertainty = intensityVal;
            scenario.risk = intensityVal * 0.7f;
            break;
        case Concern::NONE:
            scenario.cost = 10.0f;
            scenario.risk = 10.0f;
            scenario.effort = 10.0f;
            scenario.uncertainty = 10.0f;
            break;
    }

    // 3. 根据 Motivation 加成驱动维度
    switch (selection.motivation) {
        case Motivation::WANT:
            scenario.emotionalValue += 40.0f;
            scenario.novelty += 20.0f;
            break;
        case Motivation::NEED:
            scenario.practicalValue += 50.0f;
            break;
        case Motivation::CURIOUS:
            scenario.novelty += 50.0f;
            scenario.risk += 15.0f;
            break;
        case Motivation::OPPORTUNITY:
            scenario.novelty += 40.0f;
            scenario.practicalValue += 30.0f;
            break;
        case Motivation::FUN:
            scenario.emotionalValue += 45.0f;
            scenario.novelty += 30.0f;
            break;
        case Motivation::PEOPLE:
            scenario.social += 45.0f;
            scenario.emotionalValue += 30.0f;
            break;
        case Motivation::CHANGE:
            scenario.novelty += 40.0f;
            scenario.uncertainty += 20.0f;
            break;
    }

    // 4. 根据 Priority (WHAT MATTERS MORE) 进行导向修正
    switch (selection.priority) {
        case Priority::EXPERIENCE:
            scenario.emotionalValue += 20.0f;
            scenario.novelty += 20.0f;
            break;
        case Priority::PRACTICAL:
            scenario.practicalValue += 30.0f;
            scenario.cost = std::max(0.0f, scenario.cost - 10.0f);
            break;
        case Priority::PEOPLE:
            scenario.social += 30.0f;
            break;
        case Priority::SAFETY:
            scenario.risk = std::max(0.0f, scenario.risk - 25.0f);
            scenario.uncertainty = std::max(0.0f, scenario.uncertainty - 25.0f);
            break;
    }

    // Clamp 到 [0, 100]
    scenario.risk = clampValue(scenario.risk);
    scenario.cost = clampValue(scenario.cost);
    scenario.time = clampValue(scenario.time);
    scenario.novelty = clampValue(scenario.novelty);
    scenario.social = clampValue(scenario.social);
    scenario.effort = clampValue(scenario.effort);
    scenario.uncertainty = clampValue(scenario.uncertainty);
    scenario.emotionalValue = clampValue(scenario.emotionalValue);
    scenario.practicalValue = clampValue(scenario.practicalValue);

    return scenario;
}
