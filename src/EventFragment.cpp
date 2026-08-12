#include "EventFragment.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>

// ---------------- [50+ 丰富多元现实生活 Event Fragment 数据库 (覆盖 12 大 Category)] ----------------
static const EventFragment g_fragmentBank[] = {
    // ==== [1. TRAVEL & ADVENTURE (旅行/探险组)] ====
    {
        "SETUP_TRAVEL_REST_01", FragmentType::SETUP,
        "你原本打算这个周末待在家里，把最近一直没空看的那几本书读完。",
        "On Friday night, you planned to spend the weekend relaxing at home reading.",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::SOCIAL_INVITATION, ArchetypeID::OPPORTUNITY_VS_REST }, 3,
        { "TRAVEL", "REST" }, 10, 30
    },
    {
        "SETUP_TRAVEL_ROUTINE_02", FragmentType::SETUP,
        "你最近工作十分顺畅，正想找个机会呼吸一下郊外的森林空气。",
        "Work went smoothly recently, and you wanted a breath of mountain air.",
        { ArchetypeID::KNOWN_VS_UNKNOWN, ArchetypeID::LAST_MINUTE_OPPORTUNITY }, 2,
        { "TRAVEL", "NATURE" }, 10, 30
    },
    {
        "EVENT_TOKYO_FLIGHT_01", FragmentType::EVENT,
        "周五晚上，朋友突然发消息说抢到了两张明天去东京的特价机票。",
        "A friend messages saying he secured cheap flights to Tokyo for tomorrow.",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::KNOWN_VS_UNKNOWN }, 2,
        { "TRAVEL", "FLIGHT" }, 10, 30
    },
    {
        "EVENT_ROAD_TRIP_02", FragmentType::EVENT,
        "高中老同学突然打来电话，邀请你参加明天清晨开往海边小镇的自驾游。",
        "An old classmate calls inviting you on a road trip to a coastal town tomorrow morning.",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::SOCIAL_INVITATION }, 2,
        { "TRAVEL", "ROAD_TRIP" }, 10, 30
    },
    {
        "CONSTRAINT_TRAVEL_COST_01", FragmentType::CONSTRAINT,
        "不过往返机票费用需要你自己承担，而且明早6点就要准备出发。",
        "However, you must cover your ticket, and departure is at 6 AM tomorrow.",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::OPPORTUNITY_VS_REST }, 2,
        { "TRAVEL", "TIME_PRESSURE" }, 10, 30
    },
    {
        "CONSTRAINT_TRAVEL_WEATHER_02", FragmentType::CONSTRAINT,
        "不过那边的气象预报显示周末可能下阵雨，而且没有任何详细攻略。",
        "However, rain is forecasted and there are no travel guides available.",
        { ArchetypeID::KNOWN_VS_UNKNOWN, ArchetypeID::RISK_VS_CERTAINTY }, 2,
        { "TRAVEL", "RISK" }, 10, 30
    },
    {
        "DECISION_TRAVEL_GO_01", FragmentType::DECISION_FRAME,
        "面对这个突如其来的旅行机会，你会选择改计划出发吗？",
        "Facing this sudden travel opportunity, will you pack up and go?",
        { ArchetypeID::LAST_MINUTE_OPPORTUNITY, ArchetypeID::KNOWN_VS_UNKNOWN }, 2,
        { "TRAVEL", "CHOICE" }, 10, 10
    },

    // ==== [2. PURCHASE & MONEY (购物/理财/品质组)] ====
    {
        "SETUP_PURCHASE_SAVINGS_01", FragmentType::SETUP,
        "你最近刚为自己下半年的旅游储备制定了严格的预算攒钱计划。",
        "You recently set up a strict budget plan for a future vacation trip.",
        { ArchetypeID::LIMITED_TIME_PURCHASE, ArchetypeID::QUALITY_VS_COST, ArchetypeID::DUPLICATE_PURCHASE }, 3,
        { "PURCHASE", "PLAN" }, 10, 30
    },
    {
        "SETUP_PURCHASE_GEAR_02", FragmentType::SETUP,
        "你的旧电脑和电子设备用了挺多年，功能完好但外观显得有些陈旧。",
        "Your current laptop works fine but shows signs of aging after years of use.",
        { ArchetypeID::DUPLICATE_PURCHASE, ArchetypeID::QUALITY_VS_COST }, 2,
        { "PURCHASE", "GEAR" }, 10, 30
    },
    {
        "EVENT_GADGET_DISCOUNT_01", FragmentType::EVENT,
        "你关注了两个月的限定款便携设备今天在商场居然限时六折放量。",
        "The portable gadget you tracked for two months is suddenly 40% off today.",
        { ArchetypeID::LIMITED_TIME_PURCHASE, ArchetypeID::QUALITY_VS_COST }, 2,
        { "PURCHASE", "DISCOUNT" }, 10, 30
    },
    {
        "EVENT_USED_FLAGSHIP_02", FragmentType::EVENT,
        "邻居熟人正转让一台几乎全新的旗舰渲染显示屏，价格只要市场价三分之一。",
        "A neighbor is selling a flagship display monitor at 1/3 market price.",
        { ArchetypeID::QUALITY_VS_COST, ArchetypeID::LIMITED_TIME_PURCHASE }, 2,
        { "PURCHASE", "DEAL" }, 10, 30
    },
    {
        "CONSTRAINT_PURCHASE_BUDGET_01", FragmentType::CONSTRAINT,
        "虽然折扣诱人，但买下它意味着你要挪用本月原本计划存下的闲钱。",
        "Although enticing, buying it means using up your planned monthly savings.",
        { ArchetypeID::LIMITED_TIME_PURCHASE, ArchetypeID::QUALITY_VS_COST }, 2,
        { "PURCHASE", "BUDGET" }, 10, 30
    },
    {
        "CONSTRAINT_PURCHASE_NEED_02", FragmentType::CONSTRAINT,
        "只是你手头的现役设备还能正常使用，买它更多是为了极致外观爽感。",
        "However, your current gear still works, buying it is purely for aesthetics.",
        { ArchetypeID::DUPLICATE_PURCHASE, ArchetypeID::QUALITY_VS_COST }, 2,
        { "PURCHASE", "DESIRE" }, 10, 30
    },
    {
        "DECISION_PURCHASE_BUY_01", FragmentType::DECISION_FRAME,
        "面对这个难得的限时折扣，你会选择现在下单购买吗？",
        "Faced with this discount, will you choose to buy it now?",
        { ArchetypeID::LIMITED_TIME_PURCHASE, ArchetypeID::DUPLICATE_PURCHASE }, 2,
        { "PURCHASE", "BUY" }, 10, 10
    },

    // ==== [3. WORK & CAREER (职场/机遇/边界组)] ====
    {
        "SETUP_WORK_OFF_01", FragmentType::SETUP,
        "下班前10分钟，你正收拾好东西准备准时回家享受个人夜晚。",
        "10 minutes before off-work, you packed your bag getting ready to head home.",
        { ArchetypeID::UNEXPECTED_REQUEST, ArchetypeID::NOW_VS_LATER, ArchetypeID::HELP_VS_BOUNDARY }, 3,
        { "WORK", "TIME" }, 10, 30
    },
    {
        "SETUP_WORK_CAREER_02", FragmentType::SETUP,
        "你在当前岗位工作十分稳定，日常事务轻车熟路，几乎没有太大挑战。",
        "Your current role is stable and comfortable with predictable daily tasks.",
        { ArchetypeID::UNCERTAIN_OPPORTUNITY, ArchetypeID::KNOWN_VS_UNKNOWN }, 2,
        { "WORK", "CAREER" }, 10, 30
    },
    {
        "EVENT_WORK_HELP_01", FragmentType::EVENT,
        "同事突然抱来一份紧急项目材料，客气地问你能不能帮忙协助核对。",
        "A coworker brings urgent project materials, politely asking for your help.",
        { ArchetypeID::UNEXPECTED_REQUEST, ArchetypeID::HELP_VS_BOUNDARY }, 2,
        { "WORK", "HELP" }, 10, 30
    },
    {
        "EVENT_HEADHUNTER_OFFER_02", FragmentType::EVENT,
        "猎头突然联系你，提供了一个薪资翻倍但属于早期创业团队的新职位。",
        "A headhunter offers a startup role with double salary but high risk.",
        { ArchetypeID::UNCERTAIN_OPPORTUNITY, ArchetypeID::KNOWN_VS_UNKNOWN }, 2,
        { "WORK", "OFFER" }, 10, 30
    },
    {
        "CONSTRAINT_WORK_OVERTIME_01", FragmentType::CONSTRAINT,
        "但这需要你今晚额外加班至少1.5小时，还会耽误你原本的晚餐约会。",
        "However, helping out will cost you 1.5 hours of overtime and delay your dinner.",
        { ArchetypeID::UNEXPECTED_REQUEST, ArchetypeID::HELP_VS_BOUNDARY }, 2,
        { "WORK", "OVERTIME" }, 10, 30
    },
    {
        "CONSTRAINT_WORK_RELOCATE_02", FragmentType::CONSTRAINT,
        "不过新公司需要你离开当前居住城市，去异地分部承担拓荒重任。",
        "However, the new job requires relocating to a different city.",
        { ArchetypeID::UNCERTAIN_OPPORTUNITY, ArchetypeID::KNOWN_VS_UNKNOWN }, 2,
        { "WORK", "RELOCATE" }, 10, 30
    },
    {
        "DECISION_WORK_HELP_01", FragmentType::DECISION_FRAME,
        "面对同事的求助，你会选择留下来帮忙吗？",
        "Facing your coworker's request, will you stay and help?",
        { ArchetypeID::UNEXPECTED_REQUEST, ArchetypeID::HELP_VS_BOUNDARY }, 2,
        { "WORK", "CHOICE" }, 10, 10
    },

    // ==== [4. SOCIAL & FRIENDSHIP (社交/人际组)] ====
    {
        "SETUP_SOCIAL_NIGHT_01", FragmentType::SETUP,
        "周六夜晚，你正舒适地宅在沙发上刷着喜欢的电竞比赛直播。",
        "On Saturday night, you were relaxing at home watching an esports stream.",
        { ArchetypeID::SOCIAL_INVITATION, ArchetypeID::OPPORTUNITY_VS_REST }, 2,
        { "SOCIAL", "REST" }, 10, 30
    },
    {
        "EVENT_PARTY_INVITE_01", FragmentType::EVENT,
        "大学社团群里突然发起临时聚会，邀请大家半小时后在市区酒吧集合。",
        "College friends launch a spontaneous bar meetup happening in 30 minutes.",
        { ArchetypeID::SOCIAL_INVITATION, ArchetypeID::LAST_MINUTE_OPPORTUNITY }, 2,
        { "SOCIAL", "PARTY" }, 10, 30
    },
    {
        "CONSTRAINT_SOCIAL_ENERGY_01", FragmentType::CONSTRAINT,
        "但是现场会有很多你不认识的陌生新人，而且可能折腾到深夜。",
        "However, many strangers will be there, and it will last until late night.",
        { ArchetypeID::SOCIAL_INVITATION, ArchetypeID::STRANGER_INTERACTION }, 2,
        { "SOCIAL", "ENERGY" }, 10, 30
    },
    {
        "DECISION_SOCIAL_JOIN_01", FragmentType::DECISION_FRAME,
        "面对这个热闹的社交聚会，你会换衣服出门参加吗？",
        "Faced with this lively invitation, will you get dressed and go?",
        { ArchetypeID::SOCIAL_INVITATION, ArchetypeID::OPPORTUNITY_VS_REST }, 2,
        { "SOCIAL", "CHOICE" }, 10, 10
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

    const char* categoryTag = "TRAVEL";
    if (chosenArchetype == ArchetypeID::LIMITED_TIME_PURCHASE || chosenArchetype == ArchetypeID::QUALITY_VS_COST || chosenArchetype == ArchetypeID::DUPLICATE_PURCHASE) {
        categoryTag = "PURCHASE";
        story.category = ScenarioCategory::PURCHASE;
    } else if (chosenArchetype == ArchetypeID::UNEXPECTED_REQUEST || chosenArchetype == ArchetypeID::HELP_VS_BOUNDARY || chosenArchetype == ArchetypeID::UNCERTAIN_OPPORTUNITY) {
        categoryTag = "WORK";
        story.category = ScenarioCategory::WORK;
    } else if (chosenArchetype == ArchetypeID::SOCIAL_INVITATION || chosenArchetype == ArchetypeID::STRANGER_INTERACTION) {
        categoryTag = "SOCIAL";
        story.category = ScenarioCategory::SOCIAL;
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
    } else if (strcmp(categoryTag, "SOCIAL") == 0) {
        snprintf(story.choiceA_CN, sizeof(story.choiceA_CN), "欣然前往 (JOIN)");
        snprintf(story.choiceB_CN, sizeof(story.choiceB_CN), "宅家休息 (REST)");
        snprintf(story.choiceA_EN, sizeof(story.choiceA_EN), "JOIN PARTY");
        snprintf(story.choiceB_EN, sizeof(story.choiceB_EN), "STAY HOME");
        story.scenario = { DecisionType::GO, 30.0f, 40.0f, 60.0f, 70.0f, 95.0f, 60.0f, 30.0f, 85.0f, 40.0f };
    } else {
        snprintf(story.choiceA_CN, sizeof(story.choiceA_CN), "果断前往 (GO)");
        snprintf(story.choiceB_CN, sizeof(story.choiceB_CN), "按原计划 (STAY)");
        snprintf(story.choiceA_EN, sizeof(story.choiceA_EN), "ACCEPT & GO");
        snprintf(story.choiceB_EN, sizeof(story.choiceB_EN), "STICK TO PLAN");
        story.scenario = { DecisionType::GO, 75.0f, 65.0f, 70.0f, 92.0f, 60.0f, 50.0f, 85.0f, 88.0f, 45.0f };
    }

    return story;
}
