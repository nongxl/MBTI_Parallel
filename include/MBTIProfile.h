#ifndef MBTI_PROFILE_H
#define MBTI_PROFILE_H

enum class MBTIType {
    ISTJ = 0,
    ISFJ,
    INFJ,
    INTJ,
    ISTP,
    ISFP,
    INFP,
    INTP,
    ESTP,
    ESFP,
    ENFP,
    ENTP,
    ESTJ,
    ESFJ,
    ENFJ,
    ENTJ
};

constexpr int MBTI_COUNT = 16;

struct PersonalityProfile {
    MBTIType type;
    float risk;         // 0 ~ 100 风险接受程度
    float novelty;      // 0 ~ 100 新奇/未知偏好
    float logic;        // 0 ~ 100 理性/逻辑分析倾向
    float social;       // 0 ~ 100 社交/他人因素权重
    float planning;     // 0 ~ 100 计划/确定性倾向
    float practicality; // 0 ~ 100 实用/效率倾向
};

const char* getMBTIName(MBTIType type);
const PersonalityProfile& getMBTIProfile(MBTIType type);

#endif // MBTI_PROFILE_H
