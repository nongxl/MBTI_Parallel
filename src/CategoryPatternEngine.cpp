#include "CategoryPatternEngine.h"
#include <cstring>
#include <algorithm>

ContextualPersonalitySummary computeContextualPatterns(const DecisionRecordStore& store) {
    ContextualPersonalitySummary summary;
    summary.validPatternsCount = 0;
    memset(summary.patterns, 0, sizeof(summary.patterns));

    for (int catIdx = 0; catIdx < 12; ++catIdx) {
        ScenarioCategory cat = static_cast<ScenarioCategory>(catIdx);
        CategoryPattern& pat = summary.patterns[catIdx];
        pat.category = cat;
        pat.sampleCount = 0;
        memset(pat.mbtiDistribution, 0, sizeof(pat.mbtiDistribution));
        pat.dominantMBTI = MBTIType::INTP;
        pat.confidence = PatternConfidence::LOW_SAMPLE;

        // 统计该 Category 下的决策次数与 MBTI 分布
        for (int i = 0; i < store.totalRecords; ++i) {
            if (store.items[i].category == cat) {
                pat.sampleCount++;
                int mIdx = static_cast<int>(store.items[i].closestMBTI);
                if (mIdx >= 0 && mIdx < MBTI_COUNT) {
                    pat.mbtiDistribution[mIdx]++;
                }
            }
        }

        // 判断模式与置信度
        if (pat.sampleCount < 3) {
            pat.confidence = PatternConfidence::LOW_SAMPLE;
        } else {
            int maxCount = -1;
            int secondMaxCount = -1;
            MBTIType bestType = MBTIType::INTP;

            for (int m = 0; m < MBTI_COUNT; ++m) {
                int count = pat.mbtiDistribution[m];
                if (count > maxCount) {
                    secondMaxCount = maxCount;
                    maxCount = count;
                    bestType = static_cast<MBTIType>(m);
                } else if (count > secondMaxCount) {
                    secondMaxCount = count;
                }
            }

            pat.dominantMBTI = bestType;

            // 必须占有明显优势
            float ratio = (float)maxCount / (float)pat.sampleCount;
            if (ratio >= 0.50f) {
                if (pat.sampleCount >= 5 && ratio >= 0.60f) {
                    pat.confidence = PatternConfidence::HIGH;
                } else {
                    pat.confidence = PatternConfidence::MEDIUM;
                }
            } else {
                pat.confidence = PatternConfidence::MIXED;
            }
        }

        if (pat.confidence == PatternConfidence::MEDIUM || pat.confidence == PatternConfidence::HIGH) {
            summary.validPatternsCount++;
        }
    }

    return summary;
}

const char* getConfidenceTextCN(PatternConfidence conf) {
    switch (conf) {
        case PatternConfidence::LOW_SAMPLE: return "样本积累中";
        case PatternConfidence::MIXED: return "决策风格多元";
        case PatternConfidence::MEDIUM: return "中度倾向发散";
        case PatternConfidence::HIGH: return "高度同频共振";
    }
    return "未知";
}

const char* getConfidenceTextEN(PatternConfidence conf) {
    switch (conf) {
        case PatternConfidence::LOW_SAMPLE: return "COLLECTING";
        case PatternConfidence::MIXED: return "DIVERSE";
        case PatternConfidence::MEDIUM: return "TENDENCY";
        case PatternConfidence::HIGH: return "STRONG";
    }
    return "UNKNOWN";
}
