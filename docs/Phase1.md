# Phase 1：MBTI Decision Engine 核心引擎

## 1. 项目目标

为 Cardputer 项目实现一个**完全离线、无网络、无 LLM 依赖**的 MBTI 决策模拟核心。

本阶段暂时不实现 UI、不实现用户输入、不实现持久化，也不接入任何外部 API。

核心目标：

> 给定一个抽象的决策情境，模拟 16 种 MBTI 人格分别会如何决策，并输出每个人格的决策结果、简短理由以及相关评分。

最终需要能够做到：

```text
Scenario
    ↓
Decision Factors
    ↓
MBTI Personality Model
    ↓
16 × Decision Simulation
    ↓
YES / NO / MAYBE
    ↓
Reason + Score
```

本阶段完成后，后续 Phase 2 可以直接调用这个引擎构造情境，Phase 3 可以直接将结果显示到 Cardputer UI。

---

# 2. 重要产品定位

这个系统不是专业心理学测试，也不是严肃的心理咨询或人生决策工具。

MBTI 在本项目中的定位是：

> **一种轻量、有趣的“人格视角模拟器”。**

因此：

* 不追求心理学上的严格准确性
* 不声称能够预测真实人格行为
* 不给用户提供“正确答案”
* 不判断用户的性格
* 不生成复杂的人格分析
* 不需要复杂机器学习模型

重点是：

> **让不同 MBTI 类型对同一个问题产生明显、有趣、可解释的差异。**

尤其要保证某些场景能够产生明显的：

```text
ENTP → YES

VS

ISTJ → NO
```

这种“人格分歧”。

---

# 3. 技术要求

根据当前项目实际技术栈实现。

如果项目当前使用 Arduino/C++ / M5Cardputer / ESP32，则使用现有工程架构，不要为了本阶段重新引入新的框架。

要求：

* 完全离线
* 不需要 Wi-Fi
* 不需要 HTTP
* 不需要 JSON 网络接口
* 不需要 LLM
* 不需要动态内存的大量分配
* 尽量使用固定大小的数据结构
* 适合 ESP32 / Cardputer 的资源限制
* 核心逻辑必须与 UI 解耦

不要把决策算法直接写进 UI 页面代码。

建议结构：

```text
MBTI Decision Engine
        │
        ├── Personality Model
        ├── Scenario Model
        ├── Decision Algorithm
        └── Result Model
```

未来 UI 只需要调用：

```cpp
DecisionResult simulate(
    const Scenario& scenario,
    MBTIType personality
);
```

或者等价接口。

---

# 4. 16 种 MBTI

必须完整支持：

```text
ISTJ
ISFJ
INFJ
INTJ

ISTP
ISFP
INFP
INTP

ESTP
ESFP
ENFP
ENTP

ESTJ
ESFJ
ENFJ
ENTJ
```

建议使用 enum：

```cpp
enum class MBTIType {
    ISTJ,
    ISFJ,
    INFJ,
    INTJ,
    ISTP,
    ISFP,
    INFP,
    INTP,
    ESTP,
    ESFP,
    ENFP,
    ENTP,
    ESTJ,
    ESFJ,
    ENFJ,
    ENTJ
};
```

同时提供：

```cpp
const char* getMBTIName(MBTIType type);
```

保证后续 UI 可以直接获取字符串。

---

# 5. 决策因素模型

不要直接给每种 MBTI 写：

```text
ISTJ = NO
ENTP = YES
```

必须建立一个**可计算的行为倾向模型**。

第一版使用以下 6 个核心人格维度：

### Risk

风险接受程度

```text
0 = 非常保守
100 = 非常冒险
```

### Novelty

对新奇、未知体验的偏好

```text
0 = 偏好熟悉
100 = 喜欢新鲜和未知
```

### Logic

理性/逻辑分析倾向

```text
0 = 更重视情感、人际因素
100 = 更重视逻辑、分析
```

### Social

社交因素的重要程度

```text
0 = 高度独立
100 = 高度考虑他人和社交关系
```

### Planning

计划和确定性的偏好

```text
0 = 随机、灵活、即兴
100 = 计划、结构、确定性
```

### Practicality

实际收益、效率和必要性的重视程度

```text
0 = 更重视体验、兴趣、可能性
100 = 更重视实用、效率、实际价值
```

定义：

```cpp
struct PersonalityProfile {
    MBTIType type;

    float risk;
    float novelty;
    float logic;
    float social;
    float planning;
    float practicality;
};
```

---

# 6. MBTI 人格参数

建立一套固定的、可解释的初始参数。

不要求心理学精确，但必须具有内部一致性。

例如可以从以下方向建立：

### S / N

S：

* Novelty 相对较低
* Practicality 相对较高

N：

* Novelty 相对较高
* 更容易接受可能性和未知

### T / F

T：

* Logic 较高
* Social 情感因素相对较低

F：

* Logic 相对较低
* Social 相对较高

### J / P

J：

