

-  我们为什么不知道一个行动的结果会是什么？
	- 明确的随机性：掷骰子
	- 不可预测的对手：幽灵随机响应
	- 行动可能失败：当移动机器人时，轮子可能会打滑

- ==Expectimax Search== --- 计算最优策略下的平均得分
	-  Values应反映平均情况（expectimax）的结果，而不是最坏情况（minimax）的结果
	- 关键要素
		- Max 节点与minimax搜索中的相同
		- 概率节点(Chance)，计算它们的期望效用即 取子节点的加权平均值（期望）
	- ==Max - Chance 的弧是你的Action、Chance - 下一个 的弧是概率选择==
	- 该 Search 不能像 Minimax 那样剪枝，但是可以引入类似的 Depth Limited + Evaluation Function 在资源受限时使用
	- 在expectimax搜索中，我们有一个关于对手（或环境）在任何状态下如何行为的概率模型，该模型可能是一个简单的均匀分布（掷骰子）或者可能很复杂，需要大量的计算


- ==Monte Carlo Tree Search==
	- 基于 $\alpha-\beta$ search 选定一个固定的 horizon
	- 两个重要思想 --- 结合[图](../Notes/cs188-sp24-note06.pdf#page=5)更好理解
		- 通过 rollout 评估 - 从状态 s 开始玩多局游戏直到结束（使用简单、快速的 rollout 策略），并计算胜利和失败次数
		- 选择性搜索Selective search - 探索有助于改善根节点决策的树的部分，而不考虑深度
	- Version 0 实现
		- 从根节点每个子节点进行N次Rollout，记录获胜比例
		- 根据这个指标选择最佳走法
	- Version 0.9 实现
		- 在更有希望的节点上运行更多次 Rollout
	- Version 1.0 实现
		- 在不确定的节点上运行更多次 Rollout
	- ==UCB heuristics==
		- $UCB(n)=U(n)/N(n)+C\times\sqrt{logN(PARENT(n))/N(n)}$
			- $N(n)=节点n的rollout总数$ 
			- $U(n)=节点n的rollouts总效用（例如，总获胜次数）$
			- 第一个term描述了节点有多有希望，而第二个term描述了我们对该节点效用的不确定性
	- ==Version 2.0 UCT 实现== --- [Slides](../Slides/cs188-sp24-lec06.pdf#page=58) 上的图不错
		- 重复直到时间耗尽
			- 在当前搜索树中，递归地应用UCB选择一条到达叶节点n（未完全展开）的路径
			- 向n添加一个新的子节点c并从c运行一次rollout
			- 从c更新胜率计数回根节点