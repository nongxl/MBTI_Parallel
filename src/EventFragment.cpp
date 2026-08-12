#include "EventFragment.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>

// ---------------- [高质量现实故事 Event Fragment 数据库 (严格 Category 与 Tag 分组)] ----------------
static const EventFragment g_fragmentBank[] = {
    // ==== [1. TRAVEL & OPPORTUNITY (旅行/机会组)] ====
    {
        "SETUP_TRAVEL_REST_01", FragmentType::SETUP,
        "你原本打算这个周末待在家里，把最近一直没空看的那几本书读完。",
        "On Friday night, you planned to spend the weekend relaxing at home reading.",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::SOCIAL_INVITATION, ArchetypeID::OPPORTUNITY_VS_REST }, 3,
        { "TRAVEL", "OPPORTUNITY", "REST" }, 10, 15
    },
    {
        "EVENT_TOKYO_FLIGHT_01", FragmentType::EVENT,
        "周五晚上，朋友突然发消息说抢到了两张明天去东京的特价机票。",
        "On Friday night, a friend messages saying he secured cheap flights to Tokyo for tomorrow.",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::KNOWN_VS_UNKNOWN, ArchetypeID::OPPORTUNITY_VS_REST }, 3,
        { "TRAVEL", "OPPORTUNITY", "FLIGHT" }, 10, 20
    },
    {
        "CONSTRAINT_TRAVEL_COST_01", FragmentType::CONSTRAINT,
        "不过往返机票费用需要你自己承担，而且明早6点就要准备出发。",
        "However, you must cover your own ticket, and departure is at 6 AM tomorrow.",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::OPPORTUNITY_VS_REST }, 2,
        { "TRAVEL", "OPPORTUNITY", "TIME_PRESSURE" }, 10, 15
    },
    {
        "DECISION_TRAVEL_GO_01", FragmentType::DECISION_FRAME,
        "面对这个突如其来的旅行机会，你会选择改计划出发吗？",
        "Facing this sudden travel opportunity, will you pack up and go?",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::KNOWN_VS_UNKNOWN }, 2,
        { "TRAVEL", "OPPORTUNITY", "CHOICE" }, 10, 5
    },

    // ==== [2. PURCHASE & MONEY (购物/理财组)] ====
    {
        "SETUP_PURCHASE_SAVINGS_01", FragmentType::SETUP,
        "你最近刚为自己下半年的旅游储备制定了严格的预算攒钱计划。",
        "You recently set up a strict budget plan for a future vacation trip.",
        { ArchetypeID::LIMITED_TIME_PURCHASE, ArchetypeID::QUALITY_VS_COST, ArchetypeID::DUPLICATE_PURCHASE }, 3,
        { "PURCHASE", "MONEY", "PLAN" }, 10, 15
    },
    {
        "EVENT_GADGET_DISCOUNT_01", FragmentType::EVENT,
        "你关注了两个月的限定款便携设备今天在商场居然限时六折放量。",
        "The portable gadget you tracked for two months is suddenly 40% off today.",
        { ArchetypeID::LIMITED_TIME_PURCHASE, ArchetypeID::QUALITY_VS_COST }, 2,
        { "PURCHASE", "MONEY", "GADGET" }, 10, 20
    },
    {
        "CONSTRAINT_PURCHASE_BUDGET_01", FragmentType::CONSTRAINT,
        "虽然折扣诱人，但买下它意味着你要挪用本月原本计划存下的闲钱。",
        "Although enticing, buying it means using up your planned monthly savings.",
        { ArchetypeID::LIMITED_TIME_PURCHASE, ArchetypeID::QUALITY_VS_COST }, 2,
        { "PURCHASE", "MONEY", "BUDGET" }, 10, 15
    },
    {
        "DECISION_PURCHASE_BUY_01", FragmentType::DECISION_FRAME,
        "面对这个难得的限时折扣，你会选择现在下单购买吗？",
        "Faced with this discount, will you choose to buy it now?",
        { ArchetypeID::LIMITED_TIME_PURCHASE, ArchetypeID::DUPLICATE_PURCHASE }, 2,
        { "PURCHASE", "MONEY", "BUY" }, 10, 5
    },

    // ==== [3. WORK & REQUEST (工作/请求组)] ====
    {
        "SETUP_WORK_OFF_01", FragmentType::SETUP,
        "下班前10分钟，你正收拾好东西准备准时回家享受个人夜晚。",
        "10 minutes before off-work, you packed your bag getting ready to head home.",
        { ArchetypeID::UNEXPECTED_REQUEST, ArchetypeID::NOW_VS_LATER, ArchetypeID::HELP_VS_BOUNDARY }, 3,
        { "WORK", "REQUEST", "TIME" }, 10, 15
    },
    {
        "EVENT_WORK_HELP_01", FragmentType::EVENT,
        "同事突然抱来一份紧急项目材料，客气地问你能不能帮忙协助核对。",
        "A coworker brings urgent project materials, politely asking for your help to review.",
        { ArchetypeID::UNEXPECTED_REQUEST, ArchetypeID::HELP_VS_BOUNDARY }, 2,
        { "WORK", "REQUEST", "HELP" }, 10, 20
    },
    {
        "CONSTRAINT_WORK_OVERTIME_01", FragmentType::CONSTRAINT,
        "但这需要你今晚额外加班至少1.5小时，还会耽误你原本的晚餐约会。",
        "However, helping out will cost you 1.5 hours of overtime and delay your dinner.",
        { ArchetypeID::UNEXPECTED_REQUEST, ArchetypeID::HELP_VS_BOUNDARY }, 2,
        { "WORK", "REQUEST", "TIME" }, 10, 15
    },
    {
        "DECISION_WORK_HELP_01", FragmentType::DECISION_FRAME,
        "面对同事的求助，你会选择留下来帮忙吗？",
        "Facing your coworker's request, will you stay and help?",
        { ArchetypeID::UNEXPECTED_REQUEST, ArchetypeID::HELP_VS_BOUNDARY }, 2,
        { "WORK", "REQUEST", "HELP" }, 10, 5
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

static const EventFragment* pickFragmentWithTag(FragmentType type, ArchetypeID archetype, const char* requiredTag) {
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

            bool matchesTag = true;
            if (requiredTag) {
                matchesTag = false;
                for (int t = 0; t < 4; ++t) {
                    if (g_fragmentBank[i].tags[t] && strcmp(g_fragmentBank[i].tags[t], requiredTag) == 0) {
                        matchesTag = true;
                        break;
                    }
                }
            }

            if (matchesArch && matchesTag && !isFragmentCoolingDown(g_fragmentBank[i].id)) {
                candidates[count++] = &g_fragmentBank[i];
            }
        }
    }

    if (count == 0) {
        // Fallback 宽容匹配
        for (int i = 0; i < FRAGMENT_BANK_SIZE; ++i) {
            if (g_fragmentBank[i].type == type) {
                bool matchesTag = true;
                if (requiredTag) {
                    matchesTag = false;
                    for (int t = 0; t < 4; ++t) {
                        if (g_fragmentBank[i].tags[t] && strcmp(g_fragmentBank[i].tags[t], requiredTag) == 0) {
                            matchesTag = true;
                            break;
                        }
                    }
                }
                if (matchesTag) {
                    candidates[count++] = &g_fragmentBank[i];
                }
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

    // 根据 Archetype 确定核心领域 Tag（彻底解决购物配机票的荒谬混搭 Bug）
    const char* categoryTag = "TRAVEL";
    if (chosenArchetype == ArchetypeID::LIMITED_TIME_PURCHASE || chosenArchetype == ArchetypeID::QUALITY_VS_COST || chosenArchetype == ArchetypeID::DUPLICATE_PURCHASE) {
        categoryTag = "PURCHASE";
        story.category = ScenarioCategory::PURCHASE;
    } else if (chosenArchetype == ArchetypeID::UNEXPECTED_REQUEST || chosenArchetype == ArchetypeID::HELP_VS_BOUNDARY) {
        categoryTag = "WORK";
        story.category = ScenarioCategory::WORK;
    } else {
        categoryTag = "TRAVEL";
        story.category = ScenarioCategory::TRAVEL;
    }

    const EventFragment* setupFrag = pickFragmentWithTag(FragmentType::SETUP, chosenArchetype, categoryTag);
    const EventFragment* eventFrag = pickFragmentWithTag(FragmentType::EVENT, chosenArchetype, categoryTag);
    const EventFragment* constraintFrag = pickFragmentWithTag(FragmentType::CONSTRAINT, chosenArchetype, categoryTag);
    const EventFragment* decisionFrag = pickFragmentWithTag(FragmentType::DECISION_FRAME, chosenArchetype, categoryTag);

    snprintf(story.titleCN, sizeof(story.titleCN), "现实微场景抉择");
    snprintf(story.titleEN, sizeof(story.titleEN), "REALITY MICRO DECISION");

    // 【100% 完整拼装】: Setup + Event + Constraint + Decision 确保句尾一定有清晰明确的问题！
    snprintf(story.bodyCN, sizeof(story.bodyCN), "%s%s%s%s",
             setupFrag ? setupFrag->zh : "",
             eventFrag ? eventFrag->zh : "",
             constraintFrag ? constraintFrag->zh : "",
             decisionFrag ? decisionFrag->zh : "面对这个抉择，你会做出什么选择？");

    snprintf(story.bodyEN, sizeof(story.bodyEN), "%s %s %s %s",
             setupFrag ? setupFrag->en : "",
             eventFrag ? eventFrag->en : "",
             constraintFrag ? constraintFrag->en : "",
             decisionFrag ? decisionFrag->en : "Facing this situation, what will you do?");

    // 绑定专属行为动词选项
    if (strcmp(categoryTag, "PURCHASE") == 0) {
        snprintf(story.choiceA_CN, sizeof(story.choiceA_CN), "果断购买 (BUY)");
        snprintf(story.choiceB_CN, sizeof(story.choiceB_CN), "理性克制 (WAIT)");
        snprintf(story.choiceA_EN, sizeof(story.choiceA_EN), "BUY WITH DISCOUNT");
        snprintf(story.choiceB_EN, sizeof(story.choiceB_EN), "PASS & SAVE");
        story.scenario = { DecisionType::GET, 30.0f, 70.0f, 10.0f, 70.0f, 20.0f, 10.0f, 30.0f, 85.0f, 50.0f };
    } else if (strcmp(categoryTag, "WORK") == 0) {
        snprintf(story.choiceA_CN, sizeof(story.choiceA_CN), "留下来帮 (HELP)");
        snprintf(story.choiceB_CN, sizeof(story.choiceB_CN), "准时下班 (LEAVE)");
        snprintf(story.choiceA_EN, sizeof(story.choiceA_EN), "STAY & HELP");
        snprintf(story.choiceB_EN, sizeof(story.choiceB_EN), "LEAVE ON TIME");
        story.scenario = { DecisionType::DO, 20.0f, 40.0f, 80.0f, 70.0f, 90.0f, 70.0f, 30.0f, 60.0f, 80.0f };
    } else {
        snprintf(story.choiceA_CN, sizeof(story.choiceA_CN), "果断前往 (GO)");
        snprintf(story.choiceB_CN, sizeof(story.choiceB_CN), "按原计划 (STAY)");
        snprintf(story.choiceA_EN, sizeof(story.choiceA_EN), "ACCEPT & GO");
        snprintf(story.choiceB_EN, sizeof(story.choiceB_EN), "STICK TO PLAN");
        story.scenario = { DecisionType::GO, 75.0f, 60.0f, 60.0f, 90.0f, 60.0f, 50.0f, 85.0f, 85.0f, 40.0f };
    }

    return story;
}
