
- 在CS188中我们从 MDPs 的角度学习了 RL
	- 当时学习的RL从Model-based 和 no model-based 两个角度展开更贴近 MDPs
	- 但是不涉及 DRL，如何在 DNN 中使用RL是本节的重点

- RL的应用典例
	- [AlphaGo](../../Slides/Lecture12/drl_v5.pdf#page=9)

- 三步看RL
	- 第一步：[找函数](../../Slides/Lecture12/drl_v5.pdf#page=11)
		- 找函数即设计模型
		- Actor 策略模型
	- 第二步：[定义奖励](../../Slides/Lecture12/drl_v5.pdf#page=13)
		- 定义奖励其实也可以看成定义损失
		- episode --- 从开始到结束的所有交互的集合
	- 第三步：[优化](../../Slides/Lecture12/drl_v5.pdf#page=14)
		- trajectory 轨迹
		- 如何做优化其实是RL的最大挑战


- Policy Gradient 策略梯度
	- 可以控制 Actor 在各种状态下对于采取或不采取何种动作的倾向，比如利用交叉熵[做法如图](../../Slides/Lecture12/drl_v5.pdf#page=16)
	- [训练数据与目标如图](../../Slides/Lecture12/drl_v5.pdf#page=18-19)
		- 关键问题
			- A 怎么决定
			- 怎么更好采集数据
	- A的决定方式
		- [Version 0](../../Slides/Lecture12/drl_v5.pdf#page=20)
			- A =当前 r 
			- 显然不那么好，这是单纯的Greedy
		- [Version1](../../Slides/Lecture12/drl_v5.pdf#page=22)
			- A = 未来收益 $\sum r$
			- 这逐渐对了，考虑后续的收益，但是似乎越远的收益的影响应该越小
		- [Version2](../../Slides/Lecture12/drl_v5.pdf#page=23)
			- A = 折扣未来收益 $\sum \gamma^{n-t} r$
			- 奖励是相对的，如果几乎所有奖励都大于10，那么 10 其实就是坏情况，最好让奖励有正负
		- [Version3](../../Slides/Lecture12/drl_v5.pdf#page=24)
			- 设置 baseline b，所有的折扣未来收益都减去该 baseline，使得有正负

	- [训练的具体过程](../../Slides/Lecture12/drl_v5.pdf#page=25)
		- [On-policy & Off-policy](../../Slides/Lecture12/drl_v5.pdf#page=31)
			- On-policy 就是 当前策略生成的数据更新当前策略
			- Off-policy 就是 过去策略生成的数据更新当前策略
			- 往往Off-policy不如On-policy
	- 更好采集数据
		- Exploration 探索(CS188中的RLI和RLIII中有很多sample的方法)


- Actor-Critic
	- [Critic & Value function](../../Slides/Lecture12/drl_v5.pdf#page=37)
		- Critic 评估 Actor 的好坏
		- Value function 评估当前状态的value(折扣未来收益)
		- value function 是 Critic 的形式之一
	- 训练 Value function
		- [Monte-Carlo based](../../Slides/Lecture12/drl_v5.pdf#page=38)
			- 完整的 episode 更新
			- 就是Direct Evaluation(CS188RLII)
		- [TD](../../Slides/Lecture12/drl_v5.pdf#page=39)
			- 单 sample 更新
		- 这里是用DNN 做 Value function，因此用 loss 和 梯度的角度去做更新
		- 但是在 CS188 中是直接用 Value 表格作为 Value function，Value 表格中的值不断由对应公式更新
	- 利用 Value function --- A 的决定方式更近一步
		- [Version3.5](../../Slides/Lecture12/drl_v5.pdf#page=42)
			- 用 Value function 的值来替代 baseline b
		- [Version4](../../Slides/Lecture12/drl_v5.pdf#page=44) --- 最常用的
			- 用Advantage Actor-Critic
	- Tips
		- Actor 和 critic 共用部分参数
			- 共用部分参数相当于是传感器，让系统处理输入数据的部分，比如针对图像问题， Actor 和 Critic 都需要读取图像内容，[参数共享](../../Slides/Lecture12/drl_v5.pdf#page=45)


- Reward Shaping 奖励塑造
	- Sparse Reward [奖励稀疏](../../Slides/Lecture12/drl_v5.pdf#page=48)
		- 大多数的 reward = 0 或者说 我们不知道大多数情况究竟是好是坏
		- 例子
			- 机器人拧螺丝，在多数情况下它都不能拧好螺丝，因此奖励非常稀疏

	- Reward shaping：开发者定义额外奖励以引导代理
		- Reward Shaping 给更多情形定义奖励使得奖励信号变得稠密
			- 比如 [VizDoom](../../Slides/Lecture12/drl_v5.pdf#page=50) 本身只有射杀敌人或者自己死亡才会有奖励信号，但是这样的信号太稀疏，因此引入了更多情形的奖励信号，新奖励信号的定义需要 Domain 知识
		- Reward Shaping with curiosity
			- 当代理看到新（但有意义）的事物时获得额外奖励
			- [Curiosity](../../Slides/Lecture12/drl_v5.pdf#page=51)


- No Reward: Learning from Demonstration 从演示学习
	- 动机
		- 定义奖励在某些任务中也可能具有挑战性
		- 人类定义的奖励可能导致不可控的行为
	- Imitation Learning 模仿学习
		- [模仿学习](../../Slides/Lecture12/drl_v5.pdf#page=54)
			- 专家示范轨迹
			- 这样可以用 Supervised Learning 的方法去做，这被称为 Behavior Cloning
		- Inverse RL(IRL) 逆RL
			- [Inverse RL](../../Slides/Lecture12/drl_v5.pdf#page=58)
				- 假设：老师总是最好的
				- 由  专家示范轨迹 去学习出 Reward Function
			- [IRL的框架](../../Slides/Lecture12/drl_v5.pdf#page=59-60)
				- 可以看到 IRL 很像 GAN
		- [相关工作](../../Slides/Lecture12/drl_v5.pdf#page=62-63)
			- Guided Cost Learning
			- Skew-Fit
			- RIG