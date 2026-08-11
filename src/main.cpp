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
// 【核心重构】按键松开复位锁 (Key-Release Reset Lock)
// 彻底解决轻按被误判成长按/连发的问题：按一次键 100% 仅触发一次，必须完全松开手指后才允许下一次按压
static bool g_keyReleasedLock = false;

static KeyInput readCardputerKeyboard() {
    M5Cardputer.update();

    bool anyPressed = M5Cardputer.Keyboard.isPressed();

    // 1. 如果当前没有任何按键处于按下状态，说明手指已抬起松开，解锁！
    if (!anyPressed) {
        g_keyReleasedLock = false;
        return KeyInput::NONE;
    }

    // 2. 如果当前有按键按下，但处于锁定状态（说明是同一次按压的持续驻留），绝对不二次触发
    if (g_keyReleasedLock) {
        return KeyInput::NONE;
    }

    // 3. 首次按压瞬间 (Press Down Edge)：识别具体的按键
    KeyInput detected = KeyInput::NONE;

    if (M5Cardputer.Keyboard.isKeyPressed(';')) detected = KeyInput::UP;     // ; 键向上
    else if (M5Cardputer.Keyboard.isKeyPressed('.')) detected = KeyInput::DOWN;   // . 键向下
    else if (M5Cardputer.Keyboard.isKeyPressed(',')) detected = KeyInput::LEFT;   // , 键向左
    else if (M5Cardputer.Keyboard.isKeyPressed('/')) detected = KeyInput::RIGHT;  // / 键向右
    else if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) detected = KeyInput::ENTER;
    else if (M5Cardputer.Keyboard.isKeyPressed('`') || M5Cardputer.Keyboard.isKeyPressed(KEY_BACKSPACE)) detected = KeyInput::BACK;
    else if (M5Cardputer.Keyboard.isKeyPressed('w') || M5Cardputer.Keyboard.isKeyPressed('W')) detected = KeyInput::UP;
    else if (M5Cardputer.Keyboard.isKeyPressed('s') || M5Cardputer.Keyboard.isKeyPressed('S')) detected = KeyInput::DOWN;
    else if (M5Cardputer.Keyboard.isKeyPressed('a') || M5Cardputer.Keyboard.isKeyPressed('A')) detected = KeyInput::LEFT;
    else if (M5Cardputer.Keyboard.isKeyPressed('d') || M5Cardputer.Keyboard.isKeyPressed('D')) detected = KeyInput::RIGHT;

    // 4. 一旦识别到有效的按压，立即上锁，并返回按键指令！
    if (detected != KeyInput::NONE) {
        g_keyReleasedLock = true;
        return detected;
    }

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

    // 调用按键松开复位锁算法，极速零延迟响应，且 100% 防长按误触
    KeyInput key = readCardputerKeyboard();
    if (key != KeyInput::NONE) {
        handleInput(g_uiContext, key);
        renderUI(g_uiContext);
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
