# Phase 3：16 Personality Branch Experience

## 1. 本阶段目标

基于已经完成的：

* Phase 1：MBTI Decision Engine
* Phase 2：Scenario Builder

实现 Cardputer 上的核心产品体验：

> **一个 Scenario → 16 种 MBTI 同时进行决策 → 显示分歧 → 用户探索不同人格的选择和理由。**

这是当前项目最重要的阶段。

不要增加新的业务功能。

重点是：

> **让这个过程“好玩”。**

---

# 2. 核心体验

完整流程：

```text
Scenario
   ↓
SIMULATING
   ↓
16 PERSONALITIES
   ↓
RESULT SUMMARY
   ↓
BIGGEST SPLIT
   ↓
EXPLORE
```

用户应该产生类似：

> “等等，为什么 ENTP 会选 YES，而 ISTJ 会选 NO？”

然后主动去查看不同人格。

---

# 3. Simulation Animation

用户确认 Scenario 后：

```text
SIMULATING...
```

不要立即显示结果。

创造一个短暂的“人格分支正在展开”的感觉。

动画建议：

```text
        ●

       ● ●

     ● ● ● ●

   ● ● ● ● ● ●

16 PERSONALITIES
```

或者：

```text
ISTJ   ...
ISFJ   ...
INFJ   ...
INTJ   ...
```

逐渐出现。

动画时间建议：

```text
500ms ~ 1500ms
```

不要超过 2 秒。

用户应该感觉：

> “机器正在计算。”

而不是：

> “程序故意让我等。”

---

# 4. Result Summary

模拟完成后，第一屏不要直接列出 16 个 MBTI。

首先显示总体分布：

```text
11 YES
4 NO
1 MAYBE
```

或者：

```text
YES   ███████████
NO    ████
MAYBE █
```

屏幕底部：

```text
BIGGEST SPLIT >
```

用户按 Enter：

进入最大分歧。

---

# 5. Biggest Split

这是 Phase 3 最重要的页面。

例如：

```text
BIGGEST SPLIT

ENTP
YES

      VS

ISTJ
NO
```

使用简单但有视觉冲击力的动画。

可以让：

```text
YES
```

和：

```text
NO
```

从屏幕两侧出现。

不要使用复杂 3D。

Cardputer 性能优先。

---

# 6. Explore

用户进入 Explore 后，可以在 MBTI 结果之间浏览。

推荐交互：

```text
← / →
```

切换人格。

例如：

```text
< ENTP >

YES

"Open to novelty
and accepts risk."

      1 / 16
```

右键：

```text
< ISTJ >

NO

"Prefers certainty
and practical value."

      2 / 16
```

但需要注意：

不要严格按照 MBTI 字母顺序强制浏览。

优先顺序应该考虑：

1. Biggest Split 的两个 MBTI
2. 与用户 MBTI 相关的类型（如果已有）
3. 其他人格

如果实现复杂，Phase 3 第一版可以暂时按照固定顺序。

---

# 7. MBTI Detail 页面

每个人格最多显示：

```text
ENTP

YES

Open to novelty
and accepts risk.

SCORE
█████████░ 88
```

不要显示：

* 长篇人格介绍
* MBTI 百科
* 心理学解释
* 复杂统计

本产品不是 MBTI Encyclopedia。

重点只有：

> **这个人格为什么在这个 Scenario 中做出了这个选择。**

---

# 8. Decision Reason

Phase 1 已经提供 reason。

Phase 3 直接使用。

不要重新实现解释系统。

例如：

```text
ENTP
YES

"Open to novelty
+ accepts risk"
```

如果字符串太长，自动换行。

必须确保：

* 不溢出 240×135
* 不截断重要文字
* 不出现乱码

---

# 9. 用户自己的选择

Explore 完成后，可以增加：

```text
WHAT WOULD
YOU DO?

YES
NO
MAYBE
```

用户可以选择自己的答案。

注意：

这个步骤应该很轻。

不要要求用户输入理由。

选择后显示：

```text
YOU

YES
```

然后：

```text
YOU MATCHED

ENTP
ENFP
ESTP
```

或者：

```text
YOU DIFFER FROM

ISTJ
ISFJ
```

这个结果只作为即时娱乐反馈。

本阶段暂时不做持久化。

---

# 10. “你和谁最像”

如果用户选择了自己的答案，可以计算：

```text
Your Decision
vs
16 MBTI
```

例如：

```text
YOU = YES

MATCH

ENTP
ENFP
ESTP
```

或者如果只有一个：

```text
MOST LIKE

ENTP
```

如果全部结果相同：

```text
EVERYONE AGREES.
```

如果没有任何人格和用户相同：

```text
YOU ARE
THE OUTLIER.
```

这个结果应该非常轻松、带一点游戏感。

---

# 11. 不要把结果做得太严肃

禁止出现：

```text
YOUR DECISION IS WRONG
```

禁止：

```text
You should...
```

禁止：

```text
Your personality indicates...
```

禁止：

```text
This decision may negatively affect...
```

应该使用：

```text
YOU MATCHED
ENTP
```

或者：

```text
YOU THINK
DIFFERENTLY.
```

甚至：

```text
INTERESTING.

NO ONE ELSE
CHOSE THIS.
```

保持玩具感。

---

# 12. UI 导航

推荐：

```text
Scenario Builder
       ↓
Simulation
       ↓
Summary
       ↓
Biggest Split
       ↓
Explore
       ↓
Your Choice
       ↓
Result
```

