#include "DecisionEngine.h"
#include <cmath>

static float clampScore(float score) {
    if (score < 0.0f) return 0.0f;
    if (score > 100.0f) return 100.0f;
    return score;
}

DecisionResult simulateMBTI(MBTIType personality, const Scenario& scenario) {
    const PersonalityProfile& profile = getMBTIProfile(personality);

    // 向量加权决策意愿算法
    float score = 50.0f;

    // 1. 风险与不确定性
    score += (profile.risk - scenario.risk) * 0.20f;
    score -= (scenario.uncertainty * (100.0f - profile.planning) / 100.0f) * 0.15f;

    // 2. 新奇度与体验价值
    score += (profile.novelty * scenario.novelty / 100.0f) * 0.25f;
    score += (profile.novelty * scenario.emotionalValue / 100.0f) * 0.15f;

    // 3. 社交与精力付出
    score += (profile.social * scenario.social / 100.0f) * 0.20f;
    score -= (scenario.effort * (100.0f - profile.social) / 100.0f) * 0.10f;

    // 4. 代价与实用价值
    score += (profile.practicality * scenario.practicalValue / 100.0f) * 0.20f;
    score -= (scenario.cost * (100.0f - profile.practicality) / 100.0f) * 0.15f;

    // 5. 逻辑与时间代价
    score += (profile.logic * 10.0f / 100.0f);
    score -= (scenario.time * (100.0f - profile.logic) / 100.0f) * 0.10f;

    score = clampScore(score);

    Decision decision;
    if (score >= 60.0f) {
        decision = Decision::YES;
    } else if (score <= 40.0f) {
        decision = Decision::NO;
    } else {
        decision = Decision::MAYBE;
    }

    const char* reason = "综合评估个人偏好与场景风险";
    if (scenario.risk > 60.0f && profile.risk < 40.0f) {
        reason = "风险过高，超出安全边界";
    } else if (scenario.novelty > 70.0f && profile.novelty > 70.0f) {
        reason = "极为新奇，高度契合探险渴望";
    } else if (scenario.social > 70.0f && profile.social < 30.0f) {
        reason = "社交消耗过大，偏好独立自主";
    } else if (scenario.practicalValue > 70.0f && profile.practicality > 70.0f) {
        reason = "务实高效，具备极高实际价值";
    }

    return { personality, decision, score, reason };
}

void simulateAll(const Scenario& scenario, DecisionResult results[MBTI_COUNT]) {
    for (int i = 0; i < MBTI_COUNT; ++i) {
        results[i] = simulateMBTI(static_cast<MBTIType>(i), scenario);
    }
}

DecisionSummary summarizeResults(const DecisionResult results[MBTI_COUNT]) {
    DecisionSummary summary = { 0, 0, 0 };
    for (int i = 0; i < MBTI_COUNT; ++i) {
        if (results[i].decision == Decision::YES) {
            summary.yesCount++;
        } else if (results[i].decision == Decision::NO) {
            summary.noCount++;
        } else {
            summary.maybeCount++;
        }
    }
    return summary;
}

void findBiggestSplit(const DecisionResult results[MBTI_COUNT], MBTIType& yesType, MBTIType& noType) {
    float maxYesScore = -1.0f;
    float minNoScore = 101.0f;
    yesType = MBTIType::ENFP;
    noType = MBTIType::ISTJ;

    for (int i = 0; i < MBTI_COUNT; ++i) {
        if (results[i].decision == Decision::YES && results[i].score > maxYesScore) {
            maxYesScore = results[i].score;
            yesType = results[i].personality;
        }
        if (results[i].decision == Decision::NO && results[i].score < minNoScore) {
            minNoScore = results[i].score;
            noType = results[i].personality;
        }
    }
}

const char* getDecisionName(Decision decision) {
    switch (decision) {
        case Decision::YES: return "YES";
        case Decision::NO: return "NO";
        case Decision::MAYBE: return "MAYBE";
    }
    return "UNKNOWN";
}

const char* getDecisionNameCN(Decision decision) {
    switch (decision) {
        case Decision::YES: return "同意";
        case Decision::NO: return "拒绝";
        case Decision::MAYBE: return "犹豫";
    }
    return "未知";
}

