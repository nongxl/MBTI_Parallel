#include "EventFragment.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>

// ---------------- [50+ 现实生活 Event Fragment 数据库 (带 Phase 6C 事实一致性元数据)] ----------------
static const EventFragment g_fragmentBank[] = {
    // ==== [1. TRAVEL & ADVENTURE (旅行/探险组)] ====
    {
        "SETUP_TRAVEL_REST_01", FragmentType::SETUP,
        "你原本打算这个周末待在家里，把最近一直没空看的那几本书读完。",
        "On Friday night, you planned to spend the weekend relaxing at home reading.",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::SOCIAL_INVITATION, ArchetypeID::OPPORTUNITY_VS_REST }, 3,
        { "TRAVEL", "REST" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::STAY_HOME },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::TRAVEL }
    },
    {
        "SETUP_TRAVEL_ROUTINE_02", FragmentType::SETUP,
        "你最近工作十分顺畅，正想找个机会呼吸一下郊外的森林空气。",
        "Work went smoothly recently, and you wanted a breath of mountain air.",
        { ArchetypeID::KNOWN_VS_UNKNOWN, ArchetypeID::LAST_MINUTE_OPPORTUNITY }, 2,
        { "TRAVEL", "NATURE" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::REST },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::WORK }
    },
    {
        "EVENT_TOKYO_FLIGHT_01", FragmentType::EVENT,
        "周五晚上，朋友突然发消息说抢到了两张明天去东京的特价机票。",
        "A friend messages saying he secured cheap flights to Tokyo for tomorrow.",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::KNOWN_VS_UNKNOWN }, 2,
        { "TRAVEL", "FLIGHT" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::FLIGHT, CostType::FLIGHT_TICKET, ExistingPlan::NONE },
        { TransportMode::SELF_DRIVE, CostType::FUEL, ExistingPlan::NONE }
    },
    {
        "EVENT_ROAD_TRIP_02", FragmentType::EVENT,
        "高中老同学突然打来电话，邀请你参加明天清晨开往海边小镇的自驾游。",
        "An old classmate calls inviting you on a road trip to a coastal town tomorrow morning.",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::SOCIAL_INVITATION }, 2,
        { "TRAVEL", "ROAD_TRIP" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::SELF_DRIVE, CostType::FUEL, ExistingPlan::NONE },
        { TransportMode::FLIGHT, CostType::FLIGHT_TICKET, ExistingPlan::NONE }
    },
    {
        "CONSTRAINT_TRAVEL_COST_01", FragmentType::CONSTRAINT,
        "不过往返机票费用需要你自己承担，而且明早6点就要准备出发。",
        "However, you must cover your flight ticket, and departure is at 6 AM tomorrow.",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::OPPORTUNITY_VS_REST }, 2,
        { "TRAVEL", "TIME_PRESSURE" }, 10, 30,
        { TransportMode::FLIGHT, CostType::NONE, ExistingPlan::NONE }, // 强制要求必须是机票场景！
        { TransportMode::FLIGHT, CostType::FLIGHT_TICKET, ExistingPlan::NONE },
        { TransportMode::SELF_DRIVE, CostType::FUEL, ExistingPlan::NONE } // 与自驾冲突！
    },
    {
        "CONSTRAINT_SELF_DRIVE_FUEL_02", FragmentType::CONSTRAINT,
        "不过全程往返的油费和高速过路费需要大家平摊，而且需要你帮忙轮流开一部分车程。",
        "However, fuel & toll fees will be shared, and you need to share driving duties.",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::OPPORTUNITY_VS_REST }, 2,
        { "TRAVEL", "DRIVE_COST" }, 10, 30,
        { TransportMode::SELF_DRIVE, CostType::NONE, ExistingPlan::NONE }, // 强制要求必须是自驾游场景！
        { TransportMode::SELF_DRIVE, CostType::FUEL, ExistingPlan::NONE },
        { TransportMode::FLIGHT, CostType::FLIGHT_TICKET, ExistingPlan::NONE } // 与机票冲突！
    },
    {
        "DECISION_TRAVEL_GO_01", FragmentType::DECISION_FRAME,
        "面对这个突如其来的旅行机会，你会选择改计划出发吗？",
        "Facing this sudden travel opportunity, will you pack up and go?",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::KNOWN_VS_UNKNOWN }, 2,
        { "TRAVEL", "CHOICE" }, 10, 10,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE }
    },

    // ==== [2. PURCHASE & MONEY (购物/理财/品质组)] ====
    {
        "SETUP_PURCHASE_SAVINGS_01", FragmentType::SETUP,
        "你最近刚为自己下半年的旅游储备制定了严格的预算攒钱计划。",
        "You recently set up a strict budget plan for a future vacation trip.",
        { ArchetypeID::LIMITED_TIME_PURCHASE, ArchetypeID::QUALITY_VS_COST, ArchetypeID::DUPLICATE_PURCHASE }, 3,
        { "PURCHASE", "PLAN" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::BUDGET, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE }
    },
    {
        "EVENT_GADGET_DISCOUNT_01", FragmentType::EVENT,
        "你关注了两个月的限定款便携设备今天在商场居然限时六折放量。",
        "The portable gadget you tracked for two months is suddenly 40% off today.",
        { ArchetypeID::LIMITED_TIME_PURCHASE, ArchetypeID::QUALITY_VS_COST }, 2,
        { "PURCHASE", "DISCOUNT" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::BUDGET, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE }
    },
    {
        "CONSTRAINT_PURCHASE_BUDGET_01", FragmentType::CONSTRAINT,
        "虽然折扣诱人，但买下它意味着你要挪用本月原本计划存下的闲钱。",
        "Although enticing, buying it means using up your planned monthly savings.",
        { ArchetypeID::LIMITED_TIME_PURCHASE, ArchetypeID::QUALITY_VS_COST }, 2,
        { "PURCHASE", "BUDGET" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::BUDGET, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::BUDGET, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE }
    },
    {
        "DECISION_PURCHASE_BUY_01", FragmentType::DECISION_FRAME,
        "面对这个难得的限时折扣，你会选择现在下单购买吗？",
        "Faced with this discount, will you choose to buy it now?",
        { ArchetypeID::LIMITED_TIME_PURCHASE, ArchetypeID::DUPLICATE_PURCHASE }, 2,
        { "PURCHASE", "BUY" }, 10, 10,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE }
    },

    // ==== [3. WORK & CAREER (职场/机遇/边界组)] ====
    {
        "SETUP_WORK_OFF_01", FragmentType::SETUP,
        "下班前10分钟，你正收拾好东西准备准时回家享受个人夜晚。",
        "10 minutes before off-work, you packed your bag getting ready to head home.",
        { ArchetypeID::UNEXPECTED_REQUEST, ArchetypeID::NOW_VS_LATER, ArchetypeID::HELP_VS_BOUNDARY }, 3,
        { "WORK", "TIME" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::TIME, ExistingPlan::WORK },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE }
    },
    {
        "EVENT_WORK_HELP_01", FragmentType::EVENT,
        "同事突然抱来一份紧急项目材料，客气地问你能不能帮忙协助核对。",
        "A coworker brings urgent project materials, politely asking for your help.",
        { ArchetypeID::UNEXPECTED_REQUEST, ArchetypeID::HELP_VS_BOUNDARY }, 2,
        { "WORK", "HELP" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::OVERTIME, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE }
    },
    {
        "CONSTRAINT_WORK_OVERTIME_01", FragmentType::CONSTRAINT,
        "但这需要你今晚额外加班至少1.5小时，还会耽误你原本的晚餐约会。",
        "However, helping out will cost you 1.5 hours of overtime and delay your dinner.",
        { ArchetypeID::UNEXPECTED_REQUEST, ArchetypeID::HELP_VS_BOUNDARY }, 2,
        { "WORK", "OVERTIME" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::OVERTIME, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::OVERTIME, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE }
    },
    {
        "DECISION_WORK_HELP_01", FragmentType::DECISION_FRAME,
        "面对同事的求助，你会选择留下来帮忙吗？",
        "Facing your coworker's request, will you stay and help?",
        { ArchetypeID::UNEXPECTED_REQUEST, ArchetypeID::HELP_VS_BOUNDARY }, 2,
        { "WORK", "CHOICE" }, 10, 10,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE }
    }
};

