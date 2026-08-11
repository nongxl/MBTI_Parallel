#include <cstdio>
#include "UIState.h"
#include "DisplayRenderer.h"
#include "AppController.h"
#include "ScenarioGenerator.h"

#ifdef ARDUINO
#include <Arduino.h>
#include <M5Cardputer.h>
#endif

static UIContext g_uiContext;

#ifdef ARDUINO
static KeyInput readCardputerKeyboard() {
    M5Cardputer.update();
    if (M5Cardputer.Keyboard.isKeyPressed(';')) return KeyInput::UP;     // ; 键向上
    if (M5Cardputer.Keyboard.isKeyPressed('.')) return KeyInput::DOWN;   // . 键向下
    if (M5Cardputer.Keyboard.isKeyPressed(',')) return KeyInput::LEFT;   // , 键向左
    if (M5Cardputer.Keyboard.isKeyPressed('/')) return KeyInput::RIGHT;  // / 键向右
    if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) return KeyInput::ENTER;
    if (M5Cardputer.Keyboard.isKeyPressed('`') || M5Cardputer.Keyboard.isKeyPressed(KEY_BACKSPACE)) return KeyInput::BACK;

    // 亦支持方向按键或经典 WSAD 键
    if (M5Cardputer.Keyboard.isKeyPressed('w') || M5Cardputer.Keyboard.isKeyPressed('W')) return KeyInput::UP;
    if (M5Cardputer.Keyboard.isKeyPressed('s') || M5Cardputer.Keyboard.isKeyPressed('S')) return KeyInput::DOWN;
    if (M5Cardputer.Keyboard.isKeyPressed('a') || M5Cardputer.Keyboard.isKeyPressed('A')) return KeyInput::LEFT;
    if (M5Cardputer.Keyboard.isKeyPressed('d') || M5Cardputer.Keyboard.isKeyPressed('D')) return KeyInput::RIGHT;

    return KeyInput::NONE;
}

void setup() {
    Serial.begin(115200);
    auto cfg = M5.config();
    M5Cardputer.begin(cfg, true);
    
    initDisplay();
    initApp(g_uiContext);

    renderUI(g_uiContext);
}

void loop() {
    uint32_t currentMillis = millis();
    AppState oldState = g_uiContext.state;

    updateApp(g_uiContext, currentMillis);

    // 当状态在 updateApp 中自动流转发生变更时，自动触发界面绘制
    if (oldState != g_uiContext.state) {
        renderUI(g_uiContext);
    }

    // 时间戳非阻塞防抖 (避免硬 delay(150) 卡顿中断 60FPS 动画)
    static uint32_t lastKeyPressTime = 0;
    KeyInput key = readCardputerKeyboard();
    if (key != KeyInput::NONE) {
        if (currentMillis - lastKeyPressTime > 160) {
            lastKeyPressTime = currentMillis;
            handleInput(g_uiContext, key);
            renderUI(g_uiContext);
        }
    }

    // 1. 模拟动画进行中时，不断刷新 16 分支进度条
    if (g_uiContext.state == AppState::SIMULATING) {
        renderUI(g_uiContext);
        delay(15);
    }

    // 2. 当六维雷达图形变补间插值动画进行中时，保持 60FPS 零卡顿渲染全速驱动！
    if (g_uiContext.isRadarAnimActive) {
        renderUI(g_uiContext);
        delay(5);
    }

    // 3. 当处于开机 HOME 屏时，驱动 60FPS 正弦波呼吸脉冲雷达图律动
    if (g_uiContext.state == AppState::HOME) {
        renderUI(g_uiContext);
        delay(15);
    }

    delay(5);
}
#else
int main() {
    initApp(g_uiContext);
    printf("MBTI Parallel Cardputer Engine Initialized.\n");

    // 运行 100 次程序化场景生成基准测试
    printf("\n=== RUNNING 100 PROCEDURAL SCENARIO GENERATION BENCHMARK ===\n");
    ScenarioDNA recentHistory[10];
    int count = 0;
    for (int i = 0; i < 100; ++i) {
        GeneratedScenario gen = generateProceduralScenario(recentHistory, count);
        printf("[%03d] Quality: %5.1f | Title: %-20s | Desc: %s\n", i + 1, gen.qualityScore, gen.title, gen.description);
        
        for (int k = 9; k > 0; --k) recentHistory[k] = recentHistory[k - 1];
        recentHistory[0] = gen.dna;
        if (count < 10) count++;
    }
    printf("=== BENCHMARK COMPLETED SUCCESSFULLY (100 SCENARIOS GENERATED) ===\n");

    return 0;
}
#endif
