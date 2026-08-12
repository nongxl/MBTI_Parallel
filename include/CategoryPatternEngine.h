#ifndef CATEGORY_PATTERN_ENGINE_H
#define CATEGORY_PATTERN_ENGINE_H

#include "ScenarioPool.h"
#include "MBTIProfile.h"
#include "DecisionRecord.h"

enum class PatternConfidence {
    LOW_SAMPLE,  // 样本不足 (<3 次)
    MIXED,       // 模式不明显 (分散无主导)
    MEDIUM,      // 中度倾向 (3~4 次，频次靠前)
    HIGH         // 高度共振 (5+ 次，绝对主导)
};

struct CategoryPattern {
    ScenarioCategory category;
    int sampleCount;
    int mbtiDistribution[MBTI_COUNT];
    MBTIType dominantMBTI;
    PatternConfidence confidence;
};

struct ContextualPersonalitySummary {
    int validPatternsCount;
    CategoryPattern patterns[12];
};

// 计算 12 大 Category 跨情境模式分析
ContextualPersonalitySummary computeContextualPatterns(const DecisionRecordStore& store);

// 获取 PatternConfidence 中英文解释
const char* getConfidenceTextCN(PatternConfidence conf);
const char* getConfidenceTextEN(PatternConfidence conf);

#endif // CATEGORY_PATTERN_ENGINE_H
