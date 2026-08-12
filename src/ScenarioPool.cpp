#include "ScenarioPool.h"
#include <cstdlib>
#include <cstring>

static const ConcreteScenario g_scenarioBank[SCENARIO_BANK_SIZE] = {
    // ---------------- [1. TRAVEL 分类: 旅行/出行现实选择] ----------------
    {
        "TRAVEL_001", ScenarioCategory::TRAVEL,
        "明天特价机票", "朋友突然告诉你抢到了两张明天去东京的特价机票，三天后回来，费用平摊。你明天没有紧急安排。",
        "FLIGHT TO TOKYO", "Your friend found cheap tickets to Tokyo for tomorrow, returning in 3 days. Split cost. No urgent tasks tomorrow.",
        "同意出发 (GO)", "婉言拒绝 (STAY)", "GO TO TOKYO", "DECLINE & STAY",
        {"FRIEND", "LAST_MINUTE", "EXPENSE", "NOVELTY"},
        {DecisionType::GO, 75.0f, 65.0f, 60.0f, 90.0f, 70.0f, 50.0f, 85.0f, 90.0f, 50.0f}
    },
    {
        "TRAVEL_002", ScenarioCategory::TRAVEL,
        "未知小镇无攻略", "到达旅游城市后，好友提议放弃所有热门景点攻略，随机坐上一辆公交车到终点站探险。",
        "UNPLANNED BUS", "Your friend suggests ditching all tourist guidebooks and taking a random bus to its terminal station.",
        "随性探险 (EXPLORE)", "按计划走 (STICK TO PLAN)", "RANDOM BUS EXPLORE", "STICK TO GUIDEBOOK",
        {"UNPLANNED", "FREEDOM", "RISK", "SPONTANEOUS"},
        {DecisionType::RISK, 65.0f, 20.0f, 40.0f, 95.0f, 50.0f, 30.0f, 80.0f, 85.0f, 30.0f}
    },
    {
        "TRAVEL_003", ScenarioCategory::TRAVEL,
        "帐篷露营过夜", "朋友邀请你周末去无人山顶露营看星空，没有洗手间和淋浴设施，需要自己背负重装备。",
        "WILD CAMPING", "Friends invite you to camp on a remote mountain peak. No restrooms or showers, heavy backpack required.",
        "背包出击 (CAMP)", "住舒适酒店 (HOTEL)", "GO WILD CAMPING", "STAY AT HOTEL",
        {"NATURE", "HARDSHIP", "STARS", "OUTDOORS"},
        {DecisionType::GO, 80.0f, 40.0f, 70.0f, 85.0f, 60.0f, 85.0f, 75.0f, 95.0f, 20.0f}
    },
    {
        "TRAVEL_004", ScenarioCategory::TRAVEL,
        "独行远途游学", "公司提供了一个为期两周的异国独立游学名额，全额报销但需要自己解决全程吃住与语言交流。",
        "SOLO STUDY TRIP", "Company offers a 2-week solo abroad study trip. Fully reimbursed, but you must handle all navigation yourself.",
        "独闯异国 (CHALLENGE)", "放弃留守 (DECLINE)", "ACCEPT SOLO TRIP", "STAY IN OFFICE",
        {"SOLO", "ABROAD", "GROWTH", "INDEPENDENCE"},
        {DecisionType::ACCEPT, 70.0f, 30.0f, 80.0f, 88.0f, 35.0f, 65.0f, 70.0f, 80.0f, 85.0f}
    },
    {
        "TRAVEL_005", ScenarioCategory::TRAVEL,
        "凌晨自驾看日出", "深夜 12 点，室友突然提议开车 3 小时去海边看清晨第一缕日出，明早需要补觉。",
        "MIDNIGHT DRIVE", "At midnight, roomie suggests driving 3 hours to the coast for sunrise. You'll need to sleep in tomorrow.",
        "连夜出发 (DRIVE)", "洗洗睡吧 (SLEEP)", "DRIVE TO COAST", "GO TO BED",
        {"MIDNIGHT", "SUNRISE", "IMPULSE", "YOUTH"},
        {DecisionType::GO, 60.0f, 30.0f, 50.0f, 85.0f, 65.0f, 60.0f, 65.0f, 90.0f, 20.0f}
    },

    // ---------------- [2. MONEY 分类: 金钱/理财现实选择] ----------------
    {
        "MONEY_001", ScenarioCategory::MONEY,
        "朋友创业借款", "多年好友筹备开咖啡店缺 2 万元周转，承诺一年后付高于银行的利息，但创业有失败风险。",
        "FRIEND STARTUP LOAN", "A close friend needs a $2,000 loan for a new cafe. Promises high interest in 1 year, but startups are risky.",
        "借款支持 (LEND)", "婉拒保本 (DECLINE)", "LEND THE MONEY", "KEEP SAVINGS SAFE",
        {"LOAN", "FRIEND", "RISK", "TRUST"},
        {DecisionType::KEEP, 85.0f, 80.0f, 20.0f, 40.0f, 50.0f, 10.0f, 80.0f, 60.0f, 40.0f}
    },
    {
        "MONEY_002", ScenarioCategory::MONEY,
        "新兴科技理财", "理财顾问向你推荐一个年化 15% 的前沿科技基金，但过去波动剧烈，可能损失 30% 本金。",
        "TECH FUND INVESTMENT", "Advisor suggests a 15% annual return tech fund, but volatility is high with up to 30% potential loss.",
        "小试牛刀 (INVEST)", "存定期保本 (DEPOSIT)", "INVEST IN FUND", "PUT IN FIXED DEPOSIT",
        {"HIGH_RETURN", "HIGH_RISK", "INVESTMENT", "FINANCE"},
        {DecisionType::RISK, 85.0f, 75.0f, 30.0f, 70.0f, 10.0f, 10.0f, 85.0f, 30.0f, 80.0f}
    },
    {
        "MONEY_003", ScenarioCategory::MONEY,
        "年终奖全额配置", "刚收到一笔意料之外的年终奖金，你可以选择全额存入储蓄账户，或奖励自己一场高级度假。",
        "BONUS ALLOCATION", "Unexpected year-end bonus received. You can save it all, or treat yourself to a luxury vacation.",
        "全额存入 (SAVE)", "犒劳自己 (VACATION)", "SAVE ENTIRE BONUS", "BOOK VACATION",
        {"BONUS", "SAVINGS", "REWARD", "TEMPTATION"},
        {DecisionType::KEEP, 20.0f, 60.0f, 10.0f, 60.0f, 20.0f, 10.0f, 20.0f, 70.0f, 90.0f}
    },
    {
        "MONEY_004", ScenarioCategory::MONEY,
        "二手交易刀价", "你在二手平台卖一件闲置吉他报价 1000 元，买家一口气砍到 700 元要求今天自提当面成交。",
        "USED GUITAR BARGAIN", "Selling a guitar for $100. Buyer offers $70 cash today for immediate pickup.",
        "爽快成交 (SELL)", "坚持底线 (WAIT)", "ACCEPT $70 CASH", "HOLD OUT FOR $100",
        {"BARGAIN", "BARTER", "CASH", "PATIENCE"},
        {DecisionType::ACCEPT, 30.0f, 30.0f, 10.0f, 40.0f, 30.0f, 10.0f, 40.0f, 40.0f, 80.0f}
    },
    {
        "MONEY_005", ScenarioCategory::MONEY,
        "合伙买彩票大奖", "同事组团合买头奖 5000 万的彩票，每人出 50 元。如果不参加，他们中了你将分不到一分钱。",
        "LOTTERY POOL", "Co-workers buying a $5M jackpot pool, $5 per person. If you opt out and they win, you get nothing.",
        "参与合买 (JOIN)", "不信这个 (PASS)", "JOIN THE POOL", "PASS ON LOTTERY",
        {"LOTTERY", "FOMO", "GAMBLE", "PEER"},
        {DecisionType::ACCEPT, 50.0f, 10.0f, 5.0f, 80.0f, 80.0f, 5.0f, 90.0f, 90.0f, 10.0f}
    },

    // ---------------- [3. PURCHASE 分类: 消费/购物现实选择] ----------------
    {
        "PURCHASE_001", ScenarioCategory::PURCHASE,
        "心仪物品限时打折", "你看中很久的机械键盘今天限时八折，但你手头的旧键盘还能正常工作。",
        "KEYBOARD DISCOUNT", "A mechanical keyboard you've wanted is 20% off today. Your old keyboard still works fine.",
        "果断下单 (BUY)", "继续用旧 (WAIT)", "BUY NOW WITH 20% OFF", "KEEP USING OLD ONE",
        {"DISCOUNT", "TECH", "TEMPTATION", "NEED_VS_WANT"},
        {DecisionType::GET, 30.0f, 50.0f, 10.0f, 65.0f, 20.0f, 10.0f, 30.0f, 80.0f, 60.0f}
    },
    {
        "PURCHASE_002", ScenarioCategory::PURCHASE,
        "旗舰手机换代", "最新旗舰手机发布，外观与性能有小幅提升，价格较高。你现在的手机才用了半年。",
        "FLAGSHIP PHONE UPGRADE", "New flagship phone released with minor upgrades. Expensive. Your current phone is 6 months old.",
        "尝鲜换新 (UPGRADE)", "理性克制 (STAY)", "UPGRADE NOW", "KEEP CURRENT PHONE",
        {"FLAGSHIP", "GADGET", "UPGRADE", "CONSUMERISM"},
        {DecisionType::GET, 40.0f, 80.0f, 10.0f, 85.0f, 45.0f, 10.0f, 40.0f, 75.0f, 40.0f}
    },
    {
        "PURCHASE_003", ScenarioCategory::PURCHASE,
        "网红餐厅排队 2 小时", "听说城里开了一家极具口碑的网红餐厅，现场前面排队需要等待 2 小时才能吃上。",
        "TRENDY RESTAURANT QUEUE", "A famous new restaurant has opened in town. The line is 2 hours long.",
        "耐心等待 (WAIT)", "换家打卡 (OTHER)", "WAIT IN LINE", "FIND ANOTHER PLACE",
        {"FOODIE", "QUEUE", "PATIENCE", "TREND"},
        {DecisionType::WAIT, 20.0f, 30.0f, 90.0f, 70.0f, 60.0f, 70.0f, 50.0f, 80.0f, 30.0f}
    },
    {
        "PURCHASE_004", ScenarioCategory::PURCHASE,
        "定制复古风衣", "你在独立裁缝铺看到一件绝版的复古定制风衣，剪裁极其完美但价格超出了本月预算。",
        "VINTAGE CUSTOM COAT", "A unique vintage custom coat fits you perfectly, but costs more than your monthly budget.",
        "咬牙拿下 (BUY)", "理性放弃 (PASS)", "PURCHASE COAT", "PASS DUE TO BUDGET",
        {"VINTAGE", "FASHION", "BUDGET", "AESTHETICS"},
        {DecisionType::GET, 50.0f, 85.0f, 10.0f, 80.0f, 30.0f, 10.0f, 50.0f, 95.0f, 30.0f}
    },
    {
        "PURCHASE_005", ScenarioCategory::PURCHASE,
        "订阅制软件续费", "你常用的一款效率软件从永久买断改为了按年订阅，每年收费 200 元才能保持高级功能。",
        "SOFTWARE SUBSCRIPTION", "Your productivity software switched to a $30/year subscription model.",
        "继续订阅 (SUBSCRIBE)", "寻找免费替代 (ALTERNATIVE)", "SUBSCRIBE ANNUALLY", "FIND FREE ALTERNATIVE",
        {"SUBSCRIPTION", "PRODUCTIVITY", "SAAS", "SOFTWARE"},
        {DecisionType::ACCEPT, 20.0f, 40.0f, 10.0f, 30.0f, 10.0f, 10.0f, 20.0f, 30.0f, 90.0f}
    },

    // ---------------- [4. SOCIAL 分类: 社交/派对现实选择] ----------------
    {
        "SOCIAL_001", ScenarioCategory::SOCIAL,
        "熟人久别聚会", "很久没见的中学同学突然发消息邀请你今晚参加聚会，有 10 多位熟人，但你明天早上需要早起。",
        "REUNION PARTY", "A middle school friend invites you to a reunion tonight with 10+ old friends. Early morning tomorrow.",
        "欣然前往 (JOIN)", "在家早睡 (REJECT)", "JOIN THE REUNION", "DECLINE & REST",
        {"REUNION", "OLD_FRIENDS", "LATE_NIGHT", "PARTY"},
        {DecisionType::GO, 40.0f, 20.0f, 60.0f, 50.0f, 90.0f, 60.0f, 40.0f, 80.0f, 40.0f}
    },
    {
        "SOCIAL_002", ScenarioCategory::SOCIAL,
        "行业交流沙龙上台", "参加大型行业交流沙龙时，主持人随机邀请现场观众上台分享 3 分钟个人项目经历。",
        "LIGHTNING TALK", "At a conference, the host asks for random volunteers to give a 3-minute lightning talk on stage.",
        "举手破局 (VOLUNTEER)", "安静坐着 (OBSERVE)", "VOLUNTEER ON STAGE", "STAY SEATED",
        {"STAGE", "PUBLIC_SPEAKING", "EXPOSURE", "SPOTLIGHT"},
        {DecisionType::GO, 75.0f, 10.0f, 10.0f, 85.0f, 95.0f, 50.0f, 75.0f, 85.0f, 70.0f}
    },
    {
        "SOCIAL_003", ScenarioCategory::SOCIAL,
        "陌生人桌游局", "朋友带你参加一个全是不认识新朋友的桌游局，大家都很热情但气氛有些吵闹。",
        "STRANGER BOARDGAMES", "Friend invites you to a board game night where you don't know anyone except the host.",
        "融入破冰 (SOCIALIZE)", "借故早退 (LEAVE EARLY)", "JOIN & SOCIALIZE", "LEAVE EARLY",
        {"STRANGERS", "ICEBREAKER", "BOARDGAMES", "INTROVERT_VS_EXTROVERT"},
        {DecisionType::GO, 60.0f, 10.0f, 50.0f, 80.0f, 85.0f, 50.0f, 70.0f, 75.0f, 50.0f}
    },
    {
        "SOCIAL_004", ScenarioCategory::SOCIAL,
        "破冰游戏表演", "团队建构活动中，队长指派你代表小组表演一个即兴节目以获取积分。",
        "TEAM BUILDING PERFORMANCE", "Team leader assigns you to perform an impromptu act to win points for the team.",
        "大方表演 (PERFORM)", "推辞换人 (PASS)", "PERFORM IMPROMPTU", "PASS TO TEAMMATE",
        {"IMPROMPTU", "TEAMBUILDING", "PERFORMANCE", "SHYNESS"},
        {DecisionType::DO, 70.0f, 10.0f, 20.0f, 80.0f, 90.0f, 60.0f, 70.0f, 80.0f, 50.0f}
    },
    {
        "SOCIAL_005", ScenarioCategory::SOCIAL,
        "邻居周末派对音量", "邻居周末举办派对音量较大，但邀请你一起加入他们享用免费啤酒与烧烤。",
        "NEIGHBOR BARBECUE", "Neighbors are throwing a noisy party, but invite you to join for free beer and BBQ.",
        "加入派对 (JOIN)", "戴耳机关窗 (IGNORE)", "JOIN NEIGHBOR PARTY", "PUT ON HEADPHONES",
        {"NEIGHBOR", "PARTY", "BEER", "FREE_FOOD"},
        {DecisionType::ACCEPT, 40.0f, 10.0f, 30.0f, 70.0f, 85.0f, 30.0f, 50.0f, 85.0f, 40.0f}
    },

    // ---------------- [5. FRIENDSHIP 分类: 人际/情感现实选择] ----------------
    {
        "FRIENDSHIP_001", ScenarioCategory::FRIENDSHIP,
        "朋友观点激烈分歧", "两位关系极好的朋友在群里因为社会热点话题吵得不可开交，气氛僵硬，向你寻求评判。",
        "FRIENDS ARGUMENT MEDIATION", "Two close friends are having a heated fight in a group chat over a controversial topic.",
        "出面调解 (MEDIATE)", "保持沉默 (NEUTRAL)", "MEDIATE & REASON", "STAY SILENT",
        {"FRIENDS", "CONFLICT", "MEDIATION", "HARMONY"},
        {DecisionType::SAY, 65.0f, 20.0f, 40.0f, 40.0f, 90.0f, 70.0f, 60.0f, 85.0f, 60.0f}
    },
    {
        "FRIENDSHIP_002", ScenarioCategory::FRIENDSHIP,
        "倾听深夜吐槽", "晚上 11 点，好朋友遇到伤心事打来电话倾诉哭泣，你需要听他讲话直到凌晨。",
        "LATE NIGHT VENTING", "At 11 PM, a friend calls crying over a breakup. Needs someone to listen until late night.",
        "耐心倾听陪伴 (LISTEN)", "安慰后早睡 (EXCUSE)", "LISTEN & COMFORT", "EXCUSE & SLEEP",
        {"VENTING", "EMPATHY", "LATE_NIGHT", "FRIENDSHIP"},
        {DecisionType::ACCEPT, 30.0f, 10.0f, 60.0f, 30.0f, 85.0f, 70.0f, 30.0f, 95.0f, 30.0f}
    },
    {
        "FRIENDSHIP_003", ScenarioCategory::FRIENDSHIP,
        "指出朋友刺耳真相", "朋友满怀信心展示他花了三个月完成的创业设计稿，你一眼看出存在致命的设计逻辑缺陷。",
        "HARSH TRUTH FEEDBACK", "Friend proudly shows you a project he worked 3 months on. You spot a fatal logic flaw immediately.",
        "直言不讳指出 (CRITIQUE)", "委婉称赞鼓励 (PRAISE)", "GIVE HARSH CRITIQUE", "PRAISE WARM-HEARTEDLY",
        {"HONESTY", "FEEDBACK", "LOGIC", "EMOTION"},
        {DecisionType::SAY, 50.0f, 10.0f, 20.0f, 30.0f, 70.0f, 40.0f, 40.0f, 40.0f, 90.0f}
    },
    {
        "FRIENDSHIP_004", ScenarioCategory::FRIENDSHIP,
        "搬家协助请求", "朋友周末搬家请求你帮他抬重型家具，需要花费你整整一个周六休息日。",
        "HELP FRIEND MOVE", "Friend asks for your help moving heavy furniture this Saturday, taking up your whole weekend.",
        "全力相助 (HELP)", "婉言拒绝 (DECLINE)", "HELP MOVING", "DECLINE WITH REASON",
        {"HELPING", "FAVOR", "WEEKEND", "LABOR"},
        {DecisionType::DO, 40.0f, 10.0f, 80.0f, 20.0f, 80.0f, 80.0f, 30.0f, 85.0f, 50.0f}
    },
    {
        "FRIENDSHIP_005", ScenarioCategory::FRIENDSHIP,
        "礼物惊喜准备", "好朋友即将过生日，你打算花一周时间亲自为他手工制作一件具有特别纪念意义的礼物。",
        "HANDMADE GIFT", "Friend's birthday coming up. You consider spending a week making a unique handmade gift.",
        "用心手作 (HANDMADE)", "买精美现成 (BUY READY-MADE)", "MAKE HANDMADE GIFT", "BUY READY-MADE GIFT",
        {"GIFT", "HANDMADE", "CREATIVITY", "CARE"},
        {DecisionType::DO, 20.0f, 30.0f, 70.0f, 60.0f, 60.0f, 60.0f, 30.0f, 95.0f, 40.0f}
    },

    // ---------------- [6. WORK 分类: 工作/职业现实选择] ----------------
    {
        "WORK_001", ScenarioCategory::WORK,
        "跳槽高薪创业公司", "一家初创公司高薪猎头邀请你加盟担任核心主管，薪资翻倍但工作强度极大且有随时倒闭风险。",
        "HIGH PAY STARTUP JOB", "A startup offers to double your salary as core lead, but intense hours and high risk of bankruptcy.",
        "勇敢跳槽 (JOIN STARTUP)", "稳定留任 (STAY)", "JOIN THE STARTUP", "STAY AT CURRENT JOB",
        {"CAREER", "SALARY", "RISK", "STARTUP"},
        {DecisionType::CHANGE, 85.0f, 70.0f, 90.0f, 85.0f, 40.0f, 90.0f, 80.0f, 50.0f, 80.0f}
    },
    {
        "WORK_002", ScenarioCategory::WORK,
        "替同事背锅纠错", "团队重大项目交接出现失误导致客户投诉，如果你主动出面承担责任可以保住项目，但会扣奖金。",
        "TAKE RESPONSIBILITY", "A mistake in project handoff led to client complaint. Stepping up saves the project but cuts bonus.",
        "主动担当 (TAKE CHARGE)", "按权责划分 (STICK TO RULES)", "TAKE RESPONSIBILITY", "FOLLOW PROCEDURE",
        {"RESPONSIBILITY", "WORKPLACE", "TEAM", "SACRIFICE"},
        {DecisionType::ACCEPT, 70.0f, 50.0f, 40.0f, 30.0f, 60.0f, 50.0f, 60.0f, 75.0f, 60.0f}
    },
    {
        "WORK_003", ScenarioCategory::WORK,
        "汇报方案颠覆创新", "明早向高层汇报方案，你可以选择沿用经过验证的保守模板，或展示一份颠覆性的全新创意方案。",
        "DISRUPTIVE PRESENTATION", "Presentation to executives tomorrow. Stick to proven safe template or pitch a disruptive wild idea?",
        "大胆创新 (PITCH NEW)", "稳妥汇报 (SAFE TEMPLATE)", "PITCH DISRUPTIVE IDEA", "USE SAFE TEMPLATE",
        {"PRESENTATION", "INNOVATION", "PITCH", "EXECUTIVE"},
        {DecisionType::DO, 75.0f, 40.0f, 50.0f, 90.0f, 50.0f, 50.0f, 75.0f, 70.0f, 75.0f}
    },
    {
        "WORK_004", ScenarioCategory::WORK,
        "拒绝无意义加班", "下班前 5 分钟领导布置了一项并不紧急的统计工作，要求大家今晚留下来加班完成。",
        "REJECT OVERTIME", "5 minutes before off-work, boss assigns non-urgent stats and asks everyone to stay late tonight.",
        "准时下班 (LEAVE)", "留下载加班 (OVERTIME)", "LEAVE ON TIME", "STAY FOR OVERTIME",
        {"OVERTIME", "WORK_LIFE_BALANCE", "BOUNDARY", "BOSS"},
        {DecisionType::REFUSE, 60.0f, 20.0f, 30.0f, 50.0f, 30.0f, 40.0f, 50.0f, 40.0f, 85.0f}
    },
    {
        "WORK_005", ScenarioCategory::WORK,
        "独立承接大项目", "部门空出一个重大跨国项目负责人位置，缺乏相关经验的你需要独立统筹整个团队。",
        "LEAD MAJOR PROJECT", "A major global project needs a lead. You lack direct experience, but taking it boosts your career.",
        "接下重任 (ACCEPT)", "推辞给资深同事 (PASS)", "ACCEPT THE LEAD", "PASS TO SENIOR",
        {"LEADERSHIP", "CHALLENGE", "CAREER", "PROMOTION"},
        {DecisionType::ACCEPT, 80.0f, 60.0f, 85.0f, 75.0f, 60.0f, 80.0f, 75.0f, 75.0f, 70.0f}
    },

    // ---------------- [7. PLANNING 分类: 计划/条理现实选择] ----------------
    {
        "PLANNING_001", ScenarioCategory::PLANNING,
        "制定年度详细计划", "新的一年开始，你计划花一整天时间将全年 12 个月的月度目标与每周 KPI 拆解到日历中。",
        "DETAILED YEARLY PLAN", "New year starts. You consider spending a full day breaking 12-month goals down into daily calendar tasks.",
        "精细拆解 (PLAN)", "随性推进 (GO WITH FLOW)", "CREATE DETAILED PLAN", "GO WITH THE FLOW",
        {"PLANNING", "NEW_YEAR", "CALENDAR", "ORGANIZATION"},
        {DecisionType::DO, 20.0f, 10.0f, 90.0f, 20.0f, 20.0f, 60.0f, 10.0f, 30.0f, 95.0f}
    },
    {
        "PLANNING_002", ScenarioCategory::PLANNING,
        "桌面与文档大扫除", "电脑工作桌面上堆满了 200 个未分类临时文件，你需要花费 3 小时建文件夹整理归档。",
        "DESKTOP CLEANUP", "Your computer desktop has 200 unorganized temp files. Clean up and organize them into folders?",
        "归档整理 (ORGANIZE)", "搜索即用 (SEARCH ONLY)", "ORGANIZE FILES", "JUST USE SEARCH",
        {"CLEANUP", "FILES", "ORDER", "SYSTEM"},
        {DecisionType::DO, 10.0f, 5.0f, 60.0f, 10.0f, 10.0f, 50.0f, 10.0f, 20.0f, 90.0f}
    },
    {
        "PLANNING_003", ScenarioCategory::PLANNING,
        "提前备用方案 B", "准备重要活动前，你决定额外准备两套应对突发恶劣天气与设备故障的备用方案 B。",
        "PREPARE PLAN B", "Before an important event, prepare two backup Plan B options for bad weather and equipment failure?",
        "备足方案 B (PREPARE)", "临场应变 (ADAPT)", "PREPARE BACKUP PLAN B", "ADAPT ON THE FLY",
        {"PLAN_B", "RISK_CONTROL", "PREPARATION", "SAFETY"},
        {DecisionType::DO, 30.0f, 10.0f, 85.0f, 20.0f, 30.0f, 50.0f, 15.0f, 30.0f, 90.0f}
    },
    {
        "PLANNING_004", ScenarioCategory::PLANNING,
        "每日列 Todo 清单", "每天早晨上班第一件事，先花 15 分钟列出今日 5 大 Priority 任务并打勾执行。",
        "DAILY TODO LIST", "Every morning, spend 15 minutes listing 5 priority tasks and checking them off throughout the day.",
        "严格清单 (TODO)", "随到随做 (FLEXIBLE)", "STICK TO TODO LIST", "WORK FLEXIBLY",
        {"TODO", "ROUTINE", "HABIT", "TIME_MANAGEMENT"},
        {DecisionType::DO, 10.0f, 5.0f, 80.0f, 10.0f, 20.0f, 30.0f, 10.0f, 30.0f, 85.0f}
    },
    {
        "PLANNING_005", ScenarioCategory::PLANNING,
        "旅行行程精确到小时", "制定出游计划时，将每天餐饮、景点门票、交通转乘时间精确规划到小时级别。",
        "HOURLY TRAVEL ITINERARY", "When planning a trip, schedule every meal, ticket, and transfer down to the exact hour.",
        "精细打卡 (SCHEDULE)", "大致走走 (ROUGH GUIDE)", "SCHEDULE HOURLY", "ROUGH GUIDE ONLY",
        {"ITINERARY", "TRAVEL_PLAN", "CONTROL", "PRECISION"},
        {DecisionType::DO, 20.0f, 10.0f, 95.0f, 30.0f, 30.0f, 70.0f, 10.0f, 30.0f, 80.0f}
    },

    // ---------------- [8. RISK 分类: 风险/博弈现实选择] ----------------
    {
        "RISK_001", ScenarioCategory::RISK,
        "极限滑翔伞体验", "旅游景区提供高空滑翔伞体验，有专业教练带领但需要从 800 米悬崖滑翔降落。",
        "PARAGLIDING EXPERIENCE", "A scenic spot offers tandem paragliding from an 800m cliff with a professional instructor.",
        "一飞冲天 (TRY)", "地面拍照 (WATCH)", "TRY PARAGLIDING", "STAY ON GROUND",
        {"EXTREME", "PARAGLIDING", "HEIGHTS", "ADRENALINE"},
        {DecisionType::RISK, 90.0f, 40.0f, 30.0f, 95.0f, 50.0f, 60.0f, 90.0f, 85.0f, 20.0f}
    },
    {
        "RISK_002", ScenarioCategory::RISK,
        "黑客松 24 小时破局", "参加 24 小时极限黑客松比赛，你可以选择安全保守的方案，或尝试未经验证的新技术路线。",
        "HACKATHON TECH STACK", "In a 24h hackathon, stick to a safe familiar tech stack or risk using a totally unproven new tech?",
        "险中求胜 (NEW TECH)", "稳拿奖项 (SAFE STACK)", "RISK NEW TECH STACK", "STICK TO SAFE STACK",
        {"HACKATHON", "NEW_TECH", "COMPETITION", "RISK_TAKING"},
        {DecisionType::RISK, 80.0f, 30.0f, 40.0f, 90.0f, 50.0f, 80.0f, 80.0f, 70.0f, 85.0f}
    },
    {
        "RISK_003", ScenarioCategory::RISK,
        "夜间徒步穿越野生林", "户外徒步下山时迷路，天色已黑，选择沿着未开发的陡峭小道下山还是原地等待救援。",
        "NIGHT MOUNTAIN DESCENT", "Lost while hiking at dusk. Descend down an unmarked steep shortcut or wait for night rescue?",
        "夜探下山 (DESCENT)", "原地固守 (WAIT)", "DESCEND UNMARKED PATH", "WAIT FOR RESCUE",
        {"SURVIVAL", "NIGHT", "MOUNTAIN", "DECISION"},
        {DecisionType::RISK, 95.0f, 10.0f, 70.0f, 70.0f, 30.0f, 90.0f, 95.0f, 30.0f, 70.0f}
    },
    {
        "RISK_004", ScenarioCategory::RISK,
        "公开演讲直接盲讲", "临时被叫去参加一场 500 人的行业论坛，讲稿尚未完善，选择带着草稿盲讲还是推辞。",
        "IMPROMPTU 500 AUDIENCE", "Called to speak at a 500-person forum with unfinished slides. Give impromptu speech or decline?",
        "硬核盲讲 (SPEAKE)", "推辞弃权 (DECLINE)", "GIVE IMPROMPTU SPEECH", "DECLINE THE STAGE",
        {"STAGE", "IMPROMPTU", "RISK", "CONFIDENCE"},
        {DecisionType::RISK, 85.0f, 10.0f, 20.0f, 85.0f, 80.0f, 70.0f, 85.0f, 75.0f, 60.0f}
    },
    {
        "RISK_005", ScenarioCategory::RISK,
        "试吃魔鬼辣挑战", "探店时餐厅举办魔鬼辣挑战赛，成功可享免费并上英雄榜，失败需当场喝冰牛奶降火。",
        "SPICY GHOST PEPPER", "Restaurant holds a ghost pepper challenge. Free meal & wall of fame if succeed, painful burn if fail.",
        "挑战极限 (CHALLENGE)", "安全观战 (PASS)", "TAKE SPICY CHALLENGE", "PASS & OBSERVE",
        {"SPICY", "CHALLENGE", "FUN", "PAIN"},
        {DecisionType::RISK, 70.0f, 10.0f, 20.0f, 90.0f, 70.0f, 40.0f, 70.0f, 90.0f, 10.0f}
    },

    // ---------------- [9. OPPORTUNITY 分类: 机遇/破局现实选择] ----------------
    {
        "OPPORTUNITY_001", ScenarioCategory::OPPORTUNITY,
        "行业大佬私人饭局", "偶然在活动中结识一位顶级行业前辈，他邀请你加入他的私人晚宴，但你需要取消今晚的原计划。",
        "VIP DINNER INVITATION", "Met a top industry leader who invites you to a private dinner tonight. Must cancel your plans.",
        "抓住机会 (ATTEND)", "守约原计划 (CANCEL)", "ATTEND VIP DINNER", "STICK TO ORIG PLAN",
        {"NETWORKING", "OPPORTUNITY", "VIP", "CAREER"},
        {DecisionType::GET, 60.0f, 40.0f, 50.0f, 85.0f, 85.0f, 40.0f, 60.0f, 85.0f, 80.0f}
    },
    {
        "OPPORTUNITY_002", ScenarioCategory::OPPORTUNITY,
        "海外驻点轮岗", "公司提供一个为期 1 年的海外分部负责人职位，能快速提升职业职级，但需远离亲友一年。",
        "OVERSEAS ROTATION", "A 1-year overseas station lead opening. Fast-tracks your career promotion, but far from family.",
        "抓住破局 (ACCEPT)", "陪伴留在本土 (DECLINE)", "ACCEPT OVERSEAS POST", "STAY WITH FAMILY",
        {"OVERSEAS", "PROMOTION", "CAREER", "DISTANCE"},
        {DecisionType::ACCEPT, 75.0f, 60.0f, 85.0f, 80.0f, 40.0f, 80.0f, 75.0f, 70.0f, 85.0f}
    },
    {
        "OPPORTUNITY_003", ScenarioCategory::OPPORTUNITY,
        "独家版权代理权", "你在国外发现一个尚未火爆但极具潜力的独立游戏，你有机会拿下国内独家代理权，需要投入自积蓄。",
        "INDIE GAME AGENCY", "Found an indie game abroad with great potential. You can buy exclusive agent rights using your savings.",
        "孤注一掷 (BUY AGENT)", "观望等待 (WAIT)", "PURCHASE AGENCY", "WAIT AND WATCH",
        {"AGENCY", "INDIE_GAME", "SAVINGS", "VENTURE"},
        {DecisionType::GET, 80.0f, 80.0f, 70.0f, 90.0f, 40.0f, 60.0f, 85.0f, 70.0f, 80.0f}
    },
    {
        "OPPORTUNITY_004", ScenarioCategory::OPPORTUNITY,
        "合伙人邀约", "前同事带着天使投资找你合伙创业，占据 30% 股份，要求你一个月内辞职加入团队。",
        "CO-FOUNDER OFFER", "Former colleague with angel funding offers 30% equity for you to quit and join as co-founder in 1 month.",
        "辞职创业 (JOIN)", "稳健留在原单位 (STAY)", "QUIT & JOIN CO-FOUNDER", "STAY AT CURRENT JOB",
        {"EQUITY", "COFOUNDER", "STARTUP", "RESIGNATION"},
        {DecisionType::CHANGE, 85.0f, 75.0f, 90.0f, 85.0f, 50.0f, 85.0f, 85.0f, 60.0f, 85.0f}
    },
    {
        "OPPORTUNITY_005", ScenarioCategory::OPPORTUNITY,
        "免费大师课全脱产", "你抽中了一项全球顶尖大师为期 1 个月的脱产封闭训练营名额，需请无薪假参与。",
        "MASTERCLASS BOOTCAMP", "Won a spot in an exclusive 1-month full-time masterclass bootcamp. Requires unpaid leave.",
        "请假报名 (ATTEND)", "放弃名额 (DECLINE)", "TAKE UNPAID LEAVE", "PASS OPPORTUNITY",
        {"BOOTCAMP", "MASTERCLASS", "UPGRADE", "SKILL"},
        {DecisionType::ACCEPT, 50.0f, 50.0f, 90.0f, 80.0f, 30.0f, 70.0f, 50.0f, 80.0f, 90.0f}
    },

    // ---------------- [10. TIME 分类: 时间/精力现实选择] ----------------
    {
        "TIME_001", ScenarioCategory::TIME,
        "周末完全断网沉浸", "本周末你打算进行 24 小时完全断网“数字排毒”，关掉手机与电脑，专注阅读与思考。",
        "DIGITAL DETOX WEEKEND", "Planning a 24-hour total digital detox this weekend. Turn off phones & laptops for deep reading.",
        "彻底断网 (DETOX)", "保持在线 (ONLINE)", "START 24H DETOX", "STAY ONLINE",
        {"DETOX", "OFFLINE", "READING", "CALM"},
        {DecisionType::DO, 30.0f, 10.0f, 80.0f, 40.0f, 10.0f, 30.0f, 20.0f, 90.0f, 60.0f}
    },
    {
        "TIME_002", ScenarioCategory::TIME,
        "早起 5 点晨跑计划", "为了改善作息，你决定连续一个月每天早晨 5:00 起来晨跑 5 公里，无论天气如何。",
        "5 AM MORNING RUN", "To fix sleep schedule, commit to waking up at 5:00 AM every morning for a 5km run for a month.",
        "打卡坚持 (RUN)", "顺其自然 (SLEEP IN)", "COMMIT TO 5 AM RUN", "SLEEP NORMALLY",
        {"MORNING_RUN", "HABIT", "DISCIPLINE", "HEALTH"},
        {DecisionType::DO, 20.0f, 10.0f, 85.0f, 20.0f, 20.0f, 85.0f, 10.0f, 50.0f, 80.0f}
    },
    {
        "TIME_003", ScenarioCategory::TIME,
        "通宵看剧爆肝", "你追的爆款剧集今晚迎来大结局全集上线，明早 9 点需要上班，你看完需要通宵。",
        "BINGE WATCH ALL NIGHT", "A hit series releases its full finale season tonight. You need to pull an all-nighter before 9 AM work.",
        "一口气刷完 (BINGE)", "留到周末看 (SAVE)", "BINGE WATCH ALL NIGHT", "SAVE FOR WEEKEND",
        {"BINGE", "SERIES", "ALL_NIGHTER", "TEMPTATION"},
        {DecisionType::DO, 40.0f, 20.0f, 40.0f, 70.0f, 30.0f, 70.0f, 40.0f, 90.0f, 10.0f}
    },
    {
        "TIME_004", ScenarioCategory::TIME,
        "多任务并发作业", "同时手头有 3 件事需要处理，你选择同时开启多任务并发交替推进，还是一次只做一件事。",
        "MULTITASKING VS SINGLE", "3 tasks on hand simultaneously. Multitask back and forth or focus 100% on one at a time?",
        "并发推进 (MULTITASK)", "专注单项 (SINGLE FOCUS)", "MULTITASK ALL THREE", "SINGLE TASK FOCUS",
        {"MULTITASK", "FOCUS", "EFFICIENCY", "WORKFLOW"},
        {DecisionType::DO, 40.0f, 20.0f, 50.0f, 60.0f, 40.0f, 70.0f, 50.0f, 50.0f, 80.0f}
    },
    {
        "TIME_005", ScenarioCategory::TIME,
        "碎片时间阅读学习", "每天通勤乘坐地铁的 40 分钟碎片时间，你习惯用来阅读专业书籍，还是刷短视频放松。",
        "COMMUTE TIME CHOICE", "40-min daily subway commute. Use it for reading professional books or scrolling short videos for relaxation?",
        "阅读充电 (READ)", "刷短视频放松 (RELAX)", "READ BOOKS", "SCROLL VIDEOS",
        {"COMMUTE", "READING", "SUBWAY", "HABIT"},
        {DecisionType::DO, 10.0f, 10.0f, 70.0f, 20.0f, 10.0f, 40.0f, 10.0f, 40.0f, 85.0f}
    },

    // ---------------- [11. COMFORT 分类: 舒适/生活质感现实选择] ----------------
    {
        "COMFORT_001", ScenarioCategory::COMFORT,
        "舒适宅家独处周末", "难得的周末假期，没有任何安排，你选择推掉所有邀约独自宅家看电影做饭，享受宁静。",
        "COZY HOME WEEKEND", "A rare free weekend with no plans. Decline all invites to stay home, cook, and watch movies in peace.",
        "享受宅家 (STAY HOME)", "出门约人 (GO OUT)", "STAY HOME IN PEACE", "GO OUT WITH FRIENDS",
        {"SOLITUDE", "COZY", "HOME", "RELAXATION"},
        {DecisionType::KEEP, 10.0f, 20.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 95.0f, 30.0f}
    },
    {
        "COMFORT_002", ScenarioCategory::COMFORT,
        "升舱头等舱体验", "长途飞行 10 小时，机场柜台提供优惠加价升舱至头等舱平躺卧铺名额，价格约为本月工资的 1/4。",
        "UPGRADE FIRST CLASS", "10-hour flight. Airport offers a discounted upgrade to lie-flat First Class for 1/4 of monthly pay.",
        "升舱享受 (UPGRADE)", "经济舱坚持 (ECONOMY)", "UPGRADE TO FIRST CLASS", "STAY IN ECONOMY",
        {"FIRST_CLASS", "COMFORT", "FLIGHT", "LUXURY"},
        {DecisionType::GET, 20.0f, 60.0f, 10.0f, 70.0f, 20.0f, 10.0f, 10.0f, 95.0f, 40.0f}
    },
    {
        "COMFORT_003", ScenarioCategory::COMFORT,
        "智能家居全套改造", "将家里所有灯具、窗帘、锁具改造为全套智能家居联动，需要花费两天休假进行安装调试。",
        "SMART HOME OVERHAUL", "Upgrade your home with full smart lighting, blinds, and locks. Requires 2 days of installation.",
        "全套改造 (UPGRADE)", "保持手动 (MANUAL)", "INSTALL SMART HOME", "KEEP MANUAL SWITCHES",
        {"SMART_HOME", "IOT", "CONVENIENCE", "TECH"},
        {DecisionType::DO, 30.0f, 50.0f, 40.0f, 80.0f, 30.0f, 50.0f, 20.0f, 90.0f, 75.0f}
    },
    {
        "COMFORT_004", ScenarioCategory::COMFORT,
        "高级水疗 SPA 体验", "连续加班一周后，你决定花费 600 元预订一次 2 小时的高级精油水疗 SPA 彻底放松。",
        "SPA RELAXATION", "After an intense work week, book a $100 2-hour premium oil SPA to fully unwind?",
        "预订放松 (BOOK SPA)", "省钱回家睡 (SLEEP)", "BOOK SPA TREATMENT", "GO HOME & SLEEP",
        {"SPA", "RELAX", "WELLNESS", "SELF_CARE"},
        {DecisionType::ACCEPT, 10.0f, 40.0f, 10.0f, 50.0f, 20.0f, 10.0f, 10.0f, 95.0f, 40.0f}
    },
    {
        "COMFORT_005", ScenarioCategory::COMFORT,
        "高品质人工打扫", "每周花费 150 元请专业保洁阿姨上门打扫卫生洗衣服，将自己从琐碎家务中彻底解脱出来。",
        "WEEKLY HOUSEKEEPING", "Pay $30/week for professional housekeeping to do all cleaning & laundry, freeing your weekend.",
        "聘请保洁 (HIRE)", "亲力亲为 (CLEAN MYSELF)", "HIRE HOUSEKEEPER", "CLEAN IT MYSELF",
        {"HOUSEKEEPING", "FREEDOM", "CLEANING", "CONVENIENCE"},
        {DecisionType::ACCEPT, 10.0f, 30.0f, 10.0f, 40.0f, 20.0f, 10.0f, 10.0f, 90.0f, 70.0f}
    },

    // ---------------- [12. ADVENTURE 分类: 探险/户外现实选择] ----------------
    {
        "ADVENTURE_001", ScenarioCategory::ADVENTURE,
        "无人小岛徒步", "参加一个为期三天的无人小岛生存探险营，手机无信号，需要自搭帐篷捕鱼过夜。",
        "DESERTED ISLAND ADVENTURE", "Join a 3-day uninhabited island survival trek. No cell reception, pitch tents & fish for food.",
        "报名探险 (JOIN)", "拒绝求稳 (STAY)", "JOIN ISLAND TREK", "STAY AT HOME",
        {"ISLAND", "SURVIVAL", "NO_SIGNAL", "ADVENTURE"},
        {DecisionType::RISK, 85.0f, 40.0f, 70.0f, 95.0f, 50.0f, 85.0f, 85.0f, 90.0f, 20.0f}
    },
    {
        "ADVENTURE_002", ScenarioCategory::ADVENTURE,
        "深海潜水考证", "在东南亚海岛休假，你打算花 4 天报名考取 PADI 开放水域潜水员证书，探索 18 米深海世界。",
        "PADI SCUBA DIVING", "On an island vacation, sign up for a 4-day PADI open water scuba diving certification down to 18m?",
        "考取潜水证 (DIVING)", "岸边戏水 (SHORE)", "GET SCUBA CERT", "STAY NEAR SHORE",
        {"DIVING", "OCEAN", "PADI", "EXPLORE"},
        {DecisionType::GO, 75.0f, 50.0f, 80.0f, 90.0f, 50.0f, 75.0f, 70.0f, 90.0f, 50.0f}
    },
    {
        "ADVENTURE_003", ScenarioCategory::ADVENTURE,
        "沙漠自驾穿行", "朋友约你驾车穿行塔克拉玛干沙漠公路，全程无加油站，需要车载额外油桶与救援绳索。",
        "DESERT CROSSING DRIVE", "Drive across a desert highway with friends. No gas stations, extra fuel cans & tow ropes needed.",
        "穿越沙漠 (DRIVE)", "放弃风险 (PASS)", "DRIVE CROSS DESERT", "PASS ON RISK",
        {"DESERT", "DRIVE", "CROSSING", "OFFROAD"},
        {DecisionType::GO, 85.0f, 60.0f, 80.0f, 90.0f, 60.0f, 80.0f, 80.0f, 85.0f, 40.0f}
    },
    {
        "ADVENTURE_004", ScenarioCategory::ADVENTURE,
        "冰川徒步攀登", "在冰岛旅行时，报名参加 6 小时冰川徒步，需要穿戴冰爪手持冰斧攀登开裂的蓝色冰川。",
        "GLACIER HIKE & CLIMB", "In Iceland, join a 6-hour glacier hike using crampons and ice axes over crevasses.",
        "攀登冰川 (CLIMB)", "远远观赏 (VIEW)", "CLIMB THE GLACIER", "VIEW FROM DISTANCE",
        {"GLACIER", "ICELAND", "ICE_CLIMB", "EXTREME"},
        {DecisionType::GO, 80.0f, 50.0f, 75.0f, 95.0f, 50.0f, 80.0f, 75.0f, 95.0f, 30.0f}
    },
    {
        "ADVENTURE_005", ScenarioCategory::ADVENTURE,
        "热气球自由飞行", "土耳其卡帕多奇亚晨曦热气球体验，升空 1000 米俯瞰火山地貌，随风飘移飞行。",
        "HOT AIR BALLOON", "Sunrise hot air balloon over Cappadocia, ascending 1,000 meters to drift with the wind.",
        "升空俯瞰 (BALLOON)", "地面拍摄 (GROUND)", "BOARD BALLOON", "STAY ON GROUND",
        {"BALLOON", "SUNRISE", "TURKEY", "HEIGHTS"},
        {DecisionType::GO, 65.0f, 60.0f, 30.0f, 90.0f, 60.0f, 30.0f, 60.0f, 95.0f, 40.0f}
    }
};

