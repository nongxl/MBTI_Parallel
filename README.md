<div align="center">

# 🧠 MBTI Parallel (Pocket MBTI Decision Instrument)

<p align="center">
  <a href="#english">English</a> •
  <a href="#简体中文">简体中文</a>
</p>

![PlatformIO](https://img.shields.io/badge/PlatformIO-Build%20Passed-brightgreen)
![Platform](https://img.shields.io/badge/Platform-ESP32--S3-blue)
![Framework](https://img.shields.io/badge/Framework-Arduino-orange)
![License](https://img.shields.io/badge/License-MIT-green)
![Hardware](https://img.shields.io/badge/Hardware-M5Cardputer-red)

An offline, cyberpunk-styled pocket decision simulation instrument for **M5Stack Cardputer**. Predict how 16 MBTI personality types would act under procedural scenarios, generate your 6D decision profile, overlay dual radar polygon charts, and explore parallel decision branches in real-time.

[https://github.com/nongxl/MBTI_Parallel.git](https://github.com/nongxl/MBTI_Parallel.git)

---

</div>

<a name="english"></a>
## 🌐 English

### ✨ Key Features

- **⚡ 16 MBTI Parallel Simulation Engine**: Instantly simulates and computes decision outcomes (`YES` / `NO` / `MAYBE`) and cognitive rationale for all 16 MBTI personality profiles.
- **🧬 Dual Mode Scenario Engine**: Choose between **Procedural Random Generation** (pure C++ pipeline, zero LLM, zero network) or **Custom Scenario Creator** (4-step 2D grid: WHO → SITUATION → CONDITION → TENSION).
- **📊 Phase 6A Dual Radar Polygon Overlay**: Renders real-time dual polygons (`YOU` solid green vs `MBTI` solid purple) with 50% Smooth Alpha glass and deep royal indigo overlap blend.
- **🔍 WHY Match Breakdown Screen**: Computes and compares your top 3 aligned axes against the closest MBTI personality alongside dual radar visualization.
- **💾 ESP32 NVS Profile Board**: Persists decision history into non-volatile storage, accumulating long-term MBTI convergence and confidence metrics.
- **🔤 Boot Language Selector (i18n)**: Bipolar language switching between `[ENGLISH]` and `[中文]` on first boot with seamless menu configuration.
- **🔄 Unlimited Quick Play Loop**: Press `ENTER` to navigate from `YOUR_MATCH` → `WHY_MATCH` → `SUMMARY` → `EXPLORE` and start the next scenario without returning to the main menu.

---

### 🕹️ System Architecture

```mermaid
flowchart TD
    Start[Boot Language Select / i18n] --> HOME[HOME Screen: Dynamic Dual Radar Logo]
    
    HOME -->|Option 1: RANDOM| Gen[Procedural Scenario Generator]
    HOME -->|Option 2: CREATE| Build[Custom Scenario Creator: 4-Step 2D Grid]
    HOME -->|Option 3: PROFILE| Profile[Long-Term Profile Board: ESP32 NVS]
    
    Gen --> Preview[Scenario Preview: BUILDER_PREVIEW]
    Build --> Preview
    
    Preview -->|ENTER| Choice[User Decision: YOUR_CHOICE (YES / NO / MAYBE)]
    Choice -->|ENTER| Match[Dual Radar Overlay: YOUR_MATCH (Solid YOU vs Purple MBTI)]
    Match -->|ENTER| Why[WHY Match Breakdown: WHY_MATCH (Top 3 Aligned Axes + Dual Radar)]
    Why -->|ENTER| Summary[16 Branch Summary: SUMMARY (3-Color Bars & Dominant Faction)]
    Summary -->|ENTER| Explore[16 MBTI Explore: EXPLORE (Real-time Dual Radar Compare)]
    Explore -->|ENTER Quick Play| Preview
```

---

### 🎮 Controls

| Button | Action |
| :--- | :--- |
| **`LEFT` / `RIGHT`** (`,` / `/` or `A` / `D`) | Switch Boot Language / Navigate 2D Builder / Switch MBTI in Explore Mode |
| **`UP` / `DOWN`** (`;` / `.` or `W` / `S`) | Navigate 2D Grid Selection / Move Menu Focus / Access Clear History |
| **`ENTER`** | Confirm Selection / Progress Result Screens / Quick Play Next Scenario |
| **`ESC` / `BACKSPACE`** | Return to Previous Menu or Home Screen |

---

### 🛠️ Hardware Requirements & Memory Footprint

- **Device**: M5Stack Cardputer (ESP32-S3-DevKitC-1-N8)
- **Display**: 240 × 135 Color TFT (M5GFX Double Buffering Canvas)
- **RAM Usage**: `7.4%` (24,180 / 327,680 Bytes)
- **Flash Usage**: `22.4%` (747,141 / 3,342,336 Bytes)

---

### 🚀 Building and Flashing

#### Prerequisites
- [PlatformIO CLI](https://platformio.org/) or PlatformIO extension in VS Code.

#### Clone & Build
```bash
# Clone the repository
git clone https://github.com/nongxl/MBTI_Parallel.git
cd MBTI_Parallel

# Build the firmware
pio run
```

#### Upload to Cardputer
```bash
# Flash to connected M5Cardputer
pio run --target upload
```

---

<br/>

---

<a name="简体中文"></a>
## 🇨🇳 简体中文

### ✨ 核心特性

- **⚡ 16 人格平行决策推演引擎**：瞬间模拟推演 16 种 MBTI 性格模型在特定情境下的决策概率（`同意` / `拒绝` / `犹豫`）及 48 种认知功能（Cognitive Functions）专属决策依据。
- **🧬 双模式场景生成引擎**：支持 **程序化随机场景**（纯 C++ 算法，无需 LLM 和网络，数千离线情境）与 **自定义构造场景**（4 步 2D 网格：1.谁 → 2.情境 → 3.条件 → 4.纠结）。
- **📊 Phase 6A 双雷达多边形叠加**：绘制双雷达图（实线 YOU 绿 vs 实线 MBTI 紫），具备 50% 物理 Alpha 磨砂玻璃与皇家蓝紫 (Deep Royal Indigo) 像素级重叠加深混色引擎。
- **🔍 WHY 契合维度解析屏**：自动对比计算与你最相似的 3 个核心维度分值，搭配右侧双雷达图，直观回答“为什么我的这次决定最像这个人格”。
- **💾 ESP32 NVS 真实长效画板**：将每次决策实时持久化存储至非易失性闪存 (NVS)，收敛积累你的真实 MBTI 倾向与置信度。
- **🔤 开机双语一键切选 (i18n)**：首次开机与菜单支持自由切换 `[ENGLISH]` 与 `[中文]`，全系统双语深度渲染。
- **🔄 畅玩无限闭环 (Quick Play Loop)**：在结果页体验流畅链路 `YOUR_MATCH` → `WHY_MATCH` → `SUMMARY` → `EXPLORE`，按 `ENTER` 即可无缝生成下一个全新场景，连续畅玩。

---

### 🕹️ 系统架构图

```mermaid
flowchart TD
    Start[开机语言选择 / LANGUAGE_SELECT] --> HOME[HOME 主界面: 动态双雷达 Logo]
    
    HOME -->|模式1: 随机| Gen[离线程序化场景生成器]
    HOME -->|模式2: 自定义| Build[4 步 2D 场景构造器: WHO -> SITUATION -> CONDITION -> TENSION]
    HOME -->|模式3: 画像| Profile[真实长效 MBTI 画板: ESP32 NVS 持久化]
    
    Gen --> Preview[场景预览屏: BUILDER_PREVIEW]
    Build --> Preview
    
    Preview -->|ENTER| Choice[用户决策屏: YOUR_CHOICE (同意/拒绝/犹豫)]
    Choice -->|ENTER| Match[决策轮廓双雷达: YOUR_MATCH (实线 YOU vs 紫实线 最像 MBTI)]
    Match -->|ENTER| Why[维度契合解析: WHY_MATCH (最吻合 3 维度 + 右侧双雷达)]
    Why -->|ENTER| Summary[16分支汇总: SUMMARY (3 色柱状分布 + 优势阵营判定)]
    Summary -->|ENTER| Explore[16 人格 Compare 探索: EXPLORE (左右滑动实时双雷达对比)]
    Explore -->|ENTER 畅玩闭环| Preview
```

---

### 🎮 按键操作说明

| 按键 | 功能说明 |
| :--- | :--- |
| **`左` / `右`** (`,` / `/` 或 `A` / `D`) | 开机切换中英文 / 2D 网格左右移动 / Explore 模式切换 16 人格 |
| **`上` / `下`** (`;` / `.` 或 `W` / `S`) | 2D 网格上下移动 / 菜单焦点移动 / 进入历史清空确认 |
| **`ENTER`** | 确认选择 / 逐步推进结果屏 / 无缝进入下一个场景 |
| **`ESC` / `BACKSPACE`** | 返回上一级菜单 / 返回开机 HOME 界面 |

---

### 🛠️ 硬件与资源占用

- **硬件设备**：M5Stack Cardputer (ESP32-S3-DevKitC-1-N8)
- **显示屏**：240 × 135 彩色 TFT 屏 (M5GFX 离屏 Sprite 双缓冲区)
- **RAM 占用**：`7.4%` (24,180 / 327,680 字节)
- **Flash 占用**：`22.4%` (747,141 / 3,342,336 字节)

---

### 🚀 编译与烧录指南

#### 前置要求
- [PlatformIO CLI](https://platformio.org/) 或安装有 PlatformIO 插件的 VS Code。

#### 克隆与编译
```bash
# 克隆仓库代码
git clone https://github.com/nongxl/MBTI_Parallel.git
cd MBTI_Parallel

# 编译固件
pio run
```

#### 烧录至 Cardputer 实体设备
```bash
# 将固件烧录至已连接的 Cardputer
pio run --target upload
```

---

### 📄 开源协议

本项目采用 [MIT License](LICENSE) 开源协议。
