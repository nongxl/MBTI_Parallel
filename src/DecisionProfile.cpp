#include "DecisionProfile.h"
#include <cmath>
#include <algorithm>

#ifdef ARDUINO
#include <Arduino.h>
#include <Preferences.h>
static Preferences histPrefs;
#endif

DecisionProfile calculateDecisionProfile(const Scenario& scenario, Decision userChoice) {
    DecisionProfile p;

    float choiceMultiplier = 1.0f;
    if (userChoice == Decision::YES) {
        choiceMultiplier = 1.2f;
    } else if (userChoice == Decision::NO) {
        choiceMultiplier = 0.4f;
    } else {
        choiceMultiplier = 0.8f;
    }

    p.risk = std::min(100.0f, std::max(0.0f, scenario.risk * choiceMultiplier));
    p.novelty = std::min(100.0f, std::max(0.0f, scenario.novelty * choiceMultiplier));
    p.social = std::min(100.0f, std::max(0.0f, scenario.social * choiceMultiplier));
    p.logic = std::min(100.0f, std::max(0.0f, scenario.practicalValue * choiceMultiplier));
    p.planning = std::min(100.0f, std::max(0.0f, (100.0f - scenario.uncertainty) * choiceMultiplier));
    p.practicality = std::min(100.0f, std::max(0.0f, scenario.practicalValue * choiceMultiplier));

    return p;
}

MBTIType findClosestMBTI(const DecisionProfile& userProfile, float& outSimilarity) {
    MBTIType bestType = MBTIType::INTP;
    float minDistance = 99999.0f;

    for (int i = 0; i < MBTI_COUNT; ++i) {
        const PersonalityProfile& prof = getMBTIProfile(static_cast<MBTIType>(i));

        float dRisk = (userProfile.risk - prof.risk);
        float dNov  = (userProfile.novelty - prof.novelty);
        float dLog  = (userProfile.logic - prof.logic);
        float dSoc  = (userProfile.social - prof.social);
        float dPla  = (userProfile.planning - prof.planning);
        float dPra  = (userProfile.practicality - prof.practicality);

        float dist = sqrtf(dRisk*dRisk + dNov*dNov + dLog*dLog + dSoc*dSoc + dPla*dPla + dPra*dPra);

        if (dist < minDistance) {
            minDistance = dist;
            bestType = static_cast<MBTIType>(i);
        }
    }

    // 映射距离为 0% ~ 100% 相似度
    float maxPossibleDist = sqrtf(6.0f * 100.0f * 100.0f);
    outSimilarity = std::max(0.0f, (1.0f - (minDistance / maxPossibleDist)) * 100.0f);

    return bestType;
}

void loadUserHistoryFromNVS(UserHistoryStats& stats) {
#ifdef ARDUINO
    histPrefs.begin("mbti_hist", false);
    stats.totalPlays = histPrefs.getUShort("plays", 0);
    stats.yesCount = histPrefs.getUShort("yes", 0);
    stats.noCount = histPrefs.getUShort("no", 0);
    stats.maybeCount = histPrefs.getUShort("maybe", 0);

    stats.cumulativeRisk = histPrefs.getFloat("c_risk", 50.0f);
    stats.cumulativeNovelty = histPrefs.getFloat("c_nov", 50.0f);
    stats.cumulativeLogic = histPrefs.getFloat("c_log", 50.0f);
    stats.cumulativeSocial = histPrefs.getFloat("c_soc", 50.0f);
    stats.cumulativePlanning = histPrefs.getFloat("c_pla", 50.0f);
    stats.cumulativePracticality = histPrefs.getFloat("c_pra", 50.0f);
#else
    stats.totalPlays = 0;
    stats.yesCount = 0;
    stats.noCount = 0;
    stats.maybeCount = 0;

    stats.cumulativeRisk = 50.0f;
    stats.cumulativeNovelty = 50.0f;
    stats.cumulativeLogic = 50.0f;
    stats.cumulativeSocial = 50.0f;
    stats.cumulativePlanning = 50.0f;
    stats.cumulativePracticality = 50.0f;
#endif

    DecisionProfile cProf = {
        stats.cumulativeRisk, stats.cumulativeNovelty, stats.cumulativeLogic,
        stats.cumulativeSocial, stats.cumulativePlanning, stats.cumulativePracticality
    };
    stats.dominantMBTI = findClosestMBTI(cProf, stats.dominantSimilarity);
}

void saveUserHistoryToNVS(const UserHistoryStats& stats) {
#ifdef ARDUINO
    histPrefs.putUShort("plays", stats.totalPlays);
    histPrefs.putUShort("yes", stats.yesCount);
    histPrefs.putUShort("no", stats.noCount);
    histPrefs.putUShort("maybe", stats.maybeCount);

    histPrefs.putFloat("c_risk", stats.cumulativeRisk);
    histPrefs.putFloat("c_nov", stats.cumulativeNovelty);
    histPrefs.putFloat("c_log", stats.cumulativeLogic);
    histPrefs.putFloat("c_soc", stats.cumulativeSocial);
    histPrefs.putFloat("c_pla", stats.cumulativePlanning);
    histPrefs.putFloat("c_pra", stats.cumulativePracticality);
#endif
}

void recordUserDecisionToHistory(UserHistoryStats& stats, const DecisionProfile& currentProfile, Decision userChoice) {
    stats.totalPlays++;
    if (userChoice == Decision::YES) stats.yesCount++;
    else if (userChoice == Decision::NO) stats.noCount++;
    else stats.maybeCount++;

    float n = (float)stats.totalPlays;
    if (n <= 1.0f) {
        stats.cumulativeRisk = currentProfile.risk;
        stats.cumulativeNovelty = currentProfile.novelty;
        stats.cumulativeLogic = currentProfile.logic;
        stats.cumulativeSocial = currentProfile.social;
        stats.cumulativePlanning = currentProfile.planning;
        stats.cumulativePracticality = currentProfile.practicality;
    } else {
        float prevW = (n - 1.0f) / n;
        float currW = 1.0f / n;

        stats.cumulativeRisk = stats.cumulativeRisk * prevW + currentProfile.risk * currW;
        stats.cumulativeNovelty = stats.cumulativeNovelty * prevW + currentProfile.novelty * currW;
        stats.cumulativeLogic = stats.cumulativeLogic * prevW + currentProfile.logic * currW;
        stats.cumulativeSocial = stats.cumulativeSocial * prevW + currentProfile.social * currW;
        stats.cumulativePlanning = stats.cumulativePlanning * prevW + currentProfile.planning * currW;
        stats.cumulativePracticality = stats.cumulativePracticality * prevW + currentProfile.practicality * currW;
    }

    DecisionProfile cProf = {
        stats.cumulativeRisk, stats.cumulativeNovelty, stats.cumulativeLogic,
        stats.cumulativeSocial, stats.cumulativePlanning, stats.cumulativePracticality
    };
    stats.dominantMBTI = findClosestMBTI(cProf, stats.dominantSimilarity);

    // 0ms 静默持久化存盘入 ESP32 NVS Flash
    saveUserHistoryToNVS(stats);
}
