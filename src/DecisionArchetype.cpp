#include "DecisionArchetype.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>

const char* getArchetypeIdString(ArchetypeID id) {
    switch (id) {
        case ArchetypeID::LAST_MINUTE_OPPORTUNITY: return "LAST_MINUTE_OPPORTUNITY";
        case ArchetypeID::PLAN_CHANGE:            return "PLAN_CHANGE";
        case ArchetypeID::LIMITED_TIME_PURCHASE:   return "LIMITED_TIME_PURCHASE";
        case ArchetypeID::DUPLICATE_PURCHASE:      return "DUPLICATE_PURCHASE";
        case ArchetypeID::UNEXPECTED_REQUEST:      return "UNEXPECTED_REQUEST";
        case ArchetypeID::SOCIAL_INVITATION:       return "SOCIAL_INVITATION";
        case ArchetypeID::STRANGER_INTERACTION:    return "STRANGER_INTERACTION";
        case ArchetypeID::FRIEND_CONFLICT:         return "FRIEND_CONFLICT";
        case ArchetypeID::SECRET_REQUEST:          return "SECRET_REQUEST";
        case ArchetypeID::UNCERTAIN_OPPORTUNITY:   return "UNCERTAIN_OPPORTUNITY";
        case ArchetypeID::KNOWN_VS_UNKNOWN:        return "KNOWN_VS_UNKNOWN";
        case ArchetypeID::EFFICIENCY_VS_COMFORT:   return "EFFICIENCY_VS_COMFORT";
        case ArchetypeID::AUTONOMY_VS_GUIDANCE:    return "AUTONOMY_VS_GUIDANCE";
        case ArchetypeID::PLAN_VS_IMPROVISATION:   return "PLAN_VS_IMPROVISATION";
        case ArchetypeID::HELP_VS_BOUNDARY:        return "HELP_VS_BOUNDARY";
        case ArchetypeID::RISK_VS_CERTAINTY:       return "RISK_VS_CERTAINTY";
        case ArchetypeID::NOW_VS_LATER:            return "NOW_VS_LATER";
        case ArchetypeID::QUALITY_VS_COST:         return "QUALITY_VS_COST";
        case ArchetypeID::OPPORTUNITY_VS_REST:     return "OPPORTUNITY_VS_REST";
        case ArchetypeID::PERSISTENCE_VS_EFFICIENCY:return "PERSISTENCE_VS_EFFICIENCY";
    }
    return "UNKNOWN";
}

// ---------------- [素材 Variable Pool 定义] ----------------
static const char* VAR_PERSON_CN[] = { "多年的好朋友", "你的大学室友", "项目组同事", "热心的邻居", "高中老同学" };
static const char* VAR_PERSON_EN[] = { "a close friend", "your roomie", "a co-worker", "a neighbor", "an old classmate" };

static const char* VAR_EVENT_CN[] = { "去东京看樱花", "参加音乐节演唱会", "前往海边露营", "参加高端行业沙龙", "品尝网红美食" };
static const char* VAR_EVENT_EN[] = { "trip to Tokyo", "music festival", "coast camping", "VIP conference", "trendy food tasting" };

static const char* VAR_ITEM_CN[] = { "限定款机械键盘", "旗舰级智能手表", "绝版复古定制风衣", "独立开发游戏", "降噪无线耳机" };
static const char* VAR_ITEM_EN[] = { "mechanical keyboard", "flagship smartwatch", "vintage custom coat", "indie game", "noise-canceling headphones" };

static const char* VAR_ORIGINAL_PLAN_CN[] = { "留在家里看电影放松", "按时下班休息", "坚持原有的储蓄计划", "按照预定行程安排行进" };
static const char* VAR_ORIGINAL_PLAN_EN[] = { "relaxing at home", "leaving work on time", "sticking to savings plan", "following scheduled itinerary" };

