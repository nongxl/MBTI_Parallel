#ifndef SCENARIO_H
#define SCENARIO_H

enum class DecisionType {
    GET,
    GO,
    DO,
    SAY,
    CHOOSE,
    KEEP,
    QUIT,
    CHANGE,
    ACCEPT,
    REFUSE,
    WAIT,
    RISK
};

struct Scenario {
    DecisionType type;

    float risk;           // 0 ~ 100 风险程度
    float cost;           // 0 ~ 100 资源/金钱代价
    float time;           // 0 ~ 100 时间投入
    float novelty;        // 0 ~ 100 新奇度/未知度
    float social;         // 0 ~ 100 社交影响/关注度
    float effort;         // 0 ~ 100 精力/体力付出
    float uncertainty;    // 0 ~ 100 不确定性/缺乏计划保障
    float emotionalValue; // 0 ~ 100 情绪/感受价值
    float practicalValue; // 0 ~ 100 实用/实际收益
};

#endif // SCENARIO_H
