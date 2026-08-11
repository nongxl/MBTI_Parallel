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

// 12 个 Category 的标题模板库与变体生成器
struct CategoryContext {
    const char* title;
    const char* wordingTemplates[4];
    DecisionType defaultType;
};

static const CategoryContext CATEGORY_CONTEXTS[12] = {
    // 0. SOCIAL
    {
        "SOCIAL INVITATION",
        {
            "A friend invites you to an impromptu party tonight.",
            "Join an unfamiliar group activity this weekend?",
            "An old friend asks to catch up right now.",
            "Attend a large social event with new faces?"
        },
        DecisionType::GO
    },
    // 1. MONEY
    {
        "IMPULSE PURCHASE",
        {
            "Buy a desired gadget on flash sale today?",
            "Upgrade your subscription for premium features?",
            "Spend money on a luxury dining experience?",
            "Purchase a rare collectible item on sale?"
        },
        DecisionType::GET
    },
    // 2. EXPERIENCE
    {
        "NEW EXPERIENCE",
        {
            "Try an activity you have never done before.",
            "Taste a completely unfamiliar exotic dish.",
            "Explore a hidden spot in your city today.",
            "Start learning an unusual new creative skill."
        },
        DecisionType::DO
    },
    // 3. RISK
    {
        "HIGH STAKES RISK",
        {
            "Take a high-risk high-reward opportunity.",
            "Invest in a volatile but promising venture.",
            "Make a bold leap without guaranteed safety.",
            "Accept a challenging gamble with big upside."
        },
        DecisionType::RISK
    },
    // 4. CHANGE
    {
        "ROUTINE CHANGE",
        {
            "Completely alter your usual daily schedule.",
            "Take a new unfamiliar route to your destination.",
            "Scrap your weekend plans for something else.",
            "Ditch your daily habits for a random day."
        },
        DecisionType::CHANGE
    },
    // 5. CREATIVITY
    {
        "CREATIVE IMPULSE",
        {
            "Follow a sudden intuitive artistic idea.",
            "Build something just for fun with zero utility.",
            "Express your candid thoughts on a project.",
            "Try an unproven unorthodox method today."
        },
        DecisionType::DO
    },
    // 6. TIME
    {
        "TIME DECISION",
        {
            "Spend an entire day without any schedule.",
            "Act immediately instead of waiting to plan.",
            "Dedicate your evening to personal leisure.",
            "Drop current tasks to seize a quick chance."
        },
        DecisionType::WAIT
    },
    // 7. UNCERTAINTY
    {
        "UNKNOWN OUTCOME",
        {
            "Agree to an offer without researching it first.",
            "Step into a scenario with zero predictability.",
            "Trust a stranger's recommendation blindly.",
            "Take a blind step into an unknown challenge."
        },
        DecisionType::ACCEPT
    },
    // 8. OPPORTUNITY
    {
        "SUDDEN OPPORTUNITY",
        {
            "Seize an unexpected chance offered today.",
            "Say YES to a sudden last-minute invite.",
            "Apply for a competitive surprise opening.",
            "Grab a limited-time opportunity right now."
        },
        DecisionType::ACCEPT
    },
    // 9. COMFORT
    {
        "COMFORT VS ACTION",
        {
            "Leave your cozy room for an outdoor event?",
            "Choose comfort over a novel adventure today.",
            "Cancel plans to enjoy a peaceful quiet day?",
            "Stay in your comfort zone or take a leap?"
        },
        DecisionType::CHOOSE
    },
    // 10. RELATIONSHIP
    {
        "HONEST EXPRESSION",
        {
            "Tell a close friend your 100% honest opinion.",
            "Reach out to someone you haven't talked to.",
            "Resolve a silent misunderstanding today.",
            "Share a personal secret with a trusted peer."
        },
        DecisionType::SAY
    },
    // 11. SELF
    {
        "SELF DISCOVERY",
        {
            "Challenge a personal boundary or old habit.",
            "Do something purely for your own growth.",
            "Make a decision ignoring others' opinions.",
            "Try an option you normally always avoid."
        },
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

        // 拼接生成 Candidate 的 Scenario 向量
        Scenario sc;
        sc.type = CATEGORY_CONTEXTS[catIdx].defaultType;
        sc.risk = (float)candidateDNA.riskLevel;
        sc.novelty = (float)candidateDNA.noveltyLevel;
        sc.social = (float)candidateDNA.socialLevel;
        sc.uncertainty = (float)candidateDNA.uncertaintyLevel;
        sc.time = (float)candidateDNA.timePressure;
        sc.cost = (float)candidateDNA.financialImpact;
        sc.effort = 40.0f;
        sc.emotionalValue = (candidateDNA.category == ScenarioCategory::EXPERIENCE || candidateDNA.category == ScenarioCategory::COMFORT) ? 85.0f : 40.0f;
        sc.practicalValue = (candidateDNA.category == ScenarioCategory::MONEY || candidateDNA.category == ScenarioCategory::TIME) ? 85.0f : 35.0f;

        // 1. 评估 Diversity Score (与最近 10 次生成 DNA 历史计算相似度)
        float maxSimilarity = 0.0f;
        for (int h = 0; h < historyCount && h < RECENT_DNA_HISTORY_SIZE; ++h) {
            float sim = calculateDNASimilarity(candidateDNA, recentDNAs[h]);
            if (sim > maxSimilarity) maxSimilarity = sim;
        }
        float diversityScore = 100.0f - maxSimilarity;
        if (maxSimilarity > 65.0f) {
            diversityScore -= 50.0f; // 重罚换皮重复
        }

        // 2. 评估 MBTI Disagreement Score (先运行 16 人格模拟查看分歧度)
        DecisionResult results[MBTI_COUNT];
        simulateAll(sc, results);
        DecisionSummary summary = summarizeResults(results);

        float disagreementScore = 0.0f;
        if (summary.yesCount >= 4 && summary.noCount >= 4) {
            disagreementScore = 50.0f; // 人格强烈分歧，极富把玩价值！
        } else if (summary.yesCount >= 2 && summary.noCount >= 2) {
            disagreementScore = 30.0f;
        } else {
            disagreementScore = 10.0f; // 结果同质化过高
        }

        float totalQuality = diversityScore + disagreementScore;

        if (totalQuality > bestQualityScore) {
            bestQualityScore = totalQuality;
            bestScenario.dna = candidateDNA;
            bestScenario.scenario = sc;
            bestScenario.qualityScore = totalQuality;
            snprintf(bestScenario.title, sizeof(bestScenario.title), "%s", CATEGORY_CONTEXTS[catIdx].title);
            snprintf(bestScenario.description, sizeof(bestScenario.description), "%s", CATEGORY_CONTEXTS[catIdx].wordingTemplates[wordIdx]);
        }
    }

    return bestScenario;
}
