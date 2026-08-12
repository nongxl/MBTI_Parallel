#include "EventFragment.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>

// ---------------- [高品质生活故事 Event Fragment 数据库] ----------------
static const EventFragment g_fragmentBank[] = {
    // ---- [1. SETUP 前因背景片段] ----
    {
        "SETUP_HOME_WEEKEND_01", FragmentType::SETUP,
        "你原本打算这个周末待在家里，把最近一直没空看的那几本书读完。",
        "On Friday night, you planned to spend the weekend relaxing at home reading.",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::SOCIAL_INVITATION, ArchetypeID::OPPORTUNITY_VS_REST }, 3,
        { "HOME", "WEEKEND", "REST" }, 10, 15
    },
    {
        "SETUP_WORK_OFF_01", FragmentType::SETUP,
        "下班前10分钟，你正收拾好东西准备准时回家享受夜晚。",
        "10 minutes before off-work, you packed your bag getting ready to head home.",
        { ArchetypeID::UNEXPECTED_REQUEST, ArchetypeID::NOW_VS_LATER, ArchetypeID::HELP_VS_BOUNDARY }, 3,
        { "WORK", "OFF_WORK", "ROUTINE" }, 10, 15
    },
    {
        "SETUP_SAVINGS_PLAN_01", FragmentType::SETUP,
        "你最近刚为自己下半年的旅游储备制定了严格的预算卡。",
        "You recently set up a strict budget plan for a future vacation trip.",
        { ArchetypeID::LIMITED_TIME_PURCHASE, ArchetypeID::QUALITY_VS_COST, ArchetypeID::DUPLICATE_PURCHASE }, 3,
        { "BUDGET", "MONEY", "PLAN" }, 10, 15
    },

    // ---- [2. EVENT 触发小故事片段] ----
    {
        "EVENT_TOKYO_FLIGHT_01", FragmentType::EVENT,
        "朋友突然发来消息说，他抢到了两张明天去东京的特价机票。",
        "A friend messages saying he secured two cheap flights to Tokyo for tomorrow.",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::KNOWN_VS_UNKNOWN, ArchetypeID::OPPORTUNITY_VS_REST }, 3,
        { "TOKYO", "FLIGHT", "TRAVEL" }, 10, 20
    },
    {
        "EVENT_CONCERT_TICKET_01", FragmentType::EVENT,
        "很久没见的老同学多买了一张今晚极难买到的前排演唱会门票。",
        "An old classmate got an extra ticket for a sold-out concert tonight.",
        { ArchetypeID::SOCIAL_INVITATION, ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::HELP_VS_BOUNDARY }, 3,
        { "CONCERT", "TICKET", "FRIEND" }, 10, 20
    },
    {
        "EVENT_DISCOUNT_GADGET_01", FragmentType::EVENT,
        "你关注了两个月的限定款便携设备今天在商场居然限时六折放量。",
        "The portable gadget you tracked for two months is suddenly 40% off today.",
        { ArchetypeID::LIMITED_TIME_PURCHASE, ArchetypeID::QUALITY_VS_COST }, 2,
        { "DISCOUNT", "GADGET", "SHOPPING" }, 10, 20
    },

    // ---- [3. CONTEXT 心理背景片段] ----
    {
        "CONTEXT_WANT_LONG_TIME_01", FragmentType::CONTEXT,
        "你其实一直非常想去那个城市体验一次，只是之前总抽不出空。",
        "You have always wanted to visit that city, but never found the free time.",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::KNOWN_VS_UNKNOWN }, 2,
        { "DESIRE", "DREAM", "TRAVEL" }, 10, 10
    },
    {
        "CONTEXT_GOOD_FRIEND_01", FragmentType::CONTEXT,
        "你们平时关系很要好，但最近几个月因为各自忙碌很少聚在一起了。",
        "You are close friends, though busy schedules kept you apart for months.",
        { ArchetypeID::SOCIAL_INVITATION, ArchetypeID::UNEXPECTED_REQUEST }, 2,
        { "FRIENDSHIP", "BOND", "REUNION" }, 10, 10
    },

    // ---- [4. CONSTRAINT 现实拉扯片段] ----
    {
        "CONSTRAINT_EXPENSE_TOMORROW_01", FragmentType::CONSTRAINT,
        "不过往返机票费用需要你自己承担，而且明早6点就要准备出发。",
        "However, you must cover your own ticket, and departure is at 6 AM tomorrow.",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::OPPORTUNITY_VS_REST }, 2,
        { "COST", "MORNING", "TIME_PRESSURE" }, 10, 15
    },
    {
        "CONSTRAINT_WORK_DEADLINE_01", FragmentType::CONSTRAINT,
        "但这需要你今晚牺牲至少2小时的私人时间，明早还要早起开会。",
        "This requires sacrificing 2 hours tonight, with an early morning meeting tomorrow.",
        { ArchetypeID::UNEXPECTED_REQUEST, ArchetypeID::NOW_VS_LATER }, 2,
        { "TIME", "OVERTIME", "PRESSURE" }, 10, 15
    },

    // ---- [5. TWIST 细节转折片段] ----
    {
        "TWIST_UNTESTED_PLACE_01", FragmentType::TWIST,
        "后来你才了解到，朋友其实也是第一次去那个陌生地点，没有任何攻略。",
        "You then realize your friend has never been there either—zero guidebooks.",
        { ArchetypeID::KNOWN_VS_UNKNOWN, ArchetypeID::LAST_MINUTE_OPPORTUNITY }, 2,
        { "UNPLANNED", "UNKNOWN", "RISK" }, 10, 10
    },

    // ---- [6. DECISION_FRAME 决策提示片段] ----
    {
        "DECISION_WILL_YOU_GO_01", FragmentType::DECISION_FRAME,
        "面对这个突如其来的机会，你会选择出发吗？",
        "Facing this sudden opportunity, will you pack up and go?",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::KNOWN_VS_UNKNOWN }, 2,
        { "DECISION", "CHOICE" }, 10, 5
    },
    {
        "DECISION_WILL_YOU_BUY_01", FragmentType::DECISION_FRAME,
        "面对这个难得的折扣，你会选择现在下单吗？",
        "Faced with this discount, will you buy it now?",
        { ArchetypeID::LIMITED_TIME_PURCHASE, ArchetypeID::DUPLICATE_PURCHASE }, 2,
        { "BUY", "CHOICE" }, 10, 5
    }
};

