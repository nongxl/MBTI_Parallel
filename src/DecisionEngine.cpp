#include "DecisionEngine.h"
#include <cmath>
#include <algorithm>
#include <cstring>

static float clampValue(float val, float minV = 0.0f, float maxV = 100.0f) {
    return std::max(minV, std::min(maxV, val));
}

DecisionResult evaluateDecision(const PersonalityProfile& profile, const Scenario& scenario) {
    DecisionResult res;
    res.personality = profile.type;

    // 六维综合权重加权算法
    float score = 50.0f; // 基准中立分

    score += (profile.risk - 50.0f) * (scenario.risk / 100.0f) * 0.25f;
    score += (profile.novelty - 50.0f) * (scenario.novelty / 100.0f) * 0.25f;
    score += (profile.social - 50.0f) * (scenario.social / 100.0f) * 0.20f;
    score += (profile.logic - 50.0f) * (scenario.practicalValue / 100.0f) * 0.15f;
    score -= (profile.planning - 50.0f) * (scenario.uncertainty / 100.0f) * 0.20f;
    score += (profile.practicality - 50.0f) * (scenario.practicalValue / 100.0f) * 0.15f;

    res.score = clampValue(score);

    // 确定 Decision 结果与决策依据
    if (res.score >= 58.0f) {
        res.decision = Decision::YES;
        if (profile.novelty >= 75.0f && scenario.novelty >= 50.0f) {
            res.reason = "NOVELTY SEEKING INSTINCT";
        } else if (profile.risk >= 70.0f && scenario.risk >= 50.0f) {
            res.reason = "HIGH RISK ACCEPTANCE";
        } else if (profile.social >= 70.0f && scenario.social >= 50.0f) {
            res.reason = "SOCIAL INVOLVEMENT PREFERENCE";
        } else if (profile.logic >= 70.0f) {
            res.reason = "ANALYTICAL LOGIC MATCH";
        } else {
            res.reason = "POSITIVE VALUE ALIGNMENT";
        }
    } else if (res.score <= 42.0f) {
        res.decision = Decision::NO;
        if (profile.planning >= 70.0f && scenario.uncertainty >= 40.0f) {
            res.reason = "STRUCTURED PLANNING NEEDED";
        } else if (profile.risk <= 35.0f && scenario.risk >= 50.0f) {
            res.reason = "CAUTIOUS RISK AVOIDANCE";
        } else if (profile.social <= 35.0f && scenario.social >= 60.0f) {
            res.reason = "SOLITUDE & FREEDOM PREFERRED";
        } else {
            res.reason = "STABILITY & SAFETY FIRST";
        }
    } else {
        res.decision = Decision::MAYBE;
        res.reason = "BALANCED WEIGHING OPTION";
    }

    return res;
}

void simulateAll(const Scenario& scenario, DecisionResult outResults[MBTI_COUNT]) {
    for (int i = 0; i < MBTI_COUNT; ++i) {
        const PersonalityProfile& prof = getMBTIProfile(static_cast<MBTIType>(i));
        outResults[i] = evaluateDecision(prof, scenario);
    }
}

DecisionSummary summarizeResults(const DecisionResult results[MBTI_COUNT]) {
    DecisionSummary summary = {0, 0, 0};
    for (int i = 0; i < MBTI_COUNT; ++i) {
        if (results[i].decision == Decision::YES) summary.yesCount++;
        else if (results[i].decision == Decision::NO) summary.noCount++;
        else summary.maybeCount++;
    }
    return summary;
}

