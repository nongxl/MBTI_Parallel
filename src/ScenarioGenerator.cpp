#ifdef LOW
#undef LOW
#endif
#ifdef HIGH
#undef HIGH
#endif
#ifdef CHANGE
#undef CHANGE
#endif

#include "ScenarioGenerator.h"
#include "DecisionEngine.h"
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>

#ifdef ARDUINO
#include <Arduino.h>
#else
static uint32_t randomVal(uint32_t minV, uint32_t maxV) {
    static uint32_t s = 12345;
    s = s * 1103515245 + 12345;
    return minV + (s % (maxV - minV + 1));
}
#define random(min, max) randomVal(min, max - 1)
#endif

#ifdef LOW
#undef LOW
#endif
#ifdef HIGH
#undef HIGH
#endif
#ifdef CHANGE
#undef CHANGE
#endif

float calculateDNASimilarity(const ScenarioDNA& a, const ScenarioDNA& b) {
    float score = 0.0f;

    if (a.category == b.category) score += 35.0f;
    if (a.conflict == b.conflict) score += 30.0f;

    float diffRisk = std::abs((float)a.riskLevel - b.riskLevel);
    float diffNov  = std::abs((float)a.noveltyLevel - b.noveltyLevel);
    float diffSoc  = std::abs((float)a.socialLevel - b.socialLevel);

    float paramDiff = (diffRisk + diffNov + diffSoc) / 3.0f; // 0 ~ 100
    score += (100.0f - paramDiff) * 0.35f;

    return std::min(100.0f, score);
}

// 12 个 Category 的中英双语标题模板库与变体生成器
struct CategoryContext {
    const char* title;
    const char* titleCN;
    const char* w0; const char* w1; const char* w2; const char* w3;
    const char* c0; const char* c1; const char* c2; const char* c3;
    DecisionType defaultType;
};

