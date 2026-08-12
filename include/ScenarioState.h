#ifndef SCENARIO_STATE_H
#define SCENARIO_STATE_H

#include <cstdint>

enum class TransportMode {
    UNKNOWN = 0,
    SELF_DRIVE,
    FLIGHT,
    TRAIN,
    BUS,
    WALK
};

enum class CostType {
    NONE = 0,
    FUEL,
    TOLL,
    PARKING,
    FLIGHT_TICKET,
    TRAIN_TICKET,
    BUDGET,
    TIME,
    OVERTIME
};

enum class ExistingPlan {
    NONE = 0,
    STAY_HOME,
    WORK,
    TRAVEL,
    REST
};

struct ScenarioFact {
    TransportMode transportMode;
    CostType costType;
    ExistingPlan existingPlan;
};

struct ScenarioState {
    TransportMode transportMode;
    CostType costType;
    ExistingPlan existingPlan;

    bool isTransportLocked;
    bool isCostLocked;
    bool isPlanLocked;

    void reset() {
        transportMode = TransportMode::UNKNOWN;
        costType = CostType::NONE;
        existingPlan = ExistingPlan::NONE;

        isTransportLocked = false;
        isCostLocked = false;
        isPlanLocked = false;
    }

    void applyProvides(const ScenarioFact& provides) {
        if (!isTransportLocked && provides.transportMode != TransportMode::UNKNOWN) {
            transportMode = provides.transportMode;
            isTransportLocked = true;
        }
        if (!isCostLocked && provides.costType != CostType::NONE) {
            costType = provides.costType;
            isCostLocked = true;
        }
        if (!isPlanLocked && provides.existingPlan != ExistingPlan::NONE) {
            existingPlan = provides.existingPlan;
            isPlanLocked = true;
        }
    }
};

#endif // SCENARIO_STATE_H
