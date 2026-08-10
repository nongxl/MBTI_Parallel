#include "ScenarioBuilder.h"

const char* getDecisionTypeName(DecisionType type) {
    switch (type) {
        case DecisionType::GET: return "GET";
        case DecisionType::GO: return "GO";
        case DecisionType::DO: return "DO";
        case DecisionType::SAY: return "SAY";
        case DecisionType::CHOOSE: return "CHOOSE";
        case DecisionType::KEEP: return "KEEP";
        case DecisionType::QUIT: return "QUIT";
        case DecisionType::CHANGE: return "CHANGE";
        case DecisionType::ACCEPT: return "ACCEPT";
        case DecisionType::REFUSE: return "REFUSE";
        case DecisionType::WAIT: return "WAIT";
        case DecisionType::RISK: return "RISK";
        default: return "DECIDE";
    }
}

const char* getMotivationName(Motivation motivation) {
    switch (motivation) {
        case Motivation::WANT: return "WANT";
        case Motivation::NEED: return "NEED";
        case Motivation::CURIOUS: return "CURIOUS";
        case Motivation::OPPORTUNITY: return "OPPORTUNITY";
        case Motivation::FUN: return "FUN";
        case Motivation::PEOPLE: return "PEOPLE";
        case Motivation::CHANGE: return "CHANGE";
        default: return "UNKNOWN";
    }
}

const char* getConcernName(Concern concern) {
    switch (concern) {
        case Concern::RISK: return "RISK";
        case Concern::COST: return "COST";
        case Concern::TIME: return "TIME";
        case Concern::EFFORT: return "EFFORT";
        case Concern::PEOPLE: return "PEOPLE";
        case Concern::UNKNOWN: return "UNKNOWN";
        case Concern::NONE: return "NONE";
        default: return "NONE";
    }
}

const char* getIntensityName(Intensity intensity) {
    switch (intensity) {
        case Intensity::LOW: return "LOW";
        case Intensity::MEDIUM: return "MEDIUM";
        case Intensity::HIGH: return "HIGH";
        default: return "MEDIUM";
    }
}

const char* getPriorityName(Priority priority) {
    switch (priority) {
        case Priority::EXPERIENCE: return "EXPERIENCE";
        case Priority::PRACTICAL: return "PRACTICAL";
        case Priority::PEOPLE: return "PEOPLE";
        case Priority::SAFETY: return "SAFETY";
        default: return "EXPERIENCE";
    }
}

// 预置 10 个轻量 RANDOM Scenario 库
static const RandomPreset RANDOM_PRESETS[RANDOM_PRESET_COUNT] = {
    {
        "LAST-MINUTE TRIP",
        "A friend invites you on an impulse weekend trip tomorrow.",
        { DecisionType::GO, 60.0f, 65.0f, 70.0f, 85.0f, 75.0f, 50.0f, 60.0f, 80.0f, 20.0f }
    },
    {
        "TRY A NEW HOBBY",
        "Start learning a completely unfamiliar skill this month.",
        { DecisionType::DO, 30.0f, 40.0f, 60.0f, 90.0f, 30.0f, 70.0f, 40.0f, 85.0f, 30.0f }
    },
    {
        "BUY SOMETHING DESIRED",
        "Buy a premium gadget you've wanted for months.",
        { DecisionType::GET, 20.0f, 85.0f, 10.0f, 70.0f, 15.0f, 10.0f, 10.0f, 90.0f, 40.0f }
    },
    {
        "JOIN UNFAMILIAR PARTY",
        "Attend a social gathering where you barely know anyone.",
        { DecisionType::GO, 45.0f, 20.0f, 40.0f, 65.0f, 95.0f, 60.0f, 50.0f, 60.0f, 15.0f }
    },
    {
        "CHANGE YOUR PLAN",
        "Completely scrap your weekend schedule for a new chance.",
        { DecisionType::CHANGE, 50.0f, 30.0f, 50.0f, 85.0f, 40.0f, 40.0f, 75.0f, 70.0f, 25.0f }
    },
    {
        "CAREER OPPORTUNITY",
        "Take on a high-risk high-reward project at work.",
        { DecisionType::ACCEPT, 80.0f, 30.0f, 80.0f, 75.0f, 45.0f, 85.0f, 70.0f, 40.0f, 90.0f }
    },
    {
        "TELL THE TRUTH",
        "Express your honest feelings to a close friend.",
        { DecisionType::SAY, 40.0f, 10.0f, 20.0f, 30.0f, 90.0f, 30.0f, 40.0f, 85.0f, 30.0f }
    },
    {
        "EARLY LEAVE",
        "Notice boss is away. Leave office 30 minutes early?",
        { DecisionType::QUIT, 65.0f, 10.0f, 30.0f, 40.0f, 20.0f, 10.0f, 50.0f, 60.0f, 20.0f }
    },
    {
        "TRY STRANGE RESTAURANT",
        "Eat at an unusual non-reviewed local food spot.",
        { DecisionType::CHOOSE, 40.0f, 30.0f, 30.0f, 85.0f, 20.0f, 20.0f, 45.0f, 70.0f, 30.0f }
    },
    {
        "RECONNECT OLD FRIEND",
        "Message a school friend you haven't talked to in years.",
        { DecisionType::DO, 25.0f, 10.0f, 20.0f, 50.0f, 85.0f, 20.0f, 35.0f, 80.0f, 20.0f }
    }
};

const RandomPreset& getRandomPreset(int index) {
    if (index < 0 || index >= RANDOM_PRESET_COUNT) {
        return RANDOM_PRESETS[0];
    }
    return RANDOM_PRESETS[index];
}
