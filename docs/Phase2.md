# Phase 2：Scenario Builder — 离线决策情境构造器

## 1. 本阶段目标

在已经完成的 Phase 1「MBTI Decision Engine」基础上，实现一个完全离线的 **Scenario Builder**。

核心目标：

> 让用户不需要输入长文本，只通过 Cardputer 的方向键 / Enter 等基础按键，在很少的操作步骤内构造一个抽象的决策情境，然后将 Scenario 交给 Phase 1 的 DecisionEngine。

本阶段重点是：

**交互设计 + Scenario 构造。**

不要修改 Phase 1 的 MBTI 决策算法。

---

# 2. 产品定位

这个功能不是表单，也不是问卷。

用户应该感觉自己是在：

> **快速告诉 Cardputer：“我现在大概在纠结什么。”**

而不是填写一份详细的决策调查。

因此：

* 不允许用户输入长文本作为主要流程
* 不要求用户描述完整背景
* 不需要用户填写具体物品名称
* 不需要输入金额
* 不需要输入日期
* 不需要填写复杂参数
* 不需要联网
* 不需要 LLM

最终只需要得到一个抽象 Scenario。

例如用户通过几次选择构造：

```text
GET
WANT
HIGH COST
MEDIUM RISK
HIGH NOVELTY
```

然后生成：

```text
Scenario {
    type = GET,
    risk = 50,
    cost = 80,
    novelty = 80,
    ...
}
```

---

# 3. UI 原则

Cardputer 屏幕较小：

* 240 × 135
* 不要显示大段文字
* 一屏尽量只表达一个问题
* 选项数量控制在 4～6 个
* 使用方向键浏览
* Enter 确认
* ESC / Backspace 返回
* 不要依赖鼠标
* 不要要求长文本输入

交互必须适合未来移植到 StickS3。

因此不要设计依赖 QWERTY 键盘的复杂操作。

---

# 4. Scenario Builder 流程

建议第一版使用以下流程：

```text
START
 ↓
WHAT ARE YOU DECIDING?
 ↓
DECISION TYPE
 ↓
MOTIVATION
 ↓
MAIN CONCERN
 ↓
INTENSITY
 ↓
GENERATE SCENARIO
 ↓
SIMULATE
```

整个流程目标：

> **5～7 次选择完成。**

---

# 5. Step 1：Decision Type

显示：

```text
WHAT ARE YOU
DECIDING?

> GET
  GO
  DO
  SAY
  CHOOSE
  CHANGE
```

如果需要，可以增加：

```text
KEEP
QUIT
ACCEPT
REFUSE
WAIT
RISK
```

但注意屏幕一次最多显示 5～6 项。

因此应该实现滚动列表。

方向键：

```text
UP / DOWN
```

选择：

```text
ENTER
```

返回：

```text
ESC
```

---

# 6. Step 2：Motivation

根据 Decision Type 显示适当的动机。

不要为每一个 Decision Type 写完全不同的复杂流程。

尽量使用共享的 Motivation 集合：

```text
WANT
NEED
CURIOUS
OPPORTUNITY
FUN
PEOPLE
CHANGE
```

例如：

GET：

```text
WHY?

> WANT
  NEED
  CURIOUS
  FUN
```

GO：

```text
WHY GO?

> FUN
  PEOPLE
  CHANGE
  OPPORTUNITY
```

DO：

```text
WHY?

> NEED
  WANT
  CURIOUS
  OPPORTUNITY
```

如果某个选项对当前 Decision Type 不合理，可以隐藏。

---

# 7. Step 3：Main Concern

显示：

```text
WHAT'S
HOLDING YOU BACK?

> RISK
  COST
  TIME
  EFFORT
  PEOPLE
  UNKNOWN
```

允许选择：

```text
NONE
```

如果用户选择 NONE，可以适当减少后续步骤。

---

# 8. Step 4：Intensity

对于 Concern 询问程度：

```text
HOW MUCH?

> LOW
  MEDIUM
  HIGH
```

将选择转换为 Scenario 参数。

例如：

```text
LOW
MEDIUM
HIGH
```

对应：

```text
20
50
80
```

不要让用户看到内部数字。

---

# 9. Step 5：补充一个“情境属性”

为了让最终 Scenario 更有差异，可以再询问：

```text
WHAT MATTERS MORE?

> EXPERIENCE
  PRACTICAL
  PEOPLE
  SAFETY
```

或者根据前面选择动态调整。

最终形成：

