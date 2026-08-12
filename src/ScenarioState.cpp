#include "ScenarioState.h"
#include "EventFragment.h"

bool canApplyFragment(const EventFragment& frag, const ScenarioState& state) {
    // 1. 检查 requires 依赖的前提事实
    if (frag.requiresFact.transportMode != TransportMode::UNKNOWN && frag.requiresFact.transportMode != state.transportMode) {
        return false;
    }
    if (frag.requiresFact.costType != CostType::NONE && frag.requiresFact.costType != state.costType) {
        return false;
    }
    if (frag.requiresFact.existingPlan != ExistingPlan::NONE && frag.requiresFact.existingPlan != state.existingPlan) {
        return false;
    }

    // 2. 检查 conflicts 冲突的事实 (如果 State 已经有某个锁定事实，且片段与其冲突)
    if (state.transportMode != TransportMode::UNKNOWN && frag.conflictsFact.transportMode == state.transportMode) {
        return false;
    }
    if (state.costType != CostType::NONE && frag.conflictsFact.costType == state.costType) {
        return false;
    }
    if (state.existingPlan != ExistingPlan::NONE && frag.conflictsFact.existingPlan == state.existingPlan) {
        return false;
    }

    // 3. 检查 provides 是否与当前已经锁定的 State 矛盾
    if (state.isTransportLocked && frag.providesFact.transportMode != TransportMode::UNKNOWN && frag.providesFact.transportMode != state.transportMode) {
        return false;
    }
    if (state.isCostLocked && frag.providesFact.costType != CostType::NONE && frag.providesFact.costType != state.costType) {
        return false;
    }
    if (state.isPlanLocked && frag.providesFact.existingPlan != ExistingPlan::NONE && frag.providesFact.existingPlan != state.existingPlan) {
        return false;
    }

    return true;
}
