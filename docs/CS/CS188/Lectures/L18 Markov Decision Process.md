
- Markov Decision Process MDP
	- actions + search + probabilities + Time
	- 构成
		- 一系列 states $s \in S$
		- 一系列 actions $a \in A$
		- 转移函数 T(s, a, s')
			- P(s'|s,a)
		- 奖励函数 R(s,a,s') (and discount $\gamma$)
		- 开始状态
		- 结束状态(optional)
	- MDPs 是非确定性搜索问题，可以使用 expectimax search 解决，当然还有其它新工具
	- 形态
		- ![MDP.png](../assets/MDP.png)

- Markov 马尔可夫
	- 这是个人名，但是我们会用它表示 在给定当前状态的情况下，过去和未来条件独立
	- 在MDP中，这意味着 动作结果仅依赖于当前状态 i.e. $P(S_{t+1}|S_t, A_t)=P(S_{t+1}|S_t,A_t,S_{t-1},A_{t-1},...)$


- Policies
	- 对于MDPs，我们希望有一个最优策略 $\pi^*: S \rightarrow A$
		- 策略为每个state 决定 action
		- 最优策略就是最大化期望效用
	- Expectimax没有计算整个策略，它只计算了单个state的action
	- 名词解释
		- Policy = Choice of action for each state
		- Utility = sum of (discounted) rewards
		- $V^*(s)$ = 从状态s开始并采取最优行动的预期效用
		- $Q^*(s,a)$ = 从状态s采取行动a（之后）的最优行动期望效用
		- $\pi^*(s)$ = 从状态s的最优行动


- MDP Search Tree
	- 每个MDP状态对应一个类似expectimax的搜索树
	- (s, a) 是一个 q-state 
	- 形态
		- ![MDPSearchTree.png](../assets/MDPSearchTree.png)
	- 由上知，我们可以使用 折扣效用计算 Sequence 的效用，即$U([r_0,r_1,...])=r_0+\gamma r_1 +\gamma^2 r_2+....$  其中，折扣系数discount factor $\gamma \in (0,1]$ 
	- 无限效用问题
		- 如果我们一直运行得到sequence不断增长那么可能会得到无限效用
		- 解决方案
			- Finite horizon
				- 在T步后终止
				- 提供非平稳策略（策略依赖于剩余时间）
			- Discounting 折扣 使用 $0< \gamma <1$
				- $\sum_{t=0}^{\infty}\gamma^{t}r_{t} \le R_{max}/(1- \gamma)$
			- Absorbing state 吸收状态
				- 保证对于每条策略，最终都将达到一个终止状态



- Values of States
	- 基本操作：计算状态（expectimax）的值
		- 在最优行动下的期望效用 
		- 折扣奖励的平均总和
		- 这正是expectimax所计算的内容
	- Bellman等式 --- 相当于mini-expectimax搜索
		- $V^*(s)=max_a Q^*(s,a)$
		- $Q^*(s,a)=\sum_{s'} T(s,a,s')[R(s,a,s')+\gamma V^*(s')]$
		- $V^*(s)=max_a\sum_{s'} T(s,a,s')[R(s,a,s')+\gamma V^*(s')]$
	- Time limited Values
		- 大部分树都可以无限延伸，这样我们需要时间限制Values
		- 定义 $V_k(s)$ 即游戏在k个时间步结束时s的最优值
	- Value Iteration
		- Bellman等式的更新形式
			- $V_{k+1}(s)=max_a \sum_{s'} T(s,a,s')[R(s,a,s')+\gamma V_k(s')]$
		- 重复上述公式直到收敛
		- 收敛判断
			- 有最大深度则收敛
			- 折扣系数小于1 则收敛
		- 图示与示例
			- ![values.png](../assets/values.png)