const char* getDecisionReasonCN(const char* fallbackReason, MBTIType personality, Decision decision, DecisionType scenarioType) {
    if (scenarioType == DecisionType::GET) { // 购物/理财场景 (如六折便携设备 vs 攒钱预算)
        if (decision == Decision::YES) {
            switch (personality) {
                case MBTIType::ENFP: return "六折限定诱惑太大，对新奇好物毫无抵抗力";
                case MBTIType::ESTP: return "抓取即时冲动满足感，千金难买心头好";
                case MBTIType::ENTP: return "看重限定科技感，当作投资自己探索新玩意";
                case MBTIType::ESFP: return "折后性价比极高，能带来当下极高的情绪快乐";
                case MBTIType::ENTJ: return "评估设备对效率的提升价值，果断下订单";
                case MBTIType::ISTP: return "研究过设备参数，打折入手实操性价比高";
                default: return "限定优惠难得，享受消费喜悦";
            }
        } else if (decision == Decision::NO) {
            switch (personality) {
                case MBTIType::ISTJ: return "严守下半年攒钱计划，绝不为打折动用闲钱";
                case MBTIType::INTJ: return "非战略必需品，冲动消费会打破长远资金规划";
                case MBTIType::ISFJ: return "出于对已知开支的焦虑，优先保留存款安全感";
                case MBTIType::ESTJ: return "评估现有设备依然可用，拒绝无谓的预算浪费";
                case MBTIType::INFP: return "对物资没有强烈执念，更看重内心精神充实";
                default: return "遵守现有预算，理性克制冲动";
            }
        } else { // MAYBE
            switch (personality) {
                case MBTIType::INTP: return "在设备参数性能提升与预算消耗间反复建模纠结";
                case MBTIType::INFJ: return "内心在物质体验欲望与克制节约间拉锯观望";
                default: return "评估折扣力度与预算落差，犹豫观望";
            }
        }
    } else if (scenarioType == DecisionType::DO) { // 工作/求助场景 (如加班1.5小时协助核对 vs 晚餐约会)
        if (decision == Decision::YES) {
            switch (personality) {
                case MBTIType::ESTJ: return "高效协助核对，看重项目把控力与团队整体进度";
                case MBTIType::ENFJ: return "体恤同事紧急难处，促进团队和谐合作";
                case MBTIType::ENTJ: return "快速锁定掌控权，解决瓶颈确保项目交付";
                case MBTIType::ISFJ: return "热心贴心照顾同事求助，彰显责任担当";
                default: return "伸出援手协助，重视合作价值";
            }
        } else if (decision == Decision::NO) {
            switch (personality) {
                case MBTIType::INFP: return "极其珍视下班后个人精神独处，坚守私人边界";
                case MBTIType::ISTP: return "非职责范围内临时突发事，拒绝无谓加班消耗";
                case MBTIType::INTJ: return "临时插队打破了今晚个人计划，拒绝无序打扰";
                case MBTIType::ESFP: return "已有安排好的晚餐约会，不想扫了朋友的兴";
                default: return "保护私人休息时间，守住边界";
            }
        } else { // MAYBE
            switch (personality) {
                case MBTIType::INFJ: return "既体恤同事求助难处，又纠结已约好的私人晚餐";
                case MBTIType::INTP: return "评估协助所需实际耗时，犹豫是否值得加班";
                default: return "平衡人情求助与私人约会，摇摆不定";
            }
        }
    } else { // 旅行/探险场景 (如明天去东京特价机票 vs 机票自理早起)
        if (decision == Decision::YES) {
            switch (personality) {
                case MBTIType::ENFP: return "极具生活热忱与灵感，对突如其来的远方说走就走";
                case MBTIType::ENTP: return "明早6点出发更显探险刺激，摆脱常规框架索性一试";
                case MBTIType::ESTP: return "果断抓取眼前即时机会，享受不可预知的新鲜感";
                default: return "拥抱未知旅程，享受即兴惊喜";
            }
        } else if (decision == Decision::NO) {
            switch (personality) {
                case MBTIType::INTJ: return "未提前制定详尽旅行路线与时间表，拒绝无秩序仓促";
                case MBTIType::ISTJ: return "自费机票与早起成本较高，缺乏已知规划保障";
                case MBTIType::ISFJ: return "仓促出行带来不确定焦虑，偏好既有周六居家安排";
                default: return "偏好既定计划，规避无准备风险";
            }
        } else { // MAYBE
            switch (personality) {
                case MBTIType::INFJ: return "渴望去未曾体验的远方，但又在早起成本与安逸间纠结";
                case MBTIType::INTP: return "理性计算往返费用与行程收益，陷入推导等待";
                default: return "权衡行程精彩度与自理成本，犹豫观望";
            }
        }
    }

    return fallbackReason ? fallbackReason : "综合权衡个人偏好";
}
