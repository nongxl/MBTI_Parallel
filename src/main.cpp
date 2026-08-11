#include <cstdio>
#include "UIState.h"
#include "DisplayRenderer.h"
#include "AppController.h"

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

    KeyInput key = readCardputerKeyboard();
    if (key != KeyInput::NONE) {
        handleInput(g_uiContext, key);
        renderUI(g_uiContext);
        delay(150); // 防抖
    }

    // 模拟动画进行中时，不断更新进度条帧
    if (g_uiContext.state == AppState::SIMULATING) {
        renderUI(g_uiContext);
        delay(30);
    }

    delay(10);
}
#else
int main() {
    initApp(g_uiContext);
    printf("MBTI Parallel Cardputer Engine Initialized.\n");
    return 0;
}
#endif