* Planning 较高
* 对确定性更敏感

P：

* Planning 较低
* 对变化和即兴更适应

### E / I

E：

* Social 较高
* 对外部互动更敏感

I：

* Social 较低
* 更倾向独立判断

注意：

**不要简单地把 MBTI 四个字母直接映射成 YES/NO。**

这些参数最终必须参与综合计算。

---

# 7. Scenario 数据结构

建立抽象决策情境。

建议：

```cpp
enum class DecisionType {
    GET,
    GO,
    DO,
    SAY,
    CHOOSE,
    KEEP,
    QUIT,
    CHANGE,
    ACCEPT,
    REFUSE,
    WAIT,
    RISK
};
```

Scenario：

```cpp
struct Scenario {
    DecisionType type;

    float risk;
    float cost;
    float time;
    float novelty;
    float social;
    float effort;
    float uncertainty;
    float emotionalValue;
    float practicalValue;
};
```

所有数值统一：

```text
0 ~ 100
```

---

# 8. Decision Result

定义：

```cpp
enum class Decision {
    YES,
    NO,
    MAYBE
};
```

结果：

```cpp
struct DecisionResult {
    MBTIType personality;

    Decision decision;

    float score;

    float confidence;

    char reason[64];
};
```

其中：

### score

表示该人格对这个情境的总体倾向。

建议：

```text
0   = 强烈 NO
50  = 中立 / MAYBE
100 = 强烈 YES
```

### confidence

表示结果偏离中间值的程度。

例如：

```text
score = 95
confidence = 90

score = 52
confidence = 4
```

---

# 9. 决策算法

实现一个简单、可解释的加权模型。

不要使用随机数决定结果。

对于同一个：

```text
Scenario + MBTI
```

必须始终得到相同结果。

建议思路：

### Step 1

从 Scenario 获取：

```text
risk
novelty
social
uncertainty
cost
time
effort
emotionalValue
practicalValue
```

### Step 2

根据 MBTI PersonalityProfile 计算：

```text
risk compatibility
novelty compatibility
social compatibility
planning compatibility
logic/practicality compatibility
```

### Step 3

计算综合 score：

```text
score = 50 + weightedFactors
```

最终限制：

```text
0 ~ 100
```

### Step 4

根据 score 判断：

```text
score >= 65
    YES

score <= 35
    NO

otherwise
    MAYBE
```

阈值不要写死在多个地方，定义为常量：

```cpp
constexpr float YES_THRESHOLD = 65.0f;
constexpr float NO_THRESHOLD = 35.0f;
```

---

# 10. 必须产生“人格分歧”

这是 Phase 1 最重要的产品验收标准。

不能出现所有人格：

```text
YES
YES
YES
YES
...
```

也不能简单通过随机数制造差异。

应该通过 Scenario 与人格参数的自然组合产生差异。

至少设计以下测试情境：

---

## Test Scenario A：高风险、高新奇、低实用性

例如：

```text
type = RISK

risk = 85
cost = 70
time = 50
novelty = 90
social = 30
effort = 50
uncertainty = 85
emotionalValue = 70
practicalValue = 25
```

预期：

* 冒险/新奇导向人格更容易 YES
* 保守/计划导向人格更容易 NO
* 中间人格可能 MAYBE

---

## Test Scenario B：低风险、高实用性、高确定性

```text
risk = 10
cost = 30
time = 30
novelty = 20
social = 20
effort = 30
uncertainty = 10
emotionalValue = 30
practicalValue = 90
```

预期：

多数人格倾向 YES，但仍然允许出现少量差异。

---

## Test Scenario C：高社交因素、低实际收益

```text
risk = 30
cost = 20
time = 60
novelty = 50
social = 90
effort = 50
uncertainty = 30
emotionalValue = 85
practicalValue = 20
```

预期：

不同人格在 Social / Logic / Practicality 上产生明显差异。

---

# 11. “理由”不要使用 LLM

Phase 1 完全离线。

因此 reason 不需要生成自然语言长句。

可以使用**模板化理由**。

例如：

```text
"High risk tolerance"
"Values practical benefit"
"Prefers certainty"
"Open to new experiences"
"Considers social impact"
"Prefers careful planning"
```

可以根据影响最大的 1～2 个因素组合。

例如：

```text
ENTP
YES
"Open to novelty + accepts risk"

ISTJ
NO
"High uncertainty + low practicality"
```

理由长度控制在：

```text
<= 50~60 characters
```

为后续 Cardputer 屏幕显示考虑。

---

# 12. 提供批量模拟接口

后续 Phase 3 会需要一次模拟全部 16 种人格。

因此提供：

```cpp
constexpr int MBTI_COUNT = 16;

void simulateAll(
    const Scenario& scenario,
    DecisionResult results[MBTI_COUNT]
);
```

结果顺序固定。

例如：