static const CategoryContext CATEGORY_CONTEXTS[12] = {
    // 0. SOCIAL
    {
        "SOCIAL INVITATION", "社交临时邀约",
        "A friend invites you to an impromptu party tonight.",
        "Join an unfamiliar group activity this weekend?",
        "An old friend asks to catch up right now.",
        "Attend a large social event with new faces?",
        "朋友临时邀请你参加今晚的聚会？",
        "本周末参加一个陌生的团队活动？",
        "多年未见的老友突然约你现在见一面？",
        "去参加一个充满新面孔的大型社交晚会？",
        DecisionType::GO
    },
    // 1. MONEY
    {
        "IMPULSE PURCHASE", "心仪物品冲动消费",
        "Buy a desired gadget on flash sale today?",
        "Upgrade your subscription for premium features?",
        "Spend money on a luxury dining experience?",
        "Purchase a rare collectible item on sale?",
        "打折购买关注已久的酷炫数码产品？",
        "升级付费订阅以解锁高级体验功能？",
        "尝试一次昂贵奢华的精致餐饮体验？",
        "购买一件限量折扣收藏物品？",
        DecisionType::GET
    },
    // 2. EXPERIENCE
    {
        "NEW EXPERIENCE", "尝试全新体验",
        "Try an activity you have never done before.",
        "Taste a completely unfamiliar exotic dish.",
        "Explore a hidden spot in your city today.",
        "Start learning an unusual new creative skill.",
        "参加一项以前从未体验过的活动。",
        "尝试一道完全陌生的异国风味料理。",
        "探索城市中一处隐秘未知的景点。",
        "开始学习一项独特的全新创意技能。",
        DecisionType::DO
    },
    // 3. RISK
    {
        "HIGH STAKES RISK", "高风险回报抉择",
        "Take a high-risk high-reward opportunity.",
        "Invest in a volatile but promising venture.",
        "Make a bold leap without guaranteed safety.",
        "Accept a challenging gamble with big upside.",
        "抓住一个高风险高回报的罕见机会。",
        "投资一个充满波动但前景广阔的项目。",
        "在没有绝对安全保障下勇敢跨出一步。",
        "接受一项充满挑战但潜力巨大的赌注。",
        DecisionType::RISK
    },
    // 4. CHANGE
    {
        "ROUTINE CHANGE", "改变常规路线习惯",
        "Completely alter your usual daily schedule.",
        "Take a new unfamiliar route to your destination.",
        "Scrap your weekend plans for something else.",
        "Ditch your daily habits for a random day.",
        "彻底改变你日常固定的时间安排。",
        "选择一条全新的陌生路线前往目的地。",
        "取消既定的周末计划尝试全新的事情。",
        "抛开日常习惯度过随机的一天。",
        DecisionType::CHANGE
    },
    // 5. CREATIVITY
    {
        "CREATIVE IMPULSE", "直觉灵感创作",
        "Follow a sudden intuitive artistic idea.",
        "Build something just for fun with zero utility.",
        "Express your candid thoughts on a project.",
        "Try an unproven unorthodox method today.",
        "顺应突然涌现的直觉艺术灵感。",
        "制作一个纯粹为了好玩而无实用价值的东西。",
        "坦诚表达你对某个项目的真实看法。",
        "尝试一种非传统且未被验证的新方法。",
        DecisionType::DO
    },
    // 6. TIME
    {
        "TIME DECISION", "时间安排控制",
        "Spend an entire day without any schedule.",
        "Act immediately instead of waiting to plan.",
        "Dedicate your evening to personal leisure.",
        "Drop current tasks to seize a quick chance.",
        "度过完全没有任何日程规划的一天。",
        "立即采取行动而不是等待周密计划。",
        "将整个晚上时间用于个人兴趣休息。",
        "放下手中任务去抓住一个即时机会。",
        DecisionType::WAIT
    },
    // 7. UNCERTAINTY
    {
        "UNKNOWN OUTCOME", "面对未知不确定性",
        "Agree to an offer without researching it first.",
        "Step into a scenario with zero predictability.",
        "Trust a stranger's recommendation blindly.",
        "Take a blind step into an unknown challenge.",
        "在未经调查的情况下直接接受一项提议。",
        "踏入一个完全无法预测后果的情境。",
        "盲目信任一位陌生人的即兴推荐。",
        "向未知的挑战迈出盲目的一步。",
        DecisionType::ACCEPT
    },
    // 8. OPPORTUNITY
    {
        "SUDDEN OPPORTUNITY", "突发罕见机会",
        "Seize an unexpected chance offered today.",
        "Say YES to a sudden last-minute invite.",
        "Apply for a competitive surprise opening.",
        "Grab a limited-time opportunity right now.",
        "抓住今天突如其来的罕见机遇。",
        "对一个临时的突发邀请回答 YES。",
        "申请一个极具竞争力的惊喜职位/机会。",
        "立刻抓住这个限时难得的机会。",
        DecisionType::ACCEPT
    },
    // 9. COMFORT
    {
        "COMFORT VS ACTION", "舒适区与挑战",
        "Leave your cozy room for an outdoor event?",
        "Choose comfort over a novel adventure today.",
        "Cancel plans to enjoy a peaceful quiet day?",
        "Stay in your comfort zone or take a leap?",
        "离开舒适的房间去参加户外活动？",
        "今天选择惬意舒适而不是冒险？",
        "取消活动安排享受一个安静的午后？",
        "留在舒适区还是勇敢跃出一步？",
        DecisionType::CHOOSE
    },
    // 10. RELATIONSHIP
    {
        "HONEST EXPRESSION", "真实想法表达",
        "Tell a close friend your 100% honest opinion.",
        "Reach out to someone you haven't talked to.",
        "Resolve a silent misunderstanding today.",
        "Share a personal secret with a trusted peer.",
        "向亲密好友表达你100%真实的观点。",
        "主动联系一位很久未沟通的人。",
        "今天主动化解一个沉寂的误会。",
        "与信任的伙伴分享一个个人秘密。",
        DecisionType::SAY
    },
    // 11. SELF
    {
        "SELF DISCOVERY", "个人边界突破",
        "Challenge a personal boundary or old habit.",
        "Do something purely for your own growth.",
        "Make a decision ignoring others' opinions.",
        "Try an option you normally always avoid.",
        "挑战一个个人习惯或旧的边界。",
        "纯粹为了自己的成长去做一件事。",
        "无视他人的眼光独立做出决策。",
        "尝试一个你平时总是习惯规避的选项。",
        DecisionType::DO
    }
};

