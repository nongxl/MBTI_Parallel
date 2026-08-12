#ifndef DECISION_MECHANISM_H
#define DECISION_MECHANISM_H

#include <cstdint>

enum class DecisionMechanism {
    OPPORTUNITY_COST = 0,     // 机会成本 (选 A 无法做 B)
    UNCERTAINTY,              // 信息不足与探险
    REVERSIBILITY,            // 决定可逆性 (随时退出 vs 无法反悔)
    SOCIAL_EXPECTATION,       // 人际期待与社会规范
    PERSONAL_CONVENIENCE,     // 个人舒适度
    FAIRNESS,                 // 公平性与边界
    TRUST,                    // 信任与验证
    COMMITMENT,               // 持续承诺成本
    IDENTITY,                 // 认同感与自我塑造
    RECIPROCITY,              // 人情与回报
    BOUNDARY,                 // 个人边界
    QUALITY_VS_QUANTITY,      // 品质 vs 数量
    SHORT_TERM_VS_LONG_TERM,  // 短期收益 vs 长期效益
    PRINCIPLE_VS_CONVENIENCE, // 原则 vs 便利
    RESOURCE_ALLOCATION,      // 时间/精力/资源分配
    DISCOVERY,                // 自我观察型决策 (如发现低效/旧设备)
    THRESHOLD,                // 个人替换/放弃阈值
    LOW_STAKES_DAILY          // 日常轻度微抉择 (如咖啡/小物品)
};

enum class DecisionShape {
    YES_NO = 0,
    BUY_PASS,
    HELP_LEAVE,
    GO_STAY,
    TRY_WAIT,
    REPAIR_REPLACE,
    USE_SAVE
};

struct DecisionSignature {
    uint8_t categoryId;
    DecisionMechanism mechanism;
    DecisionShape shape;

    bool equals(const DecisionSignature& other) const {
        return categoryId == other.categoryId &&
               mechanism == other.mechanism &&
               shape == other.shape;
    }
};

void initMechanismEngine();
bool isSignatureCoolingDown(const DecisionSignature& sig);
void markSignatureUsed(const DecisionSignature& sig);

#endif // DECISION_MECHANISM_H
