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

    // 1. 绘制网格线 (3 层六边形同心网格: 33%, 66%, 100%)
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

    // 2. 绘制放射线 (从中心连向 6 个顶点)
    int outerX[6], outerY[6];
    for (int i = 0; i < 6; ++i) {
        float angle = -M_PI / 2.0f + i * (M_PI / 3.0f);
        outerX[i] = centerX + static_cast<int>(radius * cosf(angle));
        outerY[i] = centerY + static_cast<int>(radius * sinf(angle));
        canvas.drawLine(centerX, centerY, outerX[i], outerY[i], DARKGREY);
    }

    // 3. 计算数据多边形顶点并填充透明图层
    int vx[6], vy[6];
    for (int i = 0; i < 6; ++i) {
        float val = std::min(100.0f, std::max(0.0f, values[i]));
        float r = radius * (val / 100.0f);
        float angle = -M_PI / 2.0f + i * (M_PI / 3.0f);
        vx[i] = centerX + static_cast<int>(r * cosf(angle));
        vy[i] = centerY + static_cast<int>(r * sinf(angle));
    }

    // 绘制多边形边框与双层发光光晕
    for (int i = 0; i < 6; ++i) {
        int next = (i + 1) % 6;
        canvas.drawLine(vx[i], vy[i], vx[next], vy[next], lineColor);
        canvas.drawLine(vx[i]+1, vy[i]+1, vx[next]+1, vy[next]+1, fillColor);
    }

    // 绘制加固点
    for (int i = 0; i < 6; ++i) {
        canvas.fillCircle(vx[i], vy[i], 2, WHITE);
    }

    // 4. 绘制顶点标签 (拉开 RISK/PLANNING 与 SOCIAL/LOGIC 的 Y 轴间距)
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

            // 针对 6 个方向进行专属间距与边界保护调整
            if (i == 0) { // NOVELTY (正上)
                labelX -= textLen / 2;
                labelY -= 10;
            } else if (i == 1) { // RISK (右上 - 向上挪 6px 拉开距离)
                labelY -= 6;
            } else if (i == 2) { // PLANNING (右下 - 向下挪 6px 拉开距离)
                labelY += 6;
            } else if (i == 3) { // PRACTICALITY (正下)
                labelX -= textLen / 2;
                labelY += 2;
            } else if (i == 4) { // LOGIC (左下 - 向下挪 6px 拉开距离)
                labelX -= textLen;
                labelY += 6;
            } else if (i == 5) { // SOCIAL (左上 - 向上挪 6px 拉开距离)
                labelX -= textLen;
                labelY -= 6;
            }

            // 限制坐标不能脱离画布 0 ~ 235 范围，防止 M5GFX 自动换行
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