DynamicScenario generateDynamicArchetypeScenario(ArchetypeID recentArchetypes[8], int historyCount) {
    uint32_t seed = rand();
    ArchetypeID chosenArchetype = static_cast<ArchetypeID>(seed % ARCHETYPE_COUNT);

    // 防连续重复 Archetype
    for (int retry = 0; retry < 5; ++retry) {
        bool isRecent = false;
        for (int h = 0; h < historyCount; ++h) {
            if (recentArchetypes[h] == chosenArchetype) {
                isRecent = true;
                break;
            }
        }
        if (!isRecent) break;
        chosenArchetype = static_cast<ArchetypeID>(rand() % ARCHETYPE_COUNT);
    }

    DynamicScenario dScn;
    memset(&dScn, 0, sizeof(dScn));
    dScn.archetype = chosenArchetype;
    dScn.seed = seed;

    snprintf(dScn.id, sizeof(dScn.id), "ARC_%04X", (unsigned int)(seed & 0xFFFF));

    int personIdx = seed % 5;
    int eventIdx = (seed / 5) % 5;
    int itemIdx = (seed / 25) % 5;
    int planIdx = (seed / 125) % 4;

    const char* pCN = VAR_PERSON_CN[personIdx];
    const char* pEN = VAR_PERSON_EN[personIdx];
    const char* eCN = VAR_EVENT_CN[eventIdx];
    const char* eEN = VAR_EVENT_EN[eventIdx];
    const char* iCN = VAR_ITEM_CN[itemIdx];
    const char* iEN = VAR_ITEM_EN[itemIdx];
    const char* planCN = VAR_ORIGINAL_PLAN_CN[planIdx];
    const char* planEN = VAR_ORIGINAL_PLAN_EN[planIdx];

    switch (chosenArchetype) {
        case ArchetypeID::LAST_MINUTE_OPPORTUNITY: {
            dScn.category = ScenarioCategory::OPPORTUNITY;
            dScn.tags[0] = "TIME_PRESSURE"; dScn.tags[1] = "OPPORTUNITY"; dScn.tags[2] = "SPONTANEOUS"; dScn.tags[3] = "FRIEND";
            
            snprintf(dScn.titleCN, sizeof(dScn.titleCN), "突如其来的邀约");
            snprintf(dScn.descCN, sizeof(dScn.descCN), "%s突然告诉你出现了一个%s的难得机会，需要马上决定是否改计划。", pCN, eCN);
            snprintf(dScn.titleEN, sizeof(dScn.titleEN), "LAST-MINUTE OPPORTUNITY");
            snprintf(dScn.descEN, sizeof(dScn.descEN), "%s tells you about a last-minute %s. You must decide right now.", pEN, eEN);

            snprintf(dScn.choiceA_CN, sizeof(dScn.choiceA_CN), "抓住机会 (GO)");
            snprintf(dScn.choiceB_CN, sizeof(dScn.choiceB_CN), "按原计划 (STAY)");
            snprintf(dScn.choiceA_EN, sizeof(dScn.choiceA_EN), "ACCEPT NOW");
            snprintf(dScn.choiceB_EN, sizeof(dScn.choiceB_EN), "STICK TO PLAN");

            dScn.scenario = { DecisionType::GO, 75.0f, 60.0f, 80.0f, 90.0f, 60.0f, 50.0f, 85.0f, 85.0f, 40.0f };
            break;
        }

        case ArchetypeID::LIMITED_TIME_PURCHASE: {
            dScn.category = ScenarioCategory::PURCHASE;
            dScn.tags[0] = "DISCOUNT"; dScn.tags[1] = "PURCHASE"; dScn.tags[2] = "TEMPTATION"; dScn.tags[3] = "LIMITED_TIME";

            snprintf(dScn.titleCN, sizeof(dScn.titleCN), "限时优惠心仪好物");
            snprintf(dScn.descCN, sizeof(dScn.descCN), "你看中很久的%s今天限时七折优惠，不过你手头其实有可替代品。", iCN);
            snprintf(dScn.titleEN, sizeof(dScn.titleEN), "LIMITED TIME DISCOUNT");
            snprintf(dScn.descEN, sizeof(dScn.descEN), "The %s you wanted is 30%% off today. Your current gear works fine.", iEN);

            snprintf(dScn.choiceA_CN, sizeof(dScn.choiceA_CN), "果断购买 (BUY)");
            snprintf(dScn.choiceB_CN, sizeof(dScn.choiceB_CN), "理性克制 (WAIT)");
            snprintf(dScn.choiceA_EN, sizeof(dScn.choiceA_EN), "BUY WITH DISCOUNT");
            snprintf(dScn.choiceB_EN, sizeof(dScn.choiceB_EN), "PASS & SAVE");

            dScn.scenario = { DecisionType::GET, 30.0f, 70.0f, 10.0f, 70.0f, 20.0f, 10.0f, 30.0f, 85.0f, 50.0f };
            break;
        }

        case ArchetypeID::DUPLICATE_PURCHASE: {
            dScn.category = ScenarioCategory::PURCHASE;
            dScn.tags[0] = "UPGRADE"; dScn.tags[1] = "PURCHASE"; dScn.tags[2] = "QUALITY"; dScn.tags[3] = "DESIRE";

            snprintf(dScn.titleCN, sizeof(dScn.titleCN), "旧物完好换新选购");
            snprintf(dScn.descCN, sizeof(dScn.descCN), "最新推出的%s性能与外观极佳，你手头的旧款还能正常使用但略显陈旧。", iCN);
            snprintf(dScn.titleEN, sizeof(dScn.titleEN), "EQUIPMENT UPGRADE");
            snprintf(dScn.descEN, sizeof(dScn.descEN), "The new %s offers major aesthetic upgrade, though your old one works fine.", iEN);

            snprintf(dScn.choiceA_CN, sizeof(dScn.choiceA_CN), "升级换新 (UPGRADE)");
            snprintf(dScn.choiceB_CN, sizeof(dScn.choiceB_CN), "继续用旧 (KEEP)");
            snprintf(dScn.choiceA_EN, sizeof(dScn.choiceA_EN), "UPGRADE TO NEW");
            snprintf(dScn.choiceB_EN, sizeof(dScn.choiceB_EN), "KEEP USING OLD");

            dScn.scenario = { DecisionType::GET, 40.0f, 80.0f, 10.0f, 80.0f, 30.0f, 10.0f, 40.0f, 90.0f, 30.0f };
            break;
        }

        case ArchetypeID::SOCIAL_INVITATION: {
            dScn.category = ScenarioCategory::SOCIAL;
            dScn.tags[0] = "SOCIAL"; dScn.tags[1] = "PARTY"; dScn.tags[2] = "SOLITUDE"; dScn.tags[3] = "FRIEND";

            snprintf(dScn.titleCN, sizeof(dScn.titleCN), "热闹社交局邀约");
            snprintf(dScn.descCN, sizeof(dScn.descCN), "%s临时邀请你参加今晚的%s，你原本计划%s。", pCN, eCN, planCN);
            snprintf(dScn.titleEN, sizeof(dScn.titleEN), "SOCIAL INVITATION");
            snprintf(dScn.descEN, sizeof(dScn.descEN), "%s invites you to a %s tonight, while you planned on %s.", pEN, eEN, planEN);

            snprintf(dScn.choiceA_CN, sizeof(dScn.choiceA_CN), "欣然前往 (JOIN)");
            snprintf(dScn.choiceB_CN, sizeof(dScn.choiceB_CN), "独处休息 (HOME)");
            snprintf(dScn.choiceA_EN, sizeof(dScn.choiceA_EN), "JOIN THE PARTY");
            snprintf(dScn.choiceB_EN, sizeof(dScn.choiceB_EN), "STAY HOME REST");

            dScn.scenario = { DecisionType::GO, 40.0f, 20.0f, 50.0f, 60.0f, 90.0f, 50.0f, 40.0f, 80.0f, 40.0f };
            break;
        }

        case ArchetypeID::KNOWN_VS_UNKNOWN: {
            dScn.category = ScenarioCategory::RISK;
            dScn.tags[0] = "EXPLORATION"; dScn.tags[1] = "RISK"; dScn.tags[2] = "NOVELTY"; dScn.tags[3] = "ROUTINE";

            snprintf(dScn.titleCN, sizeof(dScn.titleCN), "熟悉路线 vs 未知探险");
            snprintf(dScn.descCN, sizeof(dScn.descCN), "前往目的地时，%s建议尝试一条从没走过的神秘小径，可能遇到意外美景也可能迷路。", pCN);
            snprintf(dScn.titleEN, sizeof(dScn.titleEN), "FAMILIAR VS UNKNOWN");
            snprintf(dScn.descEN, sizeof(dScn.descEN), "%s suggests taking an unexplored shortcut. Potential scenic view or getting lost.", pEN);

            snprintf(dScn.choiceA_CN, sizeof(dScn.choiceA_CN), "尝试未知 (EXPLORE)");
            snprintf(dScn.choiceB_CN, sizeof(dScn.choiceB_CN), "走熟悉路 (SAFE)");
            snprintf(dScn.choiceA_EN, sizeof(dScn.choiceA_EN), "EXPLORE UNKNOWN");
            snprintf(dScn.choiceB_EN, sizeof(dScn.choiceB_EN), "TAKE SAFE ROUTE");

            dScn.scenario = { DecisionType::RISK, 70.0f, 10.0f, 40.0f, 95.0f, 50.0f, 40.0f, 80.0f, 85.0f, 30.0f };
            break;
        }

        default: {
            // 通用备用 Archetype
            dScn.category = ScenarioCategory::TRAVEL;
            dScn.tags[0] = "TRAVEL"; dScn.tags[1] = "CHOICE"; dScn.tags[2] = "FRIEND"; dScn.tags[3] = "EXPERIENCE";

            snprintf(dScn.titleCN, sizeof(dScn.titleCN), "现实决策考验");
            snprintf(dScn.descCN, sizeof(dScn.descCN), "%s邀请你一起%s，需要你做出一项重要的选择。", pCN, eCN);
            snprintf(dScn.titleEN, sizeof(dScn.titleEN), "REAL WORLD DECISION");
            snprintf(dScn.descEN, sizeof(dScn.descEN), "%s invites you to join %s. Make your choice.", pEN, eEN);

            snprintf(dScn.choiceA_CN, sizeof(dScn.choiceA_CN), "同意参与 (YES)");
            snprintf(dScn.choiceB_CN, sizeof(dScn.choiceB_CN), "保持原状 (NO)");
            snprintf(dScn.choiceA_EN, sizeof(dScn.choiceA_EN), "ACCEPT OFFER");
            snprintf(dScn.choiceB_EN, sizeof(dScn.choiceB_EN), "DECLINE OFFER");

            dScn.scenario = { DecisionType::GO, 60.0f, 40.0f, 50.0f, 80.0f, 60.0f, 40.0f, 60.0f, 80.0f, 50.0f };
            break;
        }
    }

    return dScn;
}