```text
Decision Type
Motivation
Concern
Intensity
Priority
```

然后映射成 Phase 1 所需要的：

```text
risk
cost
time
novelty
social
effort
uncertainty
emotionalValue
practicalValue
```

---

# 10. 非常重要：不要暴露内部参数

用户不应该看到：

```text
Risk = 73
Novelty = 81
Practicality = 26
```

这些是内部模型。

用户只看到自然语言：

```text
GET
WANT
HIGH COST
NEW EXPERIENCE
```

然后系统内部转换成 Scenario。

---

# 11. Scenario Mapping

建立独立的映射层：

```text
ScenarioBuilder
       ↓
ScenarioMapper
       ↓
Phase 1 Scenario
```

例如：

```text
Decision Type = GET
Motivation = WANT
Concern = COST
Intensity = HIGH
Priority = EXPERIENCE
```

映射成：

```text
type = GET

risk = 45
cost = 80
time = 30
novelty = 65
social = 20
effort = 20
uncertainty = 30
emotionalValue = 75
practicalValue = 35
```

具体数值需要根据实际测试进行调整。

不要让 Mapping 代码散落在 UI 中。

---

# 12. Scenario Preview

生成 Scenario 后，在进入 Phase 3 之前，显示一个非常简短的总结：

例如：

```text
YOUR SCENARIO

GET
WANT
HIGH COST
NEW EXPERIENCE

READY?

[ENTER]
```

不要显示所有内部参数。

允许：

```text
ENTER = SIMULATE
ESC = BACK
```

---

# 13. RANDOM 功能

本阶段同时实现一个简单的 RANDOM Scenario。

首页增加：

```text
DECIDE
RANDOM
```

RANDOM 不需要用户选择。

从预定义的 Scenario 模板中随机生成一个。

例如：

```text
RANDOM SCENARIO

A FRIEND INVITES
YOU SOMEWHERE
TOMORROW.

GO?
```

但注意：

第一版不需要自然语言生成。

可以预置 10～20 个轻量 Scenario 模板。

例如：

```text
LAST-MINUTE TRIP
TRY SOMETHING NEW
BUY SOMETHING YOU WANT
JOIN A PARTY
CHANGE YOUR PLAN
TAKE A RISK
HELP SOMEONE
TRY A NEW HOBBY
```

这些模板最终仍然转换成 Phase 1 的 Scenario。

---

# 14. 不要做的事情

本阶段禁止：

* LLM
* Wi-Fi
* HTTP
* 语音
* 摄像头
* 长文本输入
* 键盘输入框
* 数据库
* 用户账号
* 云端同步
* 修改 Phase 1 DecisionEngine
* 复杂动画

---

# 15. Phase 2 验收标准

### 操作体验

从首页进入 Scenario Builder：

```text
START
 ↓
TYPE
 ↓
MOTIVATION
 ↓
CONCERN
 ↓
INTENSITY
 ↓
PRIORITY
 ↓
SIMULATE
```

理想情况下：

> 20～30 秒以内完成。

---

### UI

必须满足：

* 每屏内容简洁
* 选项清晰
* 当前选项有明显选中状态
* UP/DOWN 操作直观
* ENTER 确认
* ESC 返回
* 可以随时返回修改前一步
* 不出现文字溢出
* 不出现屏幕闪烁或严重刷新问题

---

### 架构

Scenario Builder 必须最终输出标准：

```cpp
Scenario
```

交给：

```cpp
DecisionEngine::simulateAll()
```

不能自己实现一套第二套人格算法。

---

# 16. 测试要求

至少测试以下场景：

### Test 1

```text
GET
WANT
COST
HIGH
EXPERIENCE
```

### Test 2

```text
GO
FUN
UNKNOWN
HIGH
EXPERIENCE
```

### Test 3

```text
DO
NEED
RISK
LOW
PRACTICAL
```

### Test 4

```text
CHANGE
OPPORTUNITY
TIME
HIGH
EXPERIENCE
```

确认不同组合确实能够产生不同的 Scenario，并能够正常传入 Phase 1。

---

# 17. 最终报告

完成后请报告：

1. 修改了哪些文件
2. Scenario Builder 的完整交互流程
3. 每个选项如何映射到 Scenario
4. Random Scenario 实现方式
5. 实际测试了哪些组合
6. 是否成功调用 Phase 1 DecisionEngine
7. 是否发现某些选择组合产生的结果过于单一
8. 哪些 UI 操作体验需要 Phase 3 调整

不要在本阶段继续扩展功能。
