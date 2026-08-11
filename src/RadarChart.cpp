#include "RadarChart.h"

#ifdef ARDUINO
#include <cmath>

static const float M_PI_F = 3.14159265358979323846f;

void drawRadarChart(M5Canvas& canvas, int centerX, int centerY, int radius, const RadarData& data, uint16_t lineColor, uint16_t fillColor, bool showLabels) {
    float values[6] = {
        data.novelty,
        data.risk,
        data.planning,
        data.practicality,
        data.logic,
        data.social
    };

    const char* labels[6] = { "N", "R", "PL", "PR", "L", "S" };

    // 计算 6 个轴位的角度 (以 -90 度为顶点开始)
    float angles[6];
    for (int i = 0; i < 6; ++i) {
        angles[i] = -M_PI_F / 2.0f + i * (M_PI_F / 3.0f);
    }

    // 1. 绘制 3 层同心六边形背景网格
    for (int level = 1; level <= 3; ++level) {
        float r = radius * (level / 3.0f);
        uint16_t gridColor = (level == 3) ? DARKGREY : DARKCYAN;
        for (int i = 0; i < 6; ++i) {
            int next = (i + 1) % 6;
            int x1 = centerX + (int)(r * std::cos(angles[i]));
            int y1 = centerY + (int)(r * std::sin(angles[i]));
            int x2 = centerX + (int)(r * std::cos(angles[next]));
            int y2 = centerY + (int)(r * std::sin(angles[next]));
            canvas.drawLine(x1, y1, x2, y2, gridColor);
        }
    }

    // 2. 绘制 6 条从中心向外射出的轴线
    for (int i = 0; i < 6; ++i) {
        int x = centerX + (int)(radius * std::cos(angles[i]));
        int y = centerY + (int)(radius * std::sin(angles[i]));
        canvas.drawLine(centerX, centerY, x, y, DARKGREY);

        // 绘制标签
        if (showLabels) {
            int labelX = centerX + (int)((radius + 10) * std::cos(angles[i])) - 4;
            int labelY = centerY + (int)((radius + 10) * std::sin(angles[i])) - 4;
            canvas.setTextSize(1);
            canvas.setTextColor(YELLOW, BLACK);
            canvas.setCursor(labelX, labelY);
            canvas.print(labels[i]);
        }
    }

    // 3. 计算数据多边形顶点并绘制闭合边线
    int dataX[6];
    int dataY[6];
    for (int i = 0; i < 6; ++i) {
        float val = values[i];
        if (val < 5.0f) val = 5.0f; // 保证最小可见度
        if (val > 100.0f) val = 100.0f;

        float r = radius * (val / 100.0f);
        dataX[i] = centerX + (int)(r * std::cos(angles[i]));
        dataY[i] = centerY + (int)(r * std::sin(angles[i]));
    }

    for (int i = 0; i < 6; ++i) {
        int next = (i + 1) % 6;
        canvas.drawLine(dataX[i], dataY[i], dataX[next], dataY[next], lineColor);
        // 加粗边线效果
        canvas.drawPixel(dataX[i], dataY[i], lineColor);
        canvas.fillCircle(dataX[i], dataY[i], 2, lineColor);
    }
}
#endif
