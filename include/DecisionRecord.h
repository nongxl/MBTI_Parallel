#ifndef DECISION_RECORD_H
#define DECISION_RECORD_H

#include "MBTIProfile.h"
#include "ScenarioPool.h"
#include "DecisionProfile.h"
#include <cstdint>

constexpr int MAX_DECISION_RECORDS = 500;

struct DecisionRecordItem {
    char scenarioId[16];
    char archetypeId[32]; // 【Phase 6B】底层 Archetype 机制标识
    ScenarioCategory category;
    Decision choice;
    DecisionProfile profile;
    MBTIType closestMBTI;
    MBTIType biggestDiffMBTI;
    uint32_t timestamp;
};

struct DecisionRecordStore {
    int totalRecords;
    int headIndex; // 环形队列游标
    DecisionRecordItem items[MAX_DECISION_RECORDS];
};

// 内存单例存储初始化
void initDecisionRecordStore();

// 追加一次决策记录 (追加至内存与 ESP32 NVS)
void addDecisionRecord(const char* scenarioId, const char* archetypeId, ScenarioCategory category, Decision choice, const DecisionProfile& profile, MBTIType closest, MBTIType biggestDiff);

// 从 ESP32 NVS 加载历史记录
void loadDecisionRecordsFromNVS();

// 获取当前记录存储句柄
const DecisionRecordStore& getDecisionRecordStore();

#endif // DECISION_RECORD_H
