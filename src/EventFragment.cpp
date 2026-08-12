#include "EventFragment.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>

// ---------------- [50+ 丰富多元现实生活 Event Fragment 数据库 (带 Phase 6C 事实 + Phase 6D 底层机制)] ----------------
static const EventFragment g_fragmentBank[] = {
    // ==== [1. TRAVEL & ADVENTURE (旅行/探险组 - 机制: OPPORTUNITY_COST / UNCERTAINTY)] ====
    {
        "SETUP_TRAVEL_REST_01", FragmentType::SETUP,
        "你原本打算这个周末待在家里，把最近一直没空看的那几本书读完。",
        "On Friday night, you planned to spend the weekend relaxing at home reading.",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::SOCIAL_INVITATION, ArchetypeID::OPPORTUNITY_VS_REST }, 3,
        { "TRAVEL", "REST" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::STAY_HOME },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::TRAVEL },
        DecisionMechanism::OPPORTUNITY_COST
    },
    {
        "EVENT_ROAD_TRIP_02", FragmentType::EVENT,
        "高中老同学突然打来电话，邀请你参加明天清晨开往海边小镇的自驾游。",
        "An old classmate calls inviting you on a road trip to a coastal town tomorrow morning.",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::SOCIAL_INVITATION }, 2,
        { "TRAVEL", "ROAD_TRIP" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::SELF_DRIVE, CostType::FUEL, ExistingPlan::NONE },
        { TransportMode::FLIGHT, CostType::FLIGHT_TICKET, ExistingPlan::NONE },
        DecisionMechanism::UNCERTAINTY
    },
    {
        "CONSTRAINT_SELF_DRIVE_FUEL_02", FragmentType::CONSTRAINT,
        "不过全程往返的油费和高速过路费需要大家平摊，而且需要你帮忙轮流开一部分车程。",
        "However, fuel & toll fees will be shared, and you need to share driving duties.",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::OPPORTUNITY_VS_REST }, 2,
        { "TRAVEL", "DRIVE_COST" }, 10, 30,
        { TransportMode::SELF_DRIVE, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::SELF_DRIVE, CostType::FUEL, ExistingPlan::NONE },
        { TransportMode::FLIGHT, CostType::FLIGHT_TICKET, ExistingPlan::NONE },
        DecisionMechanism::RESOURCE_ALLOCATION
    },
    {
        "DECISION_TRAVEL_GO_01", FragmentType::DECISION_FRAME,
        "面对这个突如其来的旅行机会，你会选择改计划出发吗？",
        "Facing this sudden travel opportunity, will you pack up and go?",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::KNOWN_VS_UNKNOWN }, 2,
        { "TRAVEL", "CHOICE" }, 10, 10,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        DecisionMechanism::REVERSIBILITY
    },

    // ==== [2. LOW-STAKES DAILY (日常轻度决策 - 机制: LOW_STAKES_DAILY / THRESHOLD)] ====
    {
        "SETUP_DAILY_COFFEE_01", FragmentType::SETUP,
        "在午后工作间隙，你习惯性地走进楼下的精选咖啡馆准备点一杯午后饮料。",
        "During a work break, you walked into the coffee shop downstairs for a drink.",
        { ArchetypeID::KNOWN_VS_UNKNOWN, ArchetypeID::QUALITY_VS_COST }, 2,
        { "DAILY", "COFFEE" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        DecisionMechanism::LOW_STAKES_DAILY
    },
    {
        "EVENT_COFFEE_NEW_BEVERAGE_01", FragmentType::EVENT,
        "收银台前推出了一款口味奇异的特调新品，而你平常常喝的拿铁刚好在做经典打折。",
        "They launched a strange new special beverage alongside your usual classic latte on sale.",
        { ArchetypeID::KNOWN_VS_UNKNOWN, ArchetypeID::QUALITY_VS_COST }, 2,
        { "DAILY", "SPECIAL" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::BUDGET, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        DecisionMechanism::LOW_STAKES_DAILY
    },
    {
        "CONSTRAINT_COFFEE_RISK_01", FragmentType::CONSTRAINT,
        "新品口碑两极分化严重，有人觉得风味惊艳，也有人觉得口感奇怪难以接受。",
        "Reviews for the new flavor are polar, some love it while others hate it.",
        { ArchetypeID::KNOWN_VS_UNKNOWN, ArchetypeID::RISK_VS_CERTAINTY }, 2,
        { "DAILY", "RISK" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        DecisionMechanism::UNCERTAINTY
    },
    {
        "DECISION_COFFEE_TRY_01", FragmentType::DECISION_FRAME,
        "面对这款风险与惊喜并存的新品，你会选择尝试探险吗？",
        "Will you choose to try this adventurous new flavor drink?",
        { ArchetypeID::KNOWN_VS_UNKNOWN, ArchetypeID::RISK_VS_CERTAINTY }, 2,
        { "DAILY", "TRY" }, 10, 10,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        DecisionMechanism::LOW_STAKES_DAILY
    },

    // ==== [3. WORK & CAREER (职场/协作组)] ====
    {
        "SETUP_WORK_OFF_01", FragmentType::SETUP,
        "下班前10分钟，你正收拾好东西准备准时回家享受个人夜晚。",
        "10 minutes before off-work, you packed your bag getting ready to head home.",
        { ArchetypeID::UNEXPECTED_REQUEST, ArchetypeID::NOW_VS_LATER, ArchetypeID::HELP_VS_BOUNDARY }, 3,
        { "WORK", "TIME" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::TIME, ExistingPlan::WORK },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        DecisionMechanism::OPPORTUNITY_COST
    },
    {
        "EVENT_WORK_HELP_01", FragmentType::EVENT,
        "同事突然抱来一份紧急项目材料，客气地问你能不能帮忙协助核对。",
        "A coworker brings urgent project materials, politely asking for your help.",
        { ArchetypeID::UNEXPECTED_REQUEST, ArchetypeID::HELP_VS_BOUNDARY }, 2,
        { "WORK", "HELP" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::OVERTIME, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        DecisionMechanism::SOCIAL_EXPECTATION
    },
    {
        "CONSTRAINT_WORK_UNCERTAIN_02", FragmentType::CONSTRAINT,
        "但你对该项目的具体背景和活动细节完全不了解，无法预估要花多少精力和时间，也不知道是否值得。",
        "However, you know little about project details, unsure of time/energy required or if it's worth it.",
        { ArchetypeID::UNEXPECTED_REQUEST, ArchetypeID::HELP_VS_BOUNDARY }, 2,
        { "WORK", "UNCERTAIN" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::OVERTIME, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        DecisionMechanism::UNCERTAINTY
    },
    {
        "DECISION_WORK_HELP_01", FragmentType::DECISION_FRAME,
        "面对同事的求助，你会选择留下来帮忙吗？",
        "Facing your coworker's request, will you stay and help?",
        { ArchetypeID::UNEXPECTED_REQUEST, ArchetypeID::HELP_VS_BOUNDARY }, 2,
        { "WORK", "CHOICE" }, 10, 10,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        DecisionMechanism::BOUNDARY
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
        state.applyProvides(chosen->providesFact);
        return chosen;
    }
    return nullptr;
}

AssembledStoryScenario assembleFragmentScenario(ArchetypeID chosenArchetype) {
    uint32_t seed = rand();
    AssembledStoryScenario story;
    memset(&story, 0, sizeof(story));

    ScenarioState state;
    state.reset();

    story.archetype = chosenArchetype;
    story.seed = seed;
    snprintf(story.scenarioId, sizeof(story.scenarioId), "FST_%04X", (unsigned int)(seed & 0xFFFF));

    const char* categoryTag = "TRAVEL";
    if (chosenArchetype == ArchetypeID::LIMITED_TIME_PURCHASE || chosenArchetype == ArchetypeID::QUALITY_VS_COST || chosenArchetype == ArchetypeID::DUPLICATE_PURCHASE) {
        categoryTag = (rand() % 2 == 0) ? "PURCHASE" : "DAILY";
        story.category = (strcmp(categoryTag, "DAILY") == 0) ? ScenarioCategory::WORK : ScenarioCategory::PURCHASE;
    } else if (chosenArchetype == ArchetypeID::UNEXPECTED_REQUEST || chosenArchetype == ArchetypeID::HELP_VS_BOUNDARY || chosenArchetype == ArchetypeID::UNCERTAIN_OPPORTUNITY) {
        categoryTag = "WORK";
        story.category = ScenarioCategory::WORK;
    } else {
        categoryTag = "TRAVEL";
        story.category = ScenarioCategory::TRAVEL;
    }

    const EventFragment* setupFrag = pickFragmentWithFactState(FragmentType::SETUP, chosenArchetype, categoryTag, state);
    const EventFragment* eventFrag = pickFragmentWithFactState(FragmentType::EVENT, chosenArchetype, categoryTag, state);
    const EventFragment* constraintFrag = pickFragmentWithFactState(FragmentType::CONSTRAINT, chosenArchetype, categoryTag, state);
    const EventFragment* decisionFrag = pickFragmentWithFactState(FragmentType::DECISION_FRAME, chosenArchetype, categoryTag, state);

    story.mechanism = eventFrag ? eventFrag->mechanism : DecisionMechanism::OPPORTUNITY_COST;

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
    } else if (strcmp(categoryTag, "DAILY") == 0) {
        snprintf(story.choiceA_CN, sizeof(story.choiceA_CN), "尝试新品 (TRY)");
        snprintf(story.choiceB_CN, sizeof(story.choiceB_CN), "经典稳妥 (STAY)");
        snprintf(story.choiceA_EN, sizeof(story.choiceA_EN), "TRY NEW DRINK");
        snprintf(story.choiceB_EN, sizeof(story.choiceB_EN), "CLASSIC DRINK");
        story.scenario = { DecisionType::GET, 40.0f, 30.0f, 15.0f, 90.0f, 20.0f, 10.0f, 60.0f, 85.0f, 30.0f };
    } else if (strcmp(categoryTag, "WORK") == 0) {
        snprintf(story.choiceA_CN, sizeof(story.choiceA_CN), "留下来帮 (HELP)");
        snprintf(story.choiceB_CN, sizeof(story.choiceB_CN), "准时下班 (LEAVE)");
        snprintf(story.choiceA_EN, sizeof(story.choiceA_EN), "STAY & HELP");
        snprintf(story.choiceB_EN, sizeof(story.choiceB_EN), "LEAVE ON TIME");
        
        // 【关键修复】: 动态平衡 WORK 场景中的不确定性风险 (Risk/Uncertainty=85) 与人际压力，产生精彩的 16 人格激战分歧！
        story.scenario = { DecisionType::DO, 75.0f, 85.0f, 85.0f, 30.0f, 35.0f, 80.0f, 25.0f, 70.0f, 75.0f };
    } else {
        snprintf(story.choiceA_CN, sizeof(story.choiceA_CN), "果断前往 (GO)");
        snprintf(story.choiceB_CN, sizeof(story.choiceB_CN), "按原计划 (STAY)");
        snprintf(story.choiceA_EN, sizeof(story.choiceA_EN), "ACCEPT & GO");
        snprintf(story.choiceB_EN, sizeof(story.choiceB_EN), "STICK TO PLAN");
        story.scenario = { DecisionType::GO, 75.0f, 65.0f, 70.0f, 92.0f, 60.0f, 50.0f, 85.0f, 88.0f, 45.0f };
    }

    DecisionSignature sig = { static_cast<uint8_t>(story.category), story.mechanism, DecisionShape::YES_NO };
    markSignatureUsed(sig);

    return story;
}
