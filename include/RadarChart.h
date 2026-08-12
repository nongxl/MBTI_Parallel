#ifndef RADAR_CHART_H
#define RADAR_CHART_H

#ifdef ARDUINO
#include <M5GFX.h>
#else
class M5Canvas {};
#endif

#include "Scenario.h"
#include <cstdint>

struct RadarData {
    float novelty;
    float risk;
    float planning;
    float practicality;
    float logic;
    float social;
};

// 绘制单多边形雷达图
void drawRadarChart(M5Canvas& canvas, int centerX, int centerY, int radius, const RadarData& data, uint16_t lineColor, uint16_t fillColor, bool drawLabels = true, bool isCN = true);

// 【Phase 6A】绘制双多边形叠加雷达图 (实线 YOU vs 虚线/点阵 MBTI Archetype)
void drawDualRadarChart(M5Canvas& canvas, int centerX, int centerY, int radius, const RadarData& userData, const RadarData& mbtiData, const char* mbtiName, bool drawLabels = true, bool isCN = true);

#endif // RADAR_CHART_H