static ScenarioDNA generateRandomDNA() {
    ScenarioDNA dna;
#ifdef ARDUINO
    dna.category = static_cast<ScenarioCategory>(random(0, 12));
    dna.conflict = static_cast<DecisionConflict>(random(0, 10));
    dna.riskLevel = (uint8_t)random(15, 90);
    dna.noveltyLevel = (uint8_t)random(20, 95);
    dna.socialLevel = (uint8_t)random(10, 95);
    dna.uncertaintyLevel = (uint8_t)random(15, 90);
    dna.timePressure = (uint8_t)random(10, 90);
    dna.financialImpact = (uint8_t)random(10, 85);
    dna.contextId = (uint8_t)random(0, 12);
    dna.wordingVariant = (uint8_t)random(0, 4);
#else
    dna.category = static_cast<ScenarioCategory>(rand() % 12);
    dna.conflict = static_cast<DecisionConflict>(rand() % 10);
    dna.riskLevel = 20 + rand() % 70;
    dna.noveltyLevel = 25 + rand() % 70;
    dna.socialLevel = 15 + rand() % 80;
    dna.uncertaintyLevel = 20 + rand() % 70;
    dna.timePressure = 15 + rand() % 70;
    dna.financialImpact = 10 + rand() % 75;
    dna.contextId = static_cast<uint8_t>(dna.category);
    dna.wordingVariant = rand() % 4;
#endif
    return dna;
}

GeneratedScenario generateProceduralScenario(const ScenarioDNA recentDNAs[10], int historyCount) {
    GeneratedScenario bestScenario;
    float bestQualityScore = -9999.0f;

    // 从 5 个 Candidates 中进行多维选优
    for (int candidateIdx = 0; candidateIdx < CANDIDATE_POOL_SIZE; ++candidateIdx) {
        ScenarioDNA candidateDNA = generateRandomDNA();
        int catIdx = static_cast<int>(candidateDNA.category) % 12;
        int wordIdx = candidateDNA.wordingVariant % 4;

        const CategoryContext& ctxInfo = CATEGORY_CONTEXTS[catIdx];
        const char* wDesc[4] = { ctxInfo.w0, ctxInfo.w1, ctxInfo.w2, ctxInfo.w3 };
        const char* cDesc[4] = { ctxInfo.c0, ctxInfo.c1, ctxInfo.c2, ctxInfo.c3 };

        // 拼接生成 Candidate 的 Scenario 向量
        Scenario sc;
        sc.type = ctxInfo.defaultType;
        sc.risk = (float)candidateDNA.riskLevel;
        sc.novelty = (float)candidateDNA.noveltyLevel;
        sc.social = (float)candidateDNA.socialLevel;
        sc.uncertainty = (float)candidateDNA.uncertaintyLevel;
        sc.time = (float)candidateDNA.timePressure;
        sc.cost = (float)candidateDNA.financialImpact;
        sc.effort = 40.0f;
        sc.emotionalValue = (candidateDNA.category == ScenarioCategory::EXPERIENCE || candidateDNA.category == ScenarioCategory::COMFORT) ? 85.0f : 40.0f;
        sc.practicalValue = (candidateDNA.category == ScenarioCategory::MONEY || candidateDNA.category == ScenarioCategory::TIME) ? 85.0f : 35.0f;

        // 1. 评估 Diversity Score
        float maxSimilarity = 0.0f;
        for (int h = 0; h < historyCount && h < RECENT_DNA_HISTORY_SIZE; ++h) {
            float sim = calculateDNASimilarity(candidateDNA, recentDNAs[h]);
            if (sim > maxSimilarity) maxSimilarity = sim;
        }
        float diversityScore = 100.0f - maxSimilarity;
        if (maxSimilarity > 65.0f) {
            diversityScore -= 50.0f;
        }

        // 2. 评估 MBTI Disagreement Score
        DecisionResult results[MBTI_COUNT];
        simulateAll(sc, results);
        DecisionSummary summary = summarizeResults(results);

        float disagreementScore = 0.0f;
        if (summary.yesCount >= 4 && summary.noCount >= 4) {
            disagreementScore = 50.0f;
        } else if (summary.yesCount >= 2 && summary.noCount >= 2) {
            disagreementScore = 30.0f;
        } else {
            disagreementScore = 10.0f;
        }

        float totalQuality = diversityScore + disagreementScore;

        if (totalQuality > bestQualityScore) {
            bestQualityScore = totalQuality;
            bestScenario.dna = candidateDNA;
            bestScenario.scenario = sc;
            bestScenario.qualityScore = totalQuality;
            snprintf(bestScenario.title, sizeof(bestScenario.title), "%s", ctxInfo.title);
            snprintf(bestScenario.description, sizeof(bestScenario.description), "%s", wDesc[wordIdx]);
            snprintf(bestScenario.titleCN, sizeof(bestScenario.titleCN), "%s", ctxInfo.titleCN);
            snprintf(bestScenario.descriptionCN, sizeof(bestScenario.descriptionCN), "%s", cDesc[wordIdx]);
        }
    }

    return bestScenario;
}