constexpr int FRAGMENT_BANK_SIZE = sizeof(g_fragmentBank) / sizeof(EventFragment);

static char g_recentFragmentIds[32][32];
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
    for (int i = 31; i > 0; --i) {
        strncpy(g_recentFragmentIds[i], g_recentFragmentIds[i - 1], sizeof(g_recentFragmentIds[i]));
    }
    snprintf(g_recentFragmentIds[0], sizeof(g_recentFragmentIds[0]), "%s", fragId);
    if (g_recentFragCount < 32) g_recentFragCount++;
}

static const EventFragment* pickFragmentWithFactState(FragmentType type, ArchetypeID archetype, const char* requiredTag, ScenarioState& state) {
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

            // 【Phase 6C 关键约束随机】: 必须通过 canApplyFragment 校验！
            bool isFactValid = canApplyFragment(g_fragmentBank[i], state);

            if (matchesArch && matchesTag && isFactValid && !isFragmentCoolingDown(g_fragmentBank[i].id)) {
                candidates[count++] = &g_fragmentBank[i];
            }
        }
    }

    if (count == 0) {
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
                if (matchesTag && canApplyFragment(g_fragmentBank[i], state)) {
                    candidates[count++] = &g_fragmentBank[i];
                }
            }
        }
    }

    if (count > 0) {
        const EventFragment* chosen = candidates[rand() % count];
        markFragmentUsed(chosen->id);
        // 【Phase 6C 事实锁定】: 将选取的 Fragment 所提供的结构化事实更新写入 ScenarioState 并锁定！
        state.applyProvides(chosen->providesFact);
        return chosen;
    }
    return nullptr;
}