void findBiggestSplit(const DecisionResult results[MBTI_COUNT], MBTIType& yesType, MBTIType& noType) {
    float maxYesScore = -1.0f;
    float minNoScore = 999.0f;
    yesType = MBTIType::ENTP;
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

// 【16 人格专属心理学依据映射矩阵】
// 即使做出完全相同的 Decision (如 YES)，不同 MBTI 人格从各自的认知功能 (Cognitive Functions) 出发，给出完全不重样的深层依据！
const char* getDecisionReasonCN(const char* reasonEN, MBTIType personality, Decision decision) {
    if (decision == Decision::YES) {
        switch (personality) {
            case MBTIType::INTP: return "追求底层原理自洽，边际收益模型清晰";
            case MBTIType::INTJ: return "符合远期战略布局，风险受控在模型内";
            case MBTIType::ENTP: return "打破既有思维框架，拥抱绝佳脑洞尝试";
            case MBTIType::ENTJ: return "符合高效目标达成，迅速锁定掌控权";
            case MBTIType::INFJ: return "洞察到深层人文价值，符合长远使命";
            case MBTIType::INFP: return "内心价值强烈共鸣，坚守纯粹个体信念";
            case MBTIType::ENFJ: return "促进团队深度凝聚，带来积极情绪共鸣";
            case MBTIType::ENFP: return "极具灵感与生活热忱，激发现场活力";
            case MBTIType::ISTJ: return "规章明确已有先例，按秩序稳健推进";
            case MBTIType::ISFJ: return "贴心照顾身边现实需求，关怀安全";
            case MBTIType::ESTJ: return "符合标准实用规范，务实落地效率高";
            case MBTIType::ESFJ: return "符合社会普遍期待，维护和谐人际礼仪";
            case MBTIType::ISTP: return "实操工具与技术可行，可随时灵活应对";
            case MBTIType::ISFP: return "顺应当下身心感受，追求艺术自由";
            case MBTIType::ESTP: return "果断抓取眼前即时机会，享受刺激破局";
            case MBTIType::ESFP: return "现场气氛高涨，抓住当下快乐体验";
        }
    } else if (decision == Decision::NO) {
        switch (personality) {
            case MBTIType::INTP: return "逻辑模型存在漏洞，缺乏底层理论支撑";
            case MBTIType::INTJ: return "无益于远期战略目标，存在不可控随机扰动";
            case MBTIType::ENTP: return "过于死板教条，缺乏创新脑洞空间";
            case MBTIType::ENTJ: return "执行效率低下且掌控力不足，拖慢进度";
            case MBTIType::INFJ: return "违背内在人文伦理，可能伤害群体感情";
            case MBTIType::INFP: return "违背个人核心价值观，无法违背内心意志";
            case MBTIType::ENFJ: return "可能引发团队内部对立，破坏整体和谐";
            case MBTIType::ENFP: return "过程枯燥乏味，剥夺了个体自由与热情";
            case MBTIType::ISTJ: return "缺乏已知可靠经验，规避规章外潜在风险";
            case MBTIType::ISFJ: return "打破现有稳定生活，带来未知焦虑风险";
            case MBTIType::ESTJ: return "缺乏实用落地价值，不符合高效管理标准";
            case MBTIType::ESFJ: return "可能违背传统规范，影响良好人际评价";
            case MBTIType::ISTP: return "限制个体行动自由，实操性价比过低";
            case MBTIType::ISFP: return "内心产生强烈排斥压迫感，违背身心舒适";
            case MBTIType::ESTP: return "缺乏即时反馈收益，沉闷无趣";
            case MBTIType::ESFP: return "氛围过于沉闷压抑，扫兴且不快乐";
        }
    } else { // MAYBE
        switch (personality) {
            case MBTIType::INTP: return "推导数据不足，需进一步建模论证";
            case MBTIType::INTJ: return "长远与短期利益存在冲突，评估博弈比重";
            case MBTIType::ENTP: return "方案虽有趣但细节待完善，观望替代方案";
            case MBTIType::ENTJ: return "资源投入产出比尚不明确，等待最佳时机";
            case MBTIType::INFJ: return "理性思考与感性直觉发生拉锯，反复体会";
            case MBTIType::INFP: return "理想期待与现实约束存在落差，陷入纠结";
            case MBTIType::ENFJ: return "试图兼顾各方诉求，寻找最大公约数";
            case MBTIType::ENFP: return "新想法层出不穷，在多个选择间摇摆";
            case MBTIType::ISTJ: return "需收集更多已知案例数据，谨慎比对";
            case MBTIType::ISFJ: return "既想维持现状稳定，又担心辜负他人期待";
            case MBTIType::ESTJ: return "评估既定规则与例外情况的平衡成本";
            case MBTIType::ESFJ: return "关注大众看法，等待更清晰的群体共识";
            case MBTIType::ISTP: return "观察现场动态变化，保留随时退出的自由";
            case MBTIType::ISFP: return "需要静心倾听内在声音，不想仓促决定";
            case MBTIType::ESTP: return "评估风险收益比，寻找回报率最高切入点";
            case MBTIType::ESFP: return "视当时现场气氛与同伴情绪而灵活决定";
        }
    }

    return "综合权衡个人偏好";
}
