#ifndef SCENARIO_MAPPER_H
#define SCENARIO_MAPPER_H

#include "Scenario.h"
#include "ScenarioBuilder.h"

// 将 UserSelection 自然选择映射为 Phase 1 抽象 Scenario 数据
Scenario buildScenario(const UserSelection& selection);

#endif // SCENARIO_MAPPER_H
