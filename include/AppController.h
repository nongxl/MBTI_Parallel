#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include "UIState.h"

void initApp(UIContext& ctx);
void updateApp(UIContext& ctx, uint32_t currentMillis);
void handleInput(UIContext& ctx, KeyInput key);

#endif // APP_CONTROLLER_H
