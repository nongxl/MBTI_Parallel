#include "MBTIProfile.h"

static const char* MBTI_NAMES[MBTI_COUNT] = {
    "ISTJ", "ISFJ", "INFJ", "INTJ",
    "ISTP", "ISFP", "INFP", "INTP",
    "ESTP", "ESFP", "ENFP", "ENTP",
    "ESTJ", "ESFJ", "ENFJ", "ENTJ"
};

// 16 种 MBTI 的基准行为倾向配置
static const PersonalityProfile PROFILES[MBTI_COUNT] = {
    // type,  risk, novelty, logic, social, planning, practicality
    { MBTIType::ISTJ, 15.0f, 20.0f, 85.0f, 25.0f, 90.0f, 90.0f },
    { MBTIType::ISFJ, 20.0f, 25.0f, 35.0f, 75.0f, 85.0f, 75.0f },
    { MBTIType::INFJ, 30.0f, 70.0f, 40.0f, 80.0f, 80.0f, 35.0f },
    { MBTIType::INTJ, 40.0f, 80.0f, 90.0f, 20.0f, 95.0f, 60.0f },

    { MBTIType::ISTP, 75.0f, 50.0f, 85.0f, 20.0f, 20.0f, 85.0f },
    { MBTIType::ISFP, 45.0f, 60.0f, 30.0f, 60.0f, 25.0f, 45.0f },
    { MBTIType::INFP, 40.0f, 85.0f, 25.0f, 70.0f, 20.0f, 25.0f },
    { MBTIType::INTP, 55.0f, 90.0f, 95.0f, 15.0f, 15.0f, 40.0f },

    { MBTIType::ESTP, 90.0f, 75.0f, 80.0f, 65.0f, 15.0f, 85.0f },
    { MBTIType::ESFP, 80.0f, 85.0f, 25.0f, 90.0f, 15.0f, 50.0f },
    { MBTIType::ENFP, 75.0f, 95.0f, 30.0f, 85.0f, 20.0f, 30.0f },
    { MBTIType::ENTP, 85.0f, 95.0f, 90.0f, 55.0f, 10.0f, 40.0f },

    { MBTIType::ESTJ, 25.0f, 25.0f, 85.0f, 60.0f, 90.0f, 95.0f },
    { MBTIType::ESFJ, 20.0f, 30.0f, 30.0f, 90.0f, 85.0f, 80.0f },
    { MBTIType::ENFJ, 35.0f, 75.0f, 35.0f, 95.0f, 80.0f, 45.0f },
    { MBTIType::ENTJ, 65.0f, 75.0f, 90.0f, 50.0f, 90.0f, 85.0f }
};

const char* getMBTIName(MBTIType type) {
    int index = static_cast<int>(type);
    if (index >= 0 && index < MBTI_COUNT) {
        return MBTI_NAMES[index];
    }
    return "UNKNOWN";
}

const PersonalityProfile& getMBTIProfile(MBTIType type) {
    int index = static_cast<int>(type);
    if (index >= 0 && index < MBTI_COUNT) {
        return PROFILES[index];
    }
    return PROFILES[0];
}