操作：

```text
UP / DOWN
    选择

LEFT / RIGHT
    浏览人格

ENTER
    确认 / 进入

ESC
    返回
```

不要设计复杂快捷键。

---

# 13. Cardputer 屏幕设计

目标：

> **信息密度低、视觉层次清楚、像一台小型专用仪器。**

建议：

```text
标题
━━━━━━━━━━━━━━

核心结果

辅助信息


操作提示
```

例如：

```text
┌──────────────────────┐
│ BIGGEST SPLIT        │
│                      │
│       ENTP           │
│        YES           │
│                      │
│         VS           │
│                      │
│       ISTJ           │
│        NO            │
│                      │
│ ENTER  EXPLORE       │
└──────────────────────┘
```

不要塞满文字。

---

# 14. 视觉风格

整体风格：

> **Pocket Terminal + Sci-Fi Instrument**

关键词：

* 黑色背景
* 高对比文字
* 简单线条
* 少量动态效果
* 等宽字体
* 不使用复杂图片
* 不使用 3D
* 不使用大量颜色

如果当前项目已有成熟 UI 风格，优先复用。

不要为了这个阶段重新引入 UI Framework。

---

# 15. 动画要求

只做 3 类动画：

### ① Simulation

人格逐渐出现。

### ② Branch

YES / NO 分裂。

### ③ Result

最终结果短暂闪烁/展开。

动画必须：

* 非阻塞
* 不影响输入
* 不造成明显卡顿
* 不持续太久

不要添加大量动画。

---

# 16. 性能要求

Cardputer 是 ESP32 设备。

必须避免：

* 大量 String 动态拼接
* 高频 malloc/free
* 大量 bitmap
* 大型字体
* 大型动画资源
* 不必要的 framebuffer
* 高频全屏刷新

优先：

```text
drawText
drawRect
drawLine
simple primitives
```

如果项目已有 M5Gfx / LovyanGFX，则使用现有图形系统。

---

# 17. Phase 3 不实现的内容

本阶段不要实现：

* LLM
* Wi-Fi
* API
* 语音
* 摄像头
* 多模态
* History
* SD 卡存储
* 用户账号
* 云端同步
* 复杂 MBTI 测试
* MBTI 百科
* 复杂统计
* StickS3 专用 UI

当前只做：

> **Scenario → 16 MBTI → Explore**

---

# 18. 测试 Scenario

使用 Phase 2 已经可以构造的 Scenario。

至少测试：

### Scenario A

高风险 + 高新奇：

预期产生明显分歧。

### Scenario B

低风险 + 高实用性：

预期多数人格倾向 YES。

### Scenario C

高社交 + 高情感价值：

观察 T/F 人格差异。

### Scenario D

高不确定性 + 高计划需求：

观察 J/P 人格差异。

---

# 19. Phase 3 最重要的产品测试

不要只测试：

> “代码有没有 Bug？”

需要进行以下体验测试。

### Test 1

第一次看到 Summary：

```text
11 YES
4 NO
1 MAYBE
```

是否能立即理解？

---

### Test 2

看到：

```text
ENTP YES
VS
ISTJ NO
```

是否自然地产生：

> “为什么？”

---

### Test 3

进入 Explore：

是否能够很自然地浏览不同人格？

---

### Test 4

看到 Reason：

是否能够在 2～3 秒内理解？

---

### Test 5

整个流程：

```text
构造 Scenario
→ Simulation
→ Biggest Split
→ Explore
→ Your Choice
```

是否能在：

> **30 秒～2 分钟**

完成？

---

# 20. 一个非常重要的设计要求

不要把 Phase 3 做成：

```text
RESULTS

ISTJ YES
ISFJ YES
INFJ NO
INTJ YES
...
```

这种普通列表。

**核心体验必须是“发现分歧”。**

用户真正感兴趣的不是：

> 16 个人格分别是什么答案。

而是：

> **“谁和谁意见完全不同？”**

因此：

```text
BIGGEST SPLIT
```

应该是整个 UI 的核心入口。

---

# 21. Phase 3 最终验收标准

完成后，我应该可以在真实 Cardputer 上完成：

```text
启动
 ↓
构造 Scenario
 ↓
ENTER
 ↓
SIMULATING...
 ↓
看到 YES / NO / MAYBE 分布
 ↓
看到 BIGGEST SPLIT
 ↓
进入 ENTP / ISTJ
 ↓
查看两者为什么不同
 ↓
选择自己的答案
 ↓
看到自己与哪些 MBTI 相同
```

整个体验：

> **不联网。**

> **不依赖 LLM。**

> **不需要输入长文本。**

> **不需要额外硬件。**

并且：

> **一次完整体验最好控制在 30 秒～2 分钟。**

---

# 22. 完成后的报告

不要只报告“Phase 3 完成”。

请提供：

1. 修改了哪些文件
2. 当前 UI 页面结构
3. 按键操作说明
4. Simulation 动画实现方式
5. Biggest Split 如何计算和显示
6. Explore 如何排序
7. Your Choice 如何计算匹配人格
8. 实际运行截图（如果开发环境支持）
9. Cardputer 实机测试结果
10. 发现的 UI / 性能问题
11. 你认为最需要下一阶段优化的 3 个问题

不要在本阶段主动增加 LLM、联网、语音或其他功能。