constexpr int FRAGMENT_BANK_SIZE = sizeof(g_fragmentBank) / sizeof(EventFragment);

static char g_recentFragmentIds[16][32];
static int g_recentFragCount = 0;

static bool isFragmentCoolingDown(const char* fragId) {
    for (int i = 0; i < g_recentFragCount; ++i) {
        if (strcmp(g_recentFragmentIds[i], fragId) == 0) {
            return true;
        }
    }
    return false;
}

static void markFragmentUsed(const char* fragId) {
    for (int i = 15; i > 0; --i) {
        strncpy(g_recentFragmentIds[i], g_recentFragmentIds[i - 1], sizeof(g_recentFragmentIds[i]));
    }
    snprintf(g_recentFragmentIds[0], sizeof(g_recentFragmentIds[0]), "%s", fragId);
    if (g_recentFragCount < 16) g_recentFragCount++;
}

static const EventFragment* pickFragment(FragmentType type, ArchetypeID archetype) {
    const EventFragment* candidates[FRAGMENT_BANK_SIZE];
    int count = 0;

    for (int i = 0; i < FRAGMENT_BANK_SIZE; ++i) {
        if (g_fragmentBank[i].type == type) {
            bool matchesArch = false;
            for (int a = 0; a < g_fragmentBank[i].archetypeCount; ++a) {
                if (g_fragmentBank[i].compatibleArchetypes[a] == archetype) {
                    matchesArch = true;
                    break;
                }
            }
            if (matchesArch && !isFragmentCoolingDown(g_fragmentBank[i].id)) {
                candidates[count++] = &g_fragmentBank[i];
            }
        }
    }

    if (count == 0) {
        for (int i = 0; i < FRAGMENT_BANK_SIZE; ++i) {
            if (g_fragmentBank[i].type == type) {
                candidates[count++] = &g_fragmentBank[i];
            }
        }
    }

    if (count > 0) {
        const EventFragment* chosen = candidates[rand() % count];
        markFragmentUsed(chosen->id);
        return chosen;
    }
    return nullptr;
}

AssembledStoryScenario assembleFragmentScenario(ArchetypeID chosenArchetype) {
    uint32_t seed = rand();
    AssembledStoryScenario story;
    memset(&story, 0, sizeof(story));

    story.archetype = chosenArchetype;
    story.seed = seed;
    snprintf(story.scenarioId, sizeof(story.scenarioId), "FST_%04X", (unsigned int)(seed & 0xFFFF));

    const EventFragment* setupFrag = pickFragment(FragmentType::SETUP, chosenArchetype);
    const EventFragment* eventFrag = pickFragment(FragmentType::EVENT, chosenArchetype);
    const EventFragment* constraintFrag = pickFragment(FragmentType::CONSTRAINT, chosenArchetype);

    story.category = ScenarioCategory::TRAVEL;

    snprintf(story.titleCN, sizeof(story.titleCN), "现实微场景抉择");
    snprintf(story.titleEN, sizeof(story.titleEN), "REALITY MICRO DECISION");

    // 【连贯极简拼装】: 无硬回车，自然断句拼装
    snprintf(story.bodyCN, sizeof(story.bodyCN), "%s%s%s",
             setupFrag ? setupFrag->zh : "你原本打算这个周末待在家里。",
             eventFrag ? eventFrag->zh : "朋友突然发消息说抢到了机票。",
             constraintFrag ? constraintFrag->zh : "不过费用需要你自己承担。");

    snprintf(story.bodyEN, sizeof(story.bodyEN), "%s %s %s",
             setupFrag ? setupFrag->en : "You planned to spend the weekend relaxing at home.",
             eventFrag ? eventFrag->en : "A friend messages saying he secured two cheap flights.",
             constraintFrag ? constraintFrag->en : "However, you must cover your own ticket.");

    snprintf(story.choiceA_CN, sizeof(story.choiceA_CN), "果断前往 (GO)");
    snprintf(story.choiceB_CN, sizeof(story.choiceB_CN), "按原计划 (STAY)");
    snprintf(story.choiceA_EN, sizeof(story.choiceA_EN), "ACCEPT & GO");
    snprintf(story.choiceB_EN, sizeof(story.choiceB_EN), "STICK TO PLAN");

    story.scenario = { DecisionType::GO, 75.0f, 60.0f, 60.0f, 90.0f, 60.0f, 50.0f, 85.0f, 85.0f, 40.0f };
    return story;
}