AssembledStoryScenario assembleFragmentScenario(ArchetypeID chosenArchetype) {
    uint32_t seed = rand();
    AssembledStoryScenario story;
    memset(&story, 0, sizeof(story));

    // 【Phase 6C 核心】: 创建并重置场景事实状态机 ScenarioState
    ScenarioState state;
    state.reset();

    story.archetype = chosenArchetype;
    story.seed = seed;
    snprintf(story.scenarioId, sizeof(story.scenarioId), "FST_%04X", (unsigned int)(seed & 0xFFFF));

    const char* categoryTag = "TRAVEL";
    if (chosenArchetype == ArchetypeID::LIMITED_TIME_PURCHASE || chosenArchetype == ArchetypeID::QUALITY_VS_COST || chosenArchetype == ArchetypeID::DUPLICATE_PURCHASE) {
        categoryTag = "PURCHASE";
        story.category = ScenarioCategory::PURCHASE;
    } else if (chosenArchetype == ArchetypeID::UNEXPECTED_REQUEST || chosenArchetype == ArchetypeID::HELP_VS_BOUNDARY || chosenArchetype == ArchetypeID::UNCERTAIN_OPPORTUNITY) {
        categoryTag = "WORK";
        story.category = ScenarioCategory::WORK;
    } else {
        categoryTag = "TRAVEL";
        story.category = ScenarioCategory::TRAVEL;
    }

    // 【Phase 6C 链式组装】: 先选 SETUP & EVENT，触发事实写入 State 锁定，再选择严格匹配的 CONSTRAINT！
    const EventFragment* setupFrag = pickFragmentWithFactState(FragmentType::SETUP, chosenArchetype, categoryTag, state);
    const EventFragment* eventFrag = pickFragmentWithFactState(FragmentType::EVENT, chosenArchetype, categoryTag, state);
    const EventFragment* constraintFrag = pickFragmentWithFactState(FragmentType::CONSTRAINT, chosenArchetype, categoryTag, state);
    const EventFragment* decisionFrag = pickFragmentWithFactState(FragmentType::DECISION_FRAME, chosenArchetype, categoryTag, state);

    snprintf(story.titleCN, sizeof(story.titleCN), "现实微场景抉择");
    snprintf(story.titleEN, sizeof(story.titleEN), "REALITY MICRO DECISION");

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

    if (strcmp(categoryTag, "PURCHASE") == 0) {
        snprintf(story.choiceA_CN, sizeof(story.choiceA_CN), "果断购买 (BUY)");
        snprintf(story.choiceB_CN, sizeof(story.choiceB_CN), "理性克制 (WAIT)");
        snprintf(story.choiceA_EN, sizeof(story.choiceA_EN), "BUY WITH DISCOUNT");
        snprintf(story.choiceB_EN, sizeof(story.choiceB_EN), "PASS & SAVE");
        story.scenario = { DecisionType::GET, 35.0f, 75.0f, 20.0f, 85.0f, 30.0f, 15.0f, 40.0f, 80.0f, 70.0f };
    } else if (strcmp(categoryTag, "WORK") == 0) {
        snprintf(story.choiceA_CN, sizeof(story.choiceA_CN), "留下来帮 (HELP)");
        snprintf(story.choiceB_CN, sizeof(story.choiceB_CN), "准时下班 (LEAVE)");
        snprintf(story.choiceA_EN, sizeof(story.choiceA_EN), "STAY & HELP");
        snprintf(story.choiceB_EN, sizeof(story.choiceB_EN), "LEAVE ON TIME");
        story.scenario = { DecisionType::DO, 25.0f, 30.0f, 85.0f, 30.0f, 90.0f, 75.0f, 25.0f, 60.0f, 85.0f };
    } else {
        snprintf(story.choiceA_CN, sizeof(story.choiceA_CN), "果断前往 (GO)");
        snprintf(story.choiceB_CN, sizeof(story.choiceB_CN), "按原计划 (STAY)");
        snprintf(story.choiceA_EN, sizeof(story.choiceA_EN), "ACCEPT & GO");
        snprintf(story.choiceB_EN, sizeof(story.choiceB_EN), "STICK TO PLAN");
        story.scenario = { DecisionType::GO, 75.0f, 65.0f, 70.0f, 92.0f, 60.0f, 50.0f, 85.0f, 88.0f, 45.0f };
    }

    return story;
}