```text
ISTJ
ISFJ
INFJ
INTJ
ISTP
ISFP
INFP
INTP
ESTP
ESFP
ENFP
ENTP
ESTJ
ESFJ
ENFJ
ENTJ
```

---

# 13. 提供“统计分歧”的接口

后续 Phase 3 需要显示：

```text
11 YES
4 NO
1 MAYBE
```

因此提供：

```cpp
struct DecisionSummary {
    int yesCount;
    int noCount;
    int maybeCount;

    MBTIType strongestYes;
    MBTIType strongestNo;
};
```

以及：

```cpp
DecisionSummary summarizeResults(
    const DecisionResult results[MBTI_COUNT]
);
```

---

# 14. 找出“最大分歧”

这是未来 UI 的核心。

需要提供一个简单方法：

```cpp
void findBiggestSplit(
    const DecisionResult results[MBTI_COUNT],
    MBTIType& yesType,
    MBTIType& noType
);
```

逻辑：

* 找 score 最高的 MBTI
* 找 score 最低的 MBTI
* 如果两者差距足够大，则认为存在明显分歧

例如：

```text
ENTP  88
...
ISTJ  27
```

则：

```text
BIGGEST SPLIT

ENTP  YES
ISTJ  NO
```

如果所有人格结果非常接近，则允许：

```text
MOSTLY AGREE
```

---

# 15. 测试程序

Phase 1 必须提供一个简单的测试入口。

如果当前工程允许，可以先通过 Serial 输出。

例如：

```text
================================
MBTI DECISION ENGINE TEST
================================

Scenario:
HIGH RISK / HIGH NOVELTY

ISTJ   NO       28
ISFJ   NO       34
INFJ   MAYBE    48
INTJ   MAYBE    56
ISTP   YES      71
...
ENTP   YES      91
...

YES: 8
NO: 5
MAYBE: 3

BIGGEST SPLIT:
ENTP YES
ISTJ NO
```

这样不需要 UI 就能验证算法。

---

# 16. 代码架构要求

尽量将代码拆分为：

```text
MBTI/
    MBTIProfile.h
    MBTIProfile.cpp

Decision/
    Scenario.h
    DecisionEngine.h
    DecisionEngine.cpp

Tests/
    DecisionEngineTest.cpp
```

如果当前工程结构不适合，可以采用项目现有结构，但必须保持：

> **人格数据、Scenario、算法、测试与 UI 解耦。**

未来 UI 不应该知道具体算法。

UI 只调用：

```cpp
simulate(...)
simulateAll(...)
summarizeResults(...)
findBiggestSplit(...)
```

---

# 17. 本阶段明确禁止实现的内容

不要提前实现：

* Wi-Fi
* HTTP
* REST API
* LLM
* OpenAI API
* 语音输入
* 摄像头
* 多模态
* 用户账号
* 云端同步
* 数据库
* SD 卡
* History UI
* MBTI 测试问卷
* 复杂动画
* Cardputer 完整 UI
* 额外硬件

也不要为了“以后可能需要”加入复杂依赖。

---

# 18. Phase 1 最终验收标准

完成后，我应该能够在 Cardputer 上运行测试程序，并验证：

### 基础

* [ ] 16 种 MBTI 全部存在
* [ ] 每种 MBTI 有固定 PersonalityProfile
* [ ] Scenario 可以使用统一数据结构描述
* [ ] 同一个 Scenario 每次运行得到相同结果
* [ ] 完全离线运行

### 算法

* [ ] 不使用随机数制造人格差异
* [ ] 不使用 LLM
* [ ] YES / NO / MAYBE 三种结果正常工作
* [ ] score 为 0～100
* [ ] confidence 可以反映结果强弱
* [ ] 每个结果有简短、可解释的 reason

### 产品体验基础

至少有几个 Scenario 能产生明显的人格分歧，例如：

```text
ENTP    YES
ESTP    YES
ENFP    YES

VS

ISTJ    NO
ISFJ    NO
ESTJ    NO
```

而不是所有 MBTI 得到相同答案。

### 架构

后续能够直接实现：

```text
Phase 2:
Scenario Builder
       ↓
Scenario
       ↓
DecisionEngine
```

以及：

```text
Phase 3:
DecisionEngine
       ↓
16 Results
       ↓
Cardputer UI
```

---

# 19. 最终输出

完成后请不要只告诉我“代码完成”。

请提供一份简短的 Phase 1 实现报告，包括：

1. 实际创建/修改了哪些文件
2. MBTI PersonalityProfile 的最终参数
3. Scenario 数据结构
4. 最终决策算法
5. YES / NO / MAYBE 阈值
6. 测试了哪些 Scenario
7. 每个测试 Scenario 的 16 型结果
8. 是否成功产生明显的人格分歧
9. 当前已知问题
10. 对 Phase 2 Scenario Builder 的接口建议

**注意：如果在实现过程中发现当前项目已有代码或架构与上述方案冲突，应优先保持现有项目稳定，不要大规模重构。先向我说明冲突点，再选择最小修改方案。**
