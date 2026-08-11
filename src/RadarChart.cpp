#include "RadarChart.h"
#include <cmath>
#include <cstring>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

// 真实 RGB565 像素级平滑 Alpha 物理混合算法 (True Smooth Pixel-Level Alpha Blending)
// 告别点阵颗粒，读取底层画布背景色彩并按 25% 比例进行连续色彩融合，呈现柔和物理半透玻璃感
static void drawSmoothAlphaTriangle(M5Canvas& canvas, int x0, int y0, int x1, int y1, int x2, int y2, uint16_t fgColor) {
    int minX = std::min({x0, x1, x2});
    int maxX = std::max({x0, x1, x2});
    int minY = std::min({y0, y1, y2});
    int maxY = std::max({y0, y1, y2});

    minX = std::max(0, minX);
    maxX = std::min(239, maxX);
    minY = std::max(0, minY);
    maxY = std::min(134, maxY);

    float denominator = static_cast<float>((y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2));
    if (fabsf(denominator) < 0.0001f) return;

    // 前景 RGB565 解包
    uint8_t rFg = (fgColor >> 11) & 0x1F;
    uint8_t gFg = (fgColor >> 5) & 0x3F;
    uint8_t bFg = fgColor & 0x1F;

    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            float w0 = ((y1 - y2) * (x - x2) + (x2 - x1) * (y - y2)) / denominator;
            float w1 = ((y2 - y0) * (x - x2) + (x0 - x2) * (y - y2)) / denominator;
            float w2 = 1.0f - w0 - w1;

            if (w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f) {
                uint16_t bg = canvas.readPixel(x, y);
                if (bg == 0) {
                    // 黑背景上微光填色
                    canvas.drawPixel(x, y, 0x016A); 
                } else {
                    // 真实像素级 Alpha 混合 (75% 背景 + 25% 前景)
                    uint8_t rBg = (bg >> 11) & 0x1F;
                    uint8_t gBg = (bg >> 5) & 0x3F;
                    uint8_t bBg = bg & 0x1F;

                    uint8_t rOut = (rBg * 3 + rFg) >> 2;
                    uint8_t gOut = (gBg * 3 + gFg) >> 2;
                    uint8_t bOut = (bBg * 3 + bFg) >> 2;

                    uint16_t blended = (rOut << 11) | (gOut << 5) | bOut;
                    canvas.drawPixel(x, y, blended);
                }
            }
        }
    }
}

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

    // 2. 【图层 1: 真实平滑 Alpha 磨砂玻璃内衬】(完全消失点阵颗粒，75% 背景 + 25% 青蓝物理色彩融合)
    uint16_t cyanAlphaFg = 0x03FF; // 柔和极光青蓝色
    for (int i = 0; i < 6; ++i) {
        int next = (i + 1) % 6;
        drawSmoothAlphaTriangle(canvas, centerX, centerY, vx[i], vy[i], vx[next], vy[next], cyanAlphaFg);
    }

    // 3. 【图层 2: 坐标系同心网格线与放射轴】
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
