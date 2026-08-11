#include "RadarChart.h"
#include <cmath>
#include <cstring>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

constexpr int RADAR_AXIS_COUNT = 6;

#ifdef ARDUINO

void drawRadarChart(M5Canvas& canvas, int centerX, int centerY, int radius, const RadarData& data, uint16_t color, uint16_t gridColor, bool showAxisLabels, bool isCN) {
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

    // 2. 绘制 6 条轴线与轴标签 (英文全称 或 中文标签)
    const float values[RADAR_AXIS_COUNT] = {
        data.novelty,
        data.risk,
        data.planning,
        data.practicality,
        data.logic,
        data.social
    };

    const char* axisLabelsEN[RADAR_AXIS_COUNT] = { "NOVELTY", "RISK", "PLANNING", "PRACTICAL", "LOGIC", "SOCIAL" };
    const char* axisLabelsCN[RADAR_AXIS_COUNT] = { "新奇", "风险", "计划", "实用", "逻辑", "社交" };

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
            canvas.setTextColor(LIGHTGREY, BLACK);

            const char* labelText = isCN ? axisLabelsCN[i] : axisLabelsEN[i];
            if (isCN) {
                canvas.setFont(&fonts::efontCN_12);
                canvas.setTextSize(1);
            } else {
                canvas.setFont(&fonts::Font0);
                canvas.setTextSize(1);
            }

            int charWidth = isCN ? 12 : 6;
            int textLen = (int)strlen(labelText) * (isCN ? 6 : charWidth); // 估计宽度

            // 针对 6 个极坐标角度进行全称精细 offset 与防越界卡死保护
            int labelX = centerX;
            int labelY = centerY;
            switch (i) {
                case 0: // 90° Top (NOVELTY / 新奇)
                    labelX = centerX - (textLen / 2);
                    labelY = centerY - radius - 12;
                    break;
                case 1: // 30° Top-Right (RISK / 风险)
                    labelX = centerX + radius + 3;
                    labelY = centerY - 14;
                    break;
                case 2: // 330° Bottom-Right (PLANNING / 计划)
                    labelX = centerX + radius + 3;
                    labelY = centerY + 3;
                    break;
                case 3: // 270° Bottom (PRACTICAL / 实用)
                    labelX = centerX - (textLen / 2);
                    labelY = centerY + radius + 3;
                    break;
                case 4: // 210° Bottom-Left (LOGIC / 逻辑)
                    labelX = centerX - radius - textLen - 2;
                    labelY = centerY + 3;
                    break;
                case 5: // 150° Top-Left (SOCIAL / 社交)
                    labelX = centerX - radius - textLen - 2;
                    labelY = centerY - 14;
                    break;
            }

            // 【越界卡死保护】若文本右侧延伸超出 X=235，强制左移靠右对齐，绝不溢出被自动折行到最左侧！
            if (labelX + textLen > 235) {
                labelX = 235 - textLen;
            }
            if (labelX < 2) {
                labelX = 2;
            }

            canvas.setCursor(labelX, labelY);
            canvas.print(labelText);
        }
    }

    // 3. 双层发光极客雷达连线 (Dual-Layer Cyber Glow Polygon Line)
    uint16_t glowColor = DARKCYAN;
    if (color == GREEN) glowColor = DARKGREEN;
    else if (color == RED) glowColor = MAROON;

    for (int i = 0; i < RADAR_AXIS_COUNT; ++i) {
        int nextIdx = (i + 1) % RADAR_AXIS_COUNT;
        canvas.drawLine(glowX[i], glowY[i], glowX[nextIdx], glowY[nextIdx], glowColor);
    }

    for (int i = 0; i < RADAR_AXIS_COUNT; ++i) {
        int nextIdx = (i + 1) % RADAR_AXIS_COUNT;
        canvas.drawLine(dataX[i], dataY[i], dataX[nextIdx], dataY[nextIdx], color);
        canvas.fillCircle(dataX[i], dataY[i], 2, WHITE);
    }
}

#endif
