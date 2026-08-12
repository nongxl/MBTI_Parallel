#include "DecisionRecord.h"
#include <cstring>
#include <cstdio>

#ifdef ARDUINO
#include <Arduino.h>
#include <Preferences.h>
static Preferences recPrefs;
#endif

static DecisionRecordStore g_store;

void initDecisionRecordStore() {
    g_store.totalRecords = 0;
    g_store.headIndex = 0;
    memset(g_store.items, 0, sizeof(g_store.items));
}

void addDecisionRecord(const char* scenarioId, const char* archetypeId, ScenarioCategory category, Decision choice, const DecisionProfile& profile, MBTIType closest, MBTIType biggestDiff) {
    int idx = g_store.headIndex;

    snprintf(g_store.items[idx].scenarioId, sizeof(g_store.items[idx].scenarioId), "%s", scenarioId ? scenarioId : "SCN_000");
    snprintf(g_store.items[idx].archetypeId, sizeof(g_store.items[idx].archetypeId), "%s", archetypeId ? archetypeId : "CUSTOM");
    g_store.items[idx].category = category;
    g_store.items[idx].choice = choice;
    g_store.items[idx].profile = profile;
    g_store.items[idx].closestMBTI = closest;
    g_store.items[idx].biggestDiffMBTI = biggestDiff;
    g_store.items[idx].timestamp = 0;

    g_store.headIndex = (g_store.headIndex + 1) % MAX_DECISION_RECORDS;
    if (g_store.totalRecords < MAX_DECISION_RECORDS) {
        g_store.totalRecords++;
    }

#ifdef ARDUINO
    recPrefs.begin("mbti_records", false);
    recPrefs.putInt("total", g_store.totalRecords);
    recPrefs.putInt("head", g_store.headIndex);

    char keyBuf[16];
    snprintf(keyBuf, sizeof(keyBuf), "r_%d", idx);
    recPrefs.putBytes(keyBuf, &g_store.items[idx], sizeof(DecisionRecordItem));
    recPrefs.end();
#endif
}

void loadDecisionRecordsFromNVS() {
    initDecisionRecordStore();

#ifdef ARDUINO
    recPrefs.begin("mbti_records", true);
    g_store.totalRecords = recPrefs.getInt("total", 0);
    g_store.headIndex = recPrefs.getInt("head", 0);

    if (g_store.totalRecords > MAX_DECISION_RECORDS) {
        g_store.totalRecords = MAX_DECISION_RECORDS;
    }

    char keyBuf[16];
    for (int i = 0; i < g_store.totalRecords; ++i) {
        snprintf(keyBuf, sizeof(keyBuf), "r_%d", i);
        size_t len = recPrefs.getBytes(keyBuf, &g_store.items[i], sizeof(DecisionRecordItem));
        if (len != sizeof(DecisionRecordItem)) {
            // 清理非法条目
            memset(&g_store.items[i], 0, sizeof(DecisionRecordItem));
        }
    }
    recPrefs.end();
#endif
}

const DecisionRecordStore& getDecisionRecordStore() {
    return g_store;
}