const char* getCategoryNameCN(ScenarioCategory cat) {
    switch (cat) {
        case ScenarioCategory::TRAVEL: return "✈️ 旅行出行";
        case ScenarioCategory::MONEY: return "💰 金钱理财";
        case ScenarioCategory::PURCHASE: return "🛒 消费购物";
        case ScenarioCategory::SOCIAL: return "🍻 社交派对";
        case ScenarioCategory::FRIENDSHIP: return "🤝 情感人际";
        case ScenarioCategory::WORK: return "💼 职业工作";
        case ScenarioCategory::PLANNING: return "📅 计划条理";
        case ScenarioCategory::RISK: return "⚡ 风险博弈";
        case ScenarioCategory::OPPORTUNITY: return "🚀 破局机遇";
        case ScenarioCategory::TIME: return "⏰ 时间精力";
        case ScenarioCategory::COMFORT: return "🛋️ 舒适生活";
        case ScenarioCategory::ADVENTURE: return "🌋 户外探险";
    }
    return "🌐 现实抉择";
}

const char* getCategoryNameEN(ScenarioCategory cat) {
    switch (cat) {
        case ScenarioCategory::TRAVEL: return "✈️ TRAVEL";
        case ScenarioCategory::MONEY: return "💰 MONEY";
        case ScenarioCategory::PURCHASE: return "🛒 PURCHASE";
        case ScenarioCategory::SOCIAL: return "🍻 SOCIAL";
        case ScenarioCategory::FRIENDSHIP: return "🤝 FRIENDSHIP";
        case ScenarioCategory::WORK: return "💼 WORK";
        case ScenarioCategory::PLANNING: return "📅 PLANNING";
        case ScenarioCategory::RISK: return "⚡ RISK";
        case ScenarioCategory::OPPORTUNITY: return "🚀 OPPORTUNITY";
        case ScenarioCategory::TIME: return "⏰ TIME";
        case ScenarioCategory::COMFORT: return "🛋️ COMFORT";
        case ScenarioCategory::ADVENTURE: return "🌋 ADVENTURE";
    }
    return "🌐 DECISION";
}

const ConcreteScenario& getNextBankScenario(ScenarioCategory recentCats[5], int recentCount) {
    int candidateIndices[SCENARIO_BANK_SIZE];
    int candidateCount = 0;

    // 智能筛选：优先避开最近 5 题出现过的 Category
    for (int i = 0; i < SCENARIO_BANK_SIZE; ++i) {
        bool isRecentCategory = false;
        for (int r = 0; r < recentCount; ++r) {
            if (g_scenarioBank[i].category == recentCats[r]) {
                isRecentCategory = true;
                break;
            }
        }
        if (!isRecentCategory) {
            candidateIndices[candidateCount++] = i;
        }
    }

    // 如果全被包含，备选池退回到全量 60 场景
    if (candidateCount == 0) {
        for (int i = 0; i < SCENARIO_BANK_SIZE; ++i) {
            candidateIndices[candidateCount++] = i;
        }
    }

    int selectedIdx = candidateIndices[rand() % candidateCount];
    return g_scenarioBank[selectedIdx];
}
