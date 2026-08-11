#ifdef CHANGE
#undef CHANGE
#endif

#include "ScenarioPool.h"
#include <cstdlib>

#ifdef ARDUINO
#include <Arduino.h>
#else
static uint32_t randomNum() { return 12345; }
#endif

#ifdef CHANGE
#undef CHANGE
#endif

static const ScenarioTemplate SCENARIO_POOL[SCENARIO_POOL_SIZE] = {
    // id 0
    {
        0, "TRY SOMETHING NEW",
        "Engage in a completely unfamiliar activity today.",
        ScenarioCategory::EXPERIENCE,
        { DecisionType::DO, 50.0f, 30.0f, 50.0f, 90.0f, 30.0f, 60.0f, 55.0f, 85.0f, 30.0f }
    },
    // id 1
    {
        1, "UNEXPECTED OUTING",
        "A friend invites you out for a late night plan.",
        ScenarioCategory::SOCIAL,
        { DecisionType::GO, 45.0f, 40.0f, 60.0f, 75.0f, 85.0f, 50.0f, 50.0f, 75.0f, 25.0f }
    },
    // id 2
    {
        2, "BUY DESIRED GADGET",
        "Purchase the premium item you've been wanting for months.",
        ScenarioCategory::MONEY,
        { DecisionType::GET, 20.0f, 85.0f, 10.0f, 65.0f, 15.0f, 10.0f, 10.0f, 90.0f, 40.0f }
    },
    // id 3
    {
        3, "CHANGE NORMAL ROUTE",
        "Take a completely different path home today.",
        ScenarioCategory::CHANGE,
        { DecisionType::CHANGE, 30.0f, 10.0f, 30.0f, 80.0f, 20.0f, 30.0f, 65.0f, 70.0f, 20.0f }
    },
    // id 4
    {
        4, "LAST-MINUTE OPPORTUNITY",
        "Accept a sudden, high-risk high-reward chance.",
        ScenarioCategory::RISK,
        { DecisionType::ACCEPT, 85.0f, 50.0f, 70.0f, 85.0f, 40.0f, 70.0f, 80.0f, 70.0f, 85.0f }
    },
    // id 5
    {
        5, "START A NEW HOBBY",
        "Invest time and effort into learning a new skill.",
        ScenarioCategory::CREATIVITY,
        { DecisionType::DO, 30.0f, 45.0f, 70.0f, 85.0f, 25.0f, 75.0f, 35.0f, 80.0f, 40.0f }
    },
    // id 6
    {
        6, "SAY YES TO IMPULSE",
        "Agree to an unplanned event without research.",
        ScenarioCategory::UNCERTAINTY,
        { DecisionType::ACCEPT, 65.0f, 35.0f, 40.0f, 80.0f, 60.0f, 40.0f, 85.0f, 65.0f, 25.0f }
    },
    // id 7
    {
        7, "DAY WITHOUT A PLAN",
        "Spend an entire weekend day completely unplanned.",
        ScenarioCategory::TIME,
        { DecisionType::WAIT, 25.0f, 20.0f, 80.0f, 70.0f, 20.0f, 20.0f, 70.0f, 75.0f, 30.0f }
    },
    // id 8
    {
        8, "GO SOMEWHERE ALONE",
        "Travel or dine at a new location strictly alone.",
        ScenarioCategory::SOCIAL,
        { DecisionType::GO, 40.0f, 35.0f, 50.0f, 75.0f, 10.0f, 30.0f, 40.0f, 75.0f, 35.0f }
    },
    // id 9
    {
        9, "INVITE NEW FRIEND",
        "Reach out and invite someone you barely know.",
        ScenarioCategory::SOCIAL,
        { DecisionType::SAY, 50.0f, 30.0f, 40.0f, 70.0f, 90.0f, 50.0f, 55.0f, 75.0f, 30.0f }
    },
    // id 10
    {
        10, "TAKE UNFAMILIAR OPTION",
        "Pick the option you usually avoid in a menu or list.",
        ScenarioCategory::EXPERIENCE,
        { DecisionType::CHOOSE, 45.0f, 25.0f, 20.0f, 85.0f, 15.0f, 20.0f, 50.0f, 70.0f, 35.0f }
    },
    // id 11
    {
        11, "BREAK NORMAL ROUTINE",
        "Ditch your daily habits and try something random.",
        ScenarioCategory::CHANGE,
        { DecisionType::CHANGE, 55.0f, 20.0f, 40.0f, 85.0f, 30.0f, 40.0f, 75.0f, 70.0f, 30.0f }
    },
    // id 12
    {
        12, "TRY RISKY CHOICE",
        "Choose the riskier option for potential higher gains.",
        ScenarioCategory::RISK,
        { DecisionType::RISK, 85.0f, 60.0f, 50.0f, 75.0f, 30.0f, 60.0f, 80.0f, 60.0f, 85.0f }
    },
    // id 13
    {
        13, "FOLLOW FIRST INSTINCT",
        "Make a immediate choice based purely on intuition.",
        ScenarioCategory::CREATIVITY,
        { DecisionType::CHOOSE, 60.0f, 30.0f, 15.0f, 70.0f, 25.0f, 20.0f, 70.0f, 80.0f, 30.0f }
    },
    // id 14
    {
        14, "ACT NOW OR WAIT",
        "Take immediate action instead of careful planning.",
        ScenarioCategory::TIME,
        { DecisionType::DO, 70.0f, 40.0f, 60.0f, 65.0f, 35.0f, 70.0f, 75.0f, 55.0f, 70.0f }
    },
    // id 15
    {
        15, "EXPERIENCE OVER COMFORT",
        "Step out of your comfort zone for a unique memory.",
        ScenarioCategory::EXPERIENCE,
        { DecisionType::GO, 60.0f, 50.0f, 70.0f, 95.0f, 50.0f, 65.0f, 65.0f, 90.0f, 25.0f }
    },
    // id 16
    {
        16, "JUST FOR FUN",
        "Do something entertaining with zero practical gain.",
        ScenarioCategory::CREATIVITY,
        { DecisionType::DO, 30.0f, 35.0f, 50.0f, 80.0f, 45.0f, 40.0f, 30.0f, 95.0f, 10.0f }
    },
    // id 17
    {
        17, "TRY AVOIDED OPTION",
        "Face a choice you normally dislike or avoid.",
        ScenarioCategory::CHANGE,
        { DecisionType::DO, 65.0f, 30.0f, 60.0f, 60.0f, 30.0f, 70.0f, 60.0f, 50.0f, 65.0f }
    },
    // id 18
    {
        18, "CHANGE WEEKEND PLAN",
        "Completely overwrite your scheduled weekend.",
        ScenarioCategory::CHANGE,
        { DecisionType::CHANGE, 50.0f, 40.0f, 70.0f, 80.0f, 60.0f, 50.0f, 70.0f, 75.0f, 30.0f }
    },
    // id 19
    {
        19, "SAY REAL THOUGHTS",
        "Be 100% candid and express your true opinions.",
        ScenarioCategory::SOCIAL,
        { DecisionType::SAY, 55.0f, 10.0f, 20.0f, 40.0f, 85.0f, 40.0f, 45.0f, 85.0f, 40.0f }
    }
};

const ScenarioTemplate& getRandomScenario(int recentIds[3]) {
#ifdef ARDUINO
    int candidate = random(0, SCENARIO_POOL_SIZE);
#else
    static int dummyCount = 0;
    dummyCount++;
    int candidate = dummyCount % SCENARIO_POOL_SIZE;
#endif

    // 防重复算法：检查 candidate 是否在前 3 次的历史中
    for (int attempts = 0; attempts < SCENARIO_POOL_SIZE; ++attempts) {
        bool isRecent = false;
        for (int k = 0; k < 3; ++k) {
            if (recentIds[k] == candidate) {
                isRecent = true;
                break;
            }
        }
        if (!isRecent) {
            break; // 找到不重复的候选 id！
        }
        candidate = (candidate + 1) % SCENARIO_POOL_SIZE;
    }

    return SCENARIO_POOL[candidate];
}
