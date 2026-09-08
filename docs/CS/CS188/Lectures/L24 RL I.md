- 本节主要讲述 多臂老虎机以及推荐系统，重点讲述贪心算法(众多变体)、UCB等算法(基于蒙特卡洛方法)去解决这些问题，并且还引入乐观概念，可以说本节具体讲的是蒙特卡洛方法的各种算法(蒙特卡洛方法与RL关联度很高)，RL的基础介绍应该是RL-II开始讲[本节应属于拓展内容，不属于教程核心部分，可以作为额外知识了解]


- MDP、ML 和 RL 辨析
	- MDP --- 如何在你知道你的行为如何影响环境时进行规划？(用)
		- 你知道Transition model、Reward model、states、actions等进行规划
	- ML --- 如何从关于你的环境如何运作的数据中学习？（学）
		- 你知道一些 data point 以此训练模型完成其构造，比如训练Naive BNs 时就用data points习得 CPTs
	- RL --- 结合上述两者


- Multi-armed Bandit problem 多臂老虎机问题
	- 多臂老虎机是一个元组（A，R），其中A是已知的m个动作（或“臂”）集合，$R^a(r) = P[r|a]$是奖励的未知概率分布
	- 在每一步t，智能体选择一个动作 $a_t∈A$
	- 环境生成一个奖励 $r_t \sim R^{a_t}$
	- 目标是最大化累积奖励 $\sum_{\tau=1} ^t r_{\tau}$
	- 一些术语
		- 动作值action-value是动作a的平均奖励 $Q(a)=E(r|a)$
		- 最优值 $V^*$ 是 $V^*=Q(a^*)=max_{a \in A}Q(a)$
		- 遗憾 regret 是一步的机会损失 $l_t = E[V^* - Q(a_t)]$
		- 总遗憾 total reget 是总共的机会损失 $L_t=E[\sum_{\tau=1}^t V^*-Q(a_{\tau})]$
		- 最大化累积奖励 = 最小化总遗憾
		- $N_t(a)$ 是对行动 a 的预期选择次数
		-  gap $\Delta_a$ 是行动a与最优行动 $a*$ 之间的价值差异，即 $\Delta_a = V^*-Q(a)$
		- 因此 $L_t = \sum_{a \in A} E[N_t(a)]\Delta_a$


- 解决多臂老虎机的算法
	- Greedy Algorithm
		- ![RLGreedy.png](../assets/RLGreedy.png)
	- $\epsilon-$ Greedy Algorithm
		- ![RLeGreedy.png](../assets/RLeGreedy.png)
	- Optimistic Initialization
		- ![OptInit.png](../assets/OptInit.png)
	- Decaying $\epsilon_t-$ Greedy Algorithm
		- ![DRLeGreedy.png](../assets/DRLeGreedy.png)
		- 这里需要用到 gaps 的知识，但是我们在实践中是没有的，因此我们实际的更新策略会更简单 $\epsilon_t=min\{1, starting\_epsilon /(t+1)\}$
	- 算法伪代码
		- ![RLMB.png](../assets/RLMB.png)

	- Lower Bound
		- 任何算法的性能取决于最优臂与其他臂之间的相似性
		- 渐近总遗憾至少与步数成对数关系
			- $L_t \ge logt \sum_{a|\Delta_a >0}\frac{\Delta_a}{KL(R^a||R^{a^*})}$ 对于足够大的 t 

	- 利用 UCB (Upper Confidence Bound) 来选择动作 --- 能够量化探索和收益，避免 epsilon方法的单一
		- 我们在 Game 章节中学过 UCB 算法，如下：
			- $UCB1(n)=U(n)/N(n)+C\times\sqrt{logN(PARENT(n))/N(n)}$
				- $N(n)=节点n的rollout总数$ 
				- $U(n)=节点n的rollouts总效用（例如，总获胜次数）$
				- 第一个term描述了节点有多有希望，而第二个term描述了我们对该节点效用的不确定性
		- 引入 $\hat{U}(a)$ 估计每个动作的置信度，使得 $Q(a) \le \hat{Q}(a) + \hat{U}(a)$ 以高概率成立(高概率上界的目的就是防止“因为暂时估计偏低而错过真实最优动作”)，选择最大化 UCB 的动作即 
			- $a_t=argmax_{a\in A}\hat{Q_t}(a) + \hat{U_t}(a)$
		- 然后就是得到 UCB1 算法
			- 利用 Hoeffding‘s Inequality
				- ![Hoeffding.png](../assets/Hoeffding.png)
			- 得到UCB1
				- ![UCB1.png](../assets/UCB1.png)
		- 有上界
			- ![UCBLt.png](../assets/UCBLt.png)

		- 其伪代码
			- ![UCB1Code.png](../assets/UCB1Code.png)


- Optimism 乐观
	- 所谓乐观就是 when you don't know，you check it out and learn the truth，因此 所谓悲观就是 when you don't know，you never check it out and never learn the truth
	- 由此可见，乐观似乎是对的，但是 如果环境中充满严重的危险，你可能不会选择乐观而是悲观


- 这里还讲了Bayesian Bandits(一句带过这里也不细写可看ppt)
	- Bayesian UCB
	- Thompson sampling

- Contextual Bandits --- 可用作 推荐系统
	- 之前所讲到的多臂老虎机，只研究当前动作，但是在情景式场景下还需要研究状态
	- 定义
		- ![CB.png](../assets/CB.png)

	- 推荐系统例子 --- 电影推荐
		- 问题设置
			- [cs188-sp24-lec23.pdf](../Slides/cs188-sp24-lec23.pdf#page=35)
		- 问题定义与解决
			-  [cs188-sp24-lec23.pdf](../Slides/cs188-sp24-lec23.pdf#page=36-38)

- RL --- Unknown MDPs
	- 贪心解决 [cs188-sp24-lec23.pdf](../Slides/cs188-sp24-lec23.pdf#page=41)
	- 优化Rmax [cs188-sp24-lec23.pdf](../Slides/cs188-sp24-lec23.pdf#page=43)(乐观初始化，引入虚拟的天堂状态，算法会认为每个动作都是good的直到被现实打脸不断更新)