#ifndef RADAR_CHART_H
#define RADAR_CHART_H

#ifdef ARDUINO
#include <M5Cardputer.h>
#endif
#include <cstdint>

struct RadarData {
    float risk;         // 0 ~ 100
    float novelty;      // 0 ~ 100
    float logic;        // 0 ~ 100
    float social;       // 0 ~ 100
    float planning;     // 0 ~ 100
    float practicality; // 0 ~ 100
};

#ifdef ARDUINO
// 在 M5Canvas 上绘制六维极坐标雷达图 (支持越界保护与双语轴标签)
void drawRadarChart(M5Canvas& canvas, int centerX, int centerY, int radius, const RadarData& data, uint16_t lineColor, uint16_t fillColor, bool showLabels = true, bool isCN = false);
#endif

#endif // RADAR_CHART_H
