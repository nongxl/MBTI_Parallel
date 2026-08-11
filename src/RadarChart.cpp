#include "RadarChart.h"
#include <cmath>
#include <cstring>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

void drawRadarChart(M5Canvas& canvas, int centerX, int centerY, int radius, const RadarData& data, uint16_t lineColor, uint16_t fillColor, bool drawLabels, bool isCN) {
    float values[6] = {
        data.novelty,
        data.risk,
        data.planning,
        data.practicality,
        data.logic,
        data.social
    };

    const char* labelsEN[6] = { "NOVELTY", "RISK", "PLANNING", "PRACTICAL", "LOGIC", "SOCIAL" };
    const char* labelsCN[6] = { "新鲜", "风险", "计划", "实用", "逻辑", "社交" };

    // 1. 计算数据多边形顶点
    int vx[6], vy[6];
    for (int i = 0; i < 6; ++i) {
        float val = std::min(100.0f, std::max(0.0f, values[i]));
        float r = radius * (val / 100.0f);
        float angle = -M_PI / 2.0f + i * (M_PI / 3.0f);
        vx[i] = centerX + static_cast<int>(r * cosf(angle));
        vy[i] = centerY + static_cast<int>(r * sinf(angle));
    }

    // 2. 【图层 1: 极淡低调透光蓝色充能内衬】(RGB565: 0x00A5 极淡微光深青蓝，绝不遮盖任何背景文字与线段)
    uint16_t cyanFillColor = 0x00A5;
    for (int i = 0; i < 6; ++i) {
        int next = (i + 1) % 6;
        canvas.fillTriangle(centerX, centerY, vx[i], vy[i], vx[next], vy[next], cyanFillColor);
    }

    // 3. 【图层 2: 坐标系同心网格线与放射轴】(完全悬浮在极淡蓝色填充块上方)
    for (int layer = 1; layer <= 3; ++layer) {
        float r = radius * (layer / 3.0f);
        int px[6], py[6];
        for (int i = 0; i < 6; ++i) {
            float angle = -M_PI / 2.0f + i * (M_PI / 3.0f);
            px[i] = centerX + static_cast<int>(r * cosf(angle));
            py[i] = centerY + static_cast<int>(r * sinf(angle));
        }
        for (int i = 0; i < 6; ++i) {
            canvas.drawLine(px[i], py[i], px[(i + 1) % 6], py[(i + 1) % 6], DARKGREY);
        }
    }

    int outerX[6], outerY[6];
    for (int i = 0; i < 6; ++i) {
        float angle = -M_PI / 2.0f + i * (M_PI / 3.0f);
        outerX[i] = centerX + static_cast<int>(radius * cosf(angle));
        outerY[i] = centerY + static_cast<int>(radius * sinf(angle));
        canvas.drawLine(centerX, centerY, outerX[i], outerY[i], DARKGREY);
    }

    // 4. 【图层 3: 前景加粗防断线多边形轮廓线】
    for (int i = 0; i < 6; ++i) {
        int next = (i + 1) % 6;
        canvas.drawLine(vx[i], vy[i], vx[next], vy[next], lineColor);
        canvas.drawLine(vx[i] + 1, vy[i], vx[next] + 1, vy[next], lineColor);
        canvas.drawLine(vx[i], vy[i] + 1, vx[next], vy[next] + 1, lineColor);
    }

    // 5. 绘制白色加固顶点
    for (int i = 0; i < 6; ++i) {
        canvas.fillCircle(vx[i], vy[i], 2, WHITE);
    }

    // 6. 绘制顶点标签 (拉开间距与防边界碰撞)
    if (drawLabels) {
        if (isCN) {
            canvas.setFont(&fonts::efontCN_12);
            canvas.setTextSize(1);
        } else {
            canvas.setFont(&fonts::Font0);
            canvas.setTextSize(1);
        }

        canvas.setTextColor(CYAN, BLACK);
        for (int i = 0; i < 6; ++i) {
            float angle = -M_PI / 2.0f + i * (M_PI / 3.0f);
            int labelDist = radius + (isCN ? 12 : 14);
            int labelX = centerX + static_cast<int>(labelDist * cosf(angle));
            int labelY = centerY + static_cast<int>(labelDist * sinf(angle));

            const char* txt = isCN ? labelsCN[i] : labelsEN[i];
            int textLen = isCN ? (strlen(txt) / 3 * 12) : (strlen(txt) * 6);

            if (i == 0) { // NOVELTY
                labelX -= textLen / 2;
                labelY -= 10;
            } else if (i == 1) { // RISK
                labelY -= 6;
            } else if (i == 2) { // PLANNING
                labelY += 6;
            } else if (i == 3) { // PRACTICALITY
                labelX -= textLen / 2;
                labelY += 2;
            } else if (i == 4) { // LOGIC
                labelX -= textLen;
                labelY += 6;
            } else if (i == 5) { // SOCIAL
                labelX -= textLen;
                labelY -= 6;
            }

            if (labelX + textLen > 235) {
                labelX = 235 - textLen;
            }
            if (labelX < 2) {
                labelX = 2;
            }

            canvas.setCursor(labelX, labelY);
            canvas.print(txt);
        }
    }
}
