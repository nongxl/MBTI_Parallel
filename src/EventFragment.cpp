#include "EventFragment.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>

// ---------------- [全量补齐 50+ 现实生活 Event Fragment 数据库 (全矩阵 Tension 约束覆盖)] ----------------
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
    // Travel - Tension 0: 耽误原计划
    {
        "CONSTRAINT_TRAVEL_PLAN_01", FragmentType::CONSTRAINT,
        "但这会完全打乱你原本安排好的安静周末，让你没有时间休息与阅读。",
        "However, this completely disrupts your planned quiet weekend for reading.",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::OPPORTUNITY_VS_REST }, 2,
        { "TRAVEL", "PLAN_DISRUPT" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::STAY_HOME },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        DecisionMechanism::OPPORTUNITY_COST
    },
    // Travel - Tension 1: 不好拒绝
    {
        "CONSTRAINT_TRAVEL_SOCIAL_02", FragmentType::CONSTRAINT,
        "老同学表现得非常热情和期待，如果你拒绝可能会让气氛有些尴尬。",
        "Your classmate is eager; declining might make the atmosphere awkward.",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::SOCIAL_INVITATION }, 2,
        { "TRAVEL", "SOCIAL" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        DecisionMechanism::SOCIAL_EXPECTATION
    },
    // Travel - Tension 2: 花钱/路费开销
    {
        "CONSTRAINT_SELF_DRIVE_FUEL_02", FragmentType::CONSTRAINT,
        "全程往返的油费和过路费需要大家平摊，而且你需要帮忙轮流开一部分车程。",
        "However, fuel & toll fees will be shared, and you need to share driving duties.",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::OPPORTUNITY_VS_REST }, 2,
        { "TRAVEL", "DRIVE_COST" }, 10, 30,
        { TransportMode::SELF_DRIVE, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::SELF_DRIVE, CostType::FUEL, ExistingPlan::NONE },
        { TransportMode::FLIGHT, CostType::FLIGHT_TICKET, ExistingPlan::NONE },
        DecisionMechanism::RESOURCE_ALLOCATION
    },
    // Travel - Tension 3: 细节不了解/不知道值不值
    {
        "CONSTRAINT_TRAVEL_UNCERTAIN_03", FragmentType::CONSTRAINT,
        "你对那座海边小镇的天气与行程安排完全不了解，不知道折腾一趟会不会值。",
        "You know little about the coastal town weather or schedule, unsure if it's worth it.",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::KNOWN_VS_UNKNOWN }, 2,
        { "TRAVEL", "UNCERTAIN" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::TIME, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        DecisionMechanism::UNCERTAINTY
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

    // ==== [2. PURCHASE & MONEY (购物/理财/旧设备组)] ====
    {
        "SETUP_PURCHASE_SAVINGS_01", FragmentType::SETUP,
        "在整理房间和书桌时，你正思考着是否要升级自己日常高频使用的装备。",
        "While cleaning your room, you considered upgrading your daily essential gear.",
        { ArchetypeID::LIMITED_TIME_PURCHASE, ArchetypeID::QUALITY_VS_COST, ArchetypeID::DUPLICATE_PURCHASE }, 3,
        { "PURCHASE", "PLAN" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::BUDGET, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        DecisionMechanism::QUALITY_VS_COST
    },
    {
        "EVENT_GADGET_DISCOUNT_01", FragmentType::EVENT,
        "你看中的那款旗舰便携设备今天刚好限时打折，价格相当吸引人。",
        "The flagship portable gadget you tracked is suddenly on limited-time discount.",
        { ArchetypeID::LIMITED_TIME_PURCHASE, ArchetypeID::QUALITY_VS_COST }, 2,
        { "PURCHASE", "DISCOUNT" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::BUDGET, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        DecisionMechanism::QUALITY_VS_COST
    },
    // Purchase - Tension 0: 旧的还能用 / 花钱
    {
        "CONSTRAINT_PURCHASE_OLD_WORKS_01", FragmentType::CONSTRAINT,
        "虽然新设备体验很好，但你手上现有的旧设备依然能正常使用，买新的意味着额外花钱。",
        "Though the new device is great, your old one still works fine; buying costs extra.",
        { ArchetypeID::DUPLICATE_PURCHASE, ArchetypeID::QUALITY_VS_COST }, 2,
        { "PURCHASE", "OLD_WORKS" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::BUDGET, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::BUDGET, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        DecisionMechanism::THRESHOLD
    },
    // Purchase - Tension 1: 性价比不高
    {
        "CONSTRAINT_PURCHASE_PRICE_02", FragmentType::CONSTRAINT,
        "虽然新特性很吸引人，但它的售价偏高，相较于提升的效率显得性价比一般。",
        "New features are cool, but the price tag is high relative to actual utility.",
        { ArchetypeID::LIMITED_TIME_PURCHASE, ArchetypeID::QUALITY_VS_COST }, 2,
        { "PURCHASE", "RATIO" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::BUDGET, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::BUDGET, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        DecisionMechanism::QUALITY_VS_COST
    },
    // Purchase - Tension 3: 一时冲动/不知道值不值得
    {
        "CONSTRAINT_PURCHASE_IMPULSE_03", FragmentType::CONSTRAINT,
        "你担心自己只是一时新鲜冲动消费，买回家后可能用不了几次就会吃灰。",
        "You worry it's an impulse buy that will end up gathering dust on your shelf.",
        { ArchetypeID::LIMITED_TIME_PURCHASE, ArchetypeID::DUPLICATE_PURCHASE }, 2,
        { "PURCHASE", "IMPULSE" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        DecisionMechanism::UNCERTAINTY
    },
    {
        "DECISION_PURCHASE_BUY_01", FragmentType::DECISION_FRAME,
        "面对这个难得的限时折扣，你会选择现在下单购买吗？",
        "Faced with this discount, will you choose to buy it now?",
        { ArchetypeID::LIMITED_TIME_PURCHASE, ArchetypeID::DUPLICATE_PURCHASE }, 2,
        { "PURCHASE", "BUY" }, 10, 10,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        DecisionMechanism::QUALITY_VS_COST
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
    // Work - Tension 0: 耽误原计划
    {
        "CONSTRAINT_WORK_OVERTIME_01", FragmentType::CONSTRAINT,
        "但这需要你今晚额外加班至少1.5小时，还会耽误你原本安排好的晚餐约会。",
        "However, helping out will cost 1.5 hours overtime and delay your dinner date.",
        { ArchetypeID::UNEXPECTED_REQUEST, ArchetypeID::HELP_VS_BOUNDARY }, 2,
        { "WORK", "OVERTIME" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::OVERTIME, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::OVERTIME, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        DecisionMechanism::OPPORTUNITY_COST
    },
    // Work - Tension 1: 不好拒绝 / 顾及人情
    {
        "CONSTRAINT_WORK_SOCIAL_02", FragmentType::CONSTRAINT,
        "同事平时和你关系不错且表现得很焦急，直接拒绝可能会伤害人际关系。",
        "The coworker is a good friend who looks anxious; refusing might hurt your bond.",
        { ArchetypeID::UNEXPECTED_REQUEST, ArchetypeID::HELP_VS_BOUNDARY }, 2,
        { "WORK", "RELATION" }, 10, 30,
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        { TransportMode::UNKNOWN, CostType::NONE, ExistingPlan::NONE },
        DecisionMechanism::SOCIAL_EXPECTATION
    },
    // Work - Tension 3: 细节不了解 / 不知道值不值得
    {
        "CONSTRAINT_WORK_UNCERTAIN_03", FragmentType::CONSTRAINT,
        "你对该项目的具体背景和活动细节完全不了解，无法预估要花多少精力，也不知道是否值得。",
        "You know little about project details, unsure of time/energy required or if it's worth it.",
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
        categoryTag = "PURCHASE";
        story.category = ScenarioCategory::PURCHASE;
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

    DecisionMechanism activeMechanism = constraintFrag ? constraintFrag->mechanism : story.mechanism;

    if (strcmp(categoryTag, "PURCHASE") == 0) {
        snprintf(story.choiceA_CN, sizeof(story.choiceA_CN), "果断购买 (BUY)");
        snprintf(story.choiceB_CN, sizeof(story.choiceB_CN), "理性克制 (WAIT)");
        snprintf(story.choiceA_EN, sizeof(story.choiceA_EN), "BUY WITH DISCOUNT");
        snprintf(story.choiceB_EN, sizeof(story.choiceB_EN), "PASS & SAVE");

        if (activeMechanism == DecisionMechanism::THRESHOLD || activeMechanism == DecisionMechanism::QUALITY_VS_COST) {
            story.scenario = { DecisionType::GET, 60.0f, 85.0f, 75.0f, 80.0f, 15.0f, 20.0f, 70.0f, 70.0f, 50.0f };
        } else {
            story.scenario = { DecisionType::GET, 55.0f, 75.0f, 65.0f, 80.0f, 25.0f, 20.0f, 65.0f, 75.0f, 60.0f };
        }
    } else if (strcmp(categoryTag, "DAILY") == 0) {
        snprintf(story.choiceA_CN, sizeof(story.choiceA_CN), "尝试新品 (TRY)");
        snprintf(story.choiceB_CN, sizeof(story.choiceB_CN), "经典稳妥 (STAY)");
        snprintf(story.choiceA_EN, sizeof(story.choiceA_EN), "TRY NEW DRINK");
        snprintf(story.choiceB_EN, sizeof(story.choiceB_EN), "CLASSIC DRINK");
        story.scenario = { DecisionType::GET, 65.0f, 40.0f, 25.0f, 85.0f, 20.0f, 15.0f, 75.0f, 60.0f, 25.0f };
    } else if (strcmp(categoryTag, "WORK") == 0) {
        snprintf(story.choiceA_CN, sizeof(story.choiceA_CN), "留下来帮 (HELP)");
        snprintf(story.choiceB_CN, sizeof(story.choiceB_CN), "准时下班 (LEAVE)");
        snprintf(story.choiceA_EN, sizeof(story.choiceA_EN), "STAY & HELP");
        snprintf(story.choiceB_EN, sizeof(story.choiceB_EN), "LEAVE ON TIME");

        if (activeMechanism == DecisionMechanism::UNCERTAINTY) {
            story.scenario = { DecisionType::DO, 75.0f, 85.0f, 85.0f, 30.0f, 35.0f, 80.0f, 25.0f, 70.0f, 75.0f };
        } else if (activeMechanism == DecisionMechanism::OPPORTUNITY_COST) {
            story.scenario = { DecisionType::DO, 65.0f, 40.0f, 85.0f, 25.0f, 40.0f, 80.0f, 30.0f, 75.0f, 80.0f };
        } else {
            story.scenario = { DecisionType::DO, 30.0f, 35.0f, 75.0f, 30.0f, 80.0f, 85.0f, 25.0f, 60.0f, 85.0f };
        }
    } else {
        snprintf(story.choiceA_CN, sizeof(story.choiceA_CN), "果断前往 (GO)");
        snprintf(story.choiceB_CN, sizeof(story.choiceB_CN), "按原计划 (STAY)");
        snprintf(story.choiceA_EN, sizeof(story.choiceA_EN), "ACCEPT & GO");
        snprintf(story.choiceB_EN, sizeof(story.choiceB_EN), "STICK TO PLAN");

        if (activeMechanism == DecisionMechanism::OPPORTUNITY_COST) {
            story.scenario = { DecisionType::GO, 65.0f, 50.0f, 85.0f, 85.0f, 45.0f, 80.0f, 75.0f, 80.0f, 60.0f };
        } else {
            story.scenario = { DecisionType::GO, 75.0f, 75.0f, 85.0f, 85.0f, 45.0f, 80.0f, 75.0f, 65.0f, 40.0f };
        }
    }

    DecisionSignature sig = { static_cast<uint8_t>(story.category), story.mechanism, DecisionShape::YES_NO };
    markSignatureUsed(sig);

    return story;
}
