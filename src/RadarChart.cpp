#include "RadarChart.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

constexpr int RADAR_AXIS_COUNT = 6;

#ifdef ARDUINO

void drawRadarChart(M5Canvas& canvas, int centerX, int centerY, int radius, const RadarData& data, uint16_t color, uint16_t gridColor, bool showAxisLabels) {
    // 1. 绘制 3 层等距六边形网格线
    for (int r = 1; r <= 3; ++r) {
        int currentRadius = (radius * r) / 3;
        int lastX = 0, lastY = 0;
        for (int i = 0; i < RADAR_AXIS_COUNT; ++i) {
            float angle = (90.0f - i * 60.0f) * (M_PI / 180.0f);
            int x = centerX + static_cast<int>(currentRadius * cosf(angle));
            int y = centerY - static_cast<int>(currentRadius * sinf(angle));

            if (i > 0) {
                canvas.drawLine(lastX, lastY, x, y, gridColor);
            }
            lastX = x;
            lastY = y;
        }
        // 闭合六边形网格
        float firstAngle = 90.0f * (M_PI / 180.0f);
        int firstX = centerX + static_cast<int>(currentRadius * cosf(firstAngle));
        int firstY = centerY - static_cast<int>(currentRadius * sinf(firstAngle));
        canvas.drawLine(lastX, lastY, firstX, firstY, gridColor);
    }

    // 2. 绘制 6 条轴线与清晰全称轴标签 (NOVELTY, RISK, PLANNING, PRACTICAL, LOGIC, SOCIAL)
    const float values[RADAR_AXIS_COUNT] = {
        data.novelty,
        data.risk,
        data.planning,
        data.practicality,
        data.logic,
        data.social
    };

    const char* axisLabels[RADAR_AXIS_COUNT] = { "NOVELTY", "RISK", "PLANNING", "PRACTICAL", "LOGIC", "SOCIAL" };

    int dataX[RADAR_AXIS_COUNT];
    int dataY[RADAR_AXIS_COUNT];
    int glowX[RADAR_AXIS_COUNT];
    int glowY[RADAR_AXIS_COUNT];

    for (int i = 0; i < RADAR_AXIS_COUNT; ++i) {
        float angle = (90.0f - i * 60.0f) * (M_PI / 180.0f);
        int endX = centerX + static_cast<int>(radius * cosf(angle));
        int endY = centerY - static_cast<int>(radius * sinf(angle));
        canvas.drawLine(centerX, centerY, endX, endY, gridColor);

        // 数据顶点计算 (0 ~ 100 映射至 0 ~ radius)
        float normVal = values[i] / 100.0f;
        if (normVal < 0.05f) normVal = 0.05f;
        if (normVal > 1.0f) normVal = 1.0f;

        dataX[i] = centerX + static_cast<int>(radius * normVal * cosf(angle));
        dataY[i] = centerY - static_cast<int>(radius * normVal * sinf(angle));

        // 衬发光线顶点 (微缩 90% 打造内层发光沉淀感)
        glowX[i] = centerX + static_cast<int>(radius * (normVal * 0.90f) * cosf(angle));
        glowY[i] = centerY - static_cast<int>(radius * (normVal * 0.90f) * sinf(angle));

        if (showAxisLabels) {
            canvas.setTextSize(1);
            canvas.setTextColor(LIGHTGREY, BLACK);

            // 针对 6 个极坐标角度进行全称精细居中对齐 offset
            int labelX = centerX;
            int labelY = centerY;
            switch (i) {
                case 0: // 90° Top (NOVELTY)
                    labelX = centerX - 21;
                    labelY = centerY - radius - 11;
                    break;
                case 1: // 30° Top-Right (RISK)
                    labelX = centerX + radius + 3;
                    labelY = centerY - 14;
                    break;
                case 2: // 330° Bottom-Right (PLANNING)
                    labelX = centerX + radius + 3;
                    labelY = centerY + 3;
                    break;
                case 3: // 270° Bottom (PRACTICAL)
                    labelX = centerX - 24;
                    labelY = centerY + radius + 3;
                    break;
                case 4: // 210° Bottom-Left (LOGIC)
                    labelX = centerX - radius - 33;
                    labelY = centerY + 3;
                    break;
                case 5: // 150° Top-Left (SOCIAL)
                    labelX = centerX - radius - 38;
                    labelY = centerY - 14;
                    break;
            }
            canvas.setCursor(labelX, labelY);
            canvas.print(axisLabels[i]);
        }
    }

    // 3. 双层发光极客雷达连线 (Dual-Layer Cyber Glow Polygon Line)
    // 3.1 绘制内衬发光多边形线 (深浅交织衬边)
    uint16_t glowColor = DARKCYAN;
    if (color == GREEN) glowColor = DARKGREEN;
    else if (color == RED) glowColor = MAROON;

    for (int i = 0; i < RADAR_AXIS_COUNT; ++i) {
        int nextIdx = (i + 1) % RADAR_AXIS_COUNT;
        canvas.drawLine(glowX[i], glowY[i], glowX[nextIdx], glowY[nextIdx], glowColor);
    }

    // 3.2 绘制主高亮外连线与顶点加固点
    for (int i = 0; i < RADAR_AXIS_COUNT; ++i) {
        int nextIdx = (i + 1) % RADAR_AXIS_COUNT;
        canvas.drawLine(dataX[i], dataY[i], dataX[nextIdx], dataY[nextIdx], color);
        canvas.fillCircle(dataX[i], dataY[i], 2, WHITE);
    }
}

#endif
