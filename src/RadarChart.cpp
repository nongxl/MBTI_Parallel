#include "RadarChart.h"
#include <cmath>
#include <cstring>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

// 真实 RGB565 像素级平滑 Alpha 物理混合算法
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
                    canvas.drawPixel(x, y, 0x016A); 
                } else {
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

    // 2. 【图层 1: 真实平滑 Alpha 磨砂玻璃内衬】
    uint16_t cyanAlphaFg = 0x03FF;
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

    // 6. 绘制顶点标签
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
            int labelDist = radius + (isCN ? 8 : 10);
            int labelX = centerX + static_cast<int>(labelDist * cosf(angle));
            int labelY = centerY + static_cast<int>(labelDist * sinf(angle));

            const char* txt = isCN ? labelsCN[i] : labelsEN[i];
            int textLen = isCN ? (strlen(txt) / 3 * 12) : (strlen(txt) * 6);

            if (i == 0) {
                labelX -= textLen / 2;
                labelY -= 9;
            } else if (i == 1) {
                labelY -= 4;
            } else if (i == 2) {
                labelY += 4;
            } else if (i == 3) {
                labelX -= textLen / 2;
                labelY += 1;
            } else if (i == 4) {
                labelX -= textLen;
                labelY += 4;
            } else if (i == 5) {
                labelX -= textLen;
                labelY -= 4;
            }

            if (labelX + textLen > 235) labelX = 235 - textLen;
            if (labelX < 2) labelX = 2;

            canvas.setCursor(labelX, labelY);
            canvas.print(txt);
        }
    }
}

// 【Phase 6A】双雷达叠加多边形绘制实现 (实线 YOU 绿 vs 紫色实线+实心点 MBTI 紫)
void drawDualRadarChart(M5Canvas& canvas, int centerX, int centerY, int radius, const RadarData& userData, const RadarData& mbtiData, const char* mbtiName, bool drawLabels, bool isCN) {
    // 1. 优先绘制背景网格与基础设施 (基于 YOU 绘制背景 Alpha)
    drawRadarChart(canvas, centerX, centerY, radius, userData, GREEN, DARKCYAN, drawLabels, isCN);

    // 2. 计算 MBTI Archetype 数据顶点
    float mbtiValues[6] = {
        mbtiData.novelty,
        mbtiData.risk,
        mbtiData.planning,
        mbtiData.practicality,
        mbtiData.logic,
        mbtiData.social
    };

    int mx[6], my[6];
    for (int i = 0; i < 6; ++i) {
        float val = std::min(100.0f, std::max(0.0f, mbtiValues[i]));
        float r = radius * (val / 100.0f);
        float angle = -M_PI / 2.0f + i * (M_PI / 3.0f);
        mx[i] = centerX + static_cast<int>(r * cosf(angle));
        my[i] = centerY + static_cast<int>(r * sinf(angle));
    }

    // 3. 【图层 4: 叠加 MBTI 紫色实线轮廓 (MAGENTA)】
    uint16_t mbtiColor = MAGENTA;
    for (int i = 0; i < 6; ++i) {
        int next = (i + 1) % 6;
        canvas.drawLine(mx[i], my[i], mx[next], my[next], mbtiColor);
        canvas.drawLine(mx[i] + 1, my[i], mx[next] + 1, my[next], mbtiColor);
    }

    // 4. 绘制 MBTI 实体紫色圆点
    for (int i = 0; i < 6; ++i) {
        canvas.fillCircle(mx[i], my[i], 2, mbtiColor);
    }

    // 5. 【图层 5: 右侧图表区顶部对称居中图例 (─ YOU   ─ MBTI)】
    canvas.setFont(&fonts::Font0);
    canvas.setTextSize(1);
    
    // 实线绿线表示 YOU (X = 128 ~ 140, Y = 28)
    canvas.drawLine(128, 28, 140, 28, GREEN);
    canvas.fillCircle(134, 28, 2, WHITE);
    canvas.setTextColor(GREEN, BLACK);
    canvas.setCursor(144, 25);
    canvas.print("YOU");

    // 实线紫线表示 MBTI (X = 176 ~ 188, Y = 28)
    canvas.drawLine(176, 28, 188, 28, mbtiColor);
    canvas.fillCircle(182, 28, 2, mbtiColor);
    canvas.setTextColor(mbtiColor, BLACK);
    canvas.setCursor(192, 25);
    canvas.print(mbtiName ? mbtiName : "MBTI");
}
