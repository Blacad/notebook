
- Game
	- Game = task environment with > 1 agent
	- 其他信息
		- 确定性还是随机性？ --- 大富翁
		- 完美信息（完全可观察）？ --- 打牌
		- “两人、三人或更多人？ 
		- 团队还是个人？
		- 轮流还是同时？ 
		- 零和？ --- adversarial(对抗)
	- 我们想要 strategy(policy)，从每个可能状态推荐一个动作

- Deterministic Games --- 确定性游戏形式化
	- States --- S
	- Players --- P
	- Actions ---- A
	- Transition function(successor function) --- S x A -> S
	- Terminal test --- S -> {true, false}
	- ==Terminal utilities --- 终止时对赢得多好、输得多惨的评估 S x P -> R==
	- Solution for a player is a policy --- S -> A

- Zero-Sum Games
	- 零和游戏
		- 代理具有相反的效用
		- 纯粹竞争：
			- 一个最大化，另一个最小化
	- 总和游戏 General-Sum Games
		- 代理具有独立效用
		- 合作、冷漠、竞争、联盟转移等都是可能的


- ==Value of a state==
	- 从该状态所能到达的结果（utility）
	- 有时取最大，有时取最小

- ==Adversarial Game Tree --- 对抗游戏树==
	- Agents 轮流行动，不同层次的颜色(行动agent)相互交替
	- ==Minimax Values== --- 由于最终结果是依据你的偏好评估的utility，因此
		- 当处在对手的回合中，最小化 Value(对对手最好)
		- 当处在自己的回合中，最大化 Value(对自己最好)
	- Adversarial Game Tree 适用于 Deterministic + Zero-sum + Two Player games
		- 比如 井字棋、chess等
	- 实现方法 --- 互递归，本质上是DFS
	- Minimax 的特性就是假设 play 的双方都是 rational 的


- ==如果游戏不是Zero-sum且Player > 2 的方案== General Game
	- 我们可以在 Terminal State 评估对于每个 player 的 Terminal utility，这样在非Terminal State时的某个 Player Turn中只需 Max 自己的Terminal utility 就好了


- ==Game Tree Pruning==
	- Alpha-Beta Pruning
		- 方法 ：利用 Minimax 特性来剪枝部分State，在探知到某些 State后我们能知道上层State的取值范围，如果已经没有交集就可以剪枝掉(MiniMax本身是不带剪枝，它只会算所有的min-max)
		- [具体实现](../Notes/cs188-sp24-note05.pdf#page=6)
			- $\alpha$ 是 MAX 的最好值
			- $\beta$ 是 MIN的最好值
			- 剪枝逻辑
				- 从 min 到 max，是靠 $\alpha$ 剪枝 v，当 v 小于 $\alpha$ 时已经无意义了可剪枝
				- 从 max 到 min，是靠 $\beta$ 剪枝 v，当 v 大于 $\beta$ 时已经无意义了可剪枝
			- 考察时看父节点
		- 特点
			- 剪枝对根节点计算的 Value 没有影响，但是中间节点的 Value 可能是错的
			- 如果一个节点的 Value 确定则可以向上传导范围，如果只有范围则不行

- ==Evaluation Function==
	- 使用剪枝可能仍然资源受限以至于无法到 Terminal State
	- 于是可以使用 Evaluation Function
		- 不再Search到Terminal State，而==采用 Depth Limited==，并对 该限制下最深Depth的 non-terminal state采用Evaluation function 赋予它 Value，同时采用 $\alpha -\beta$ 剪枝优化
		- ==Evaluation Function ==
			- 在实践中一般是==特征加权的线性求和==
				- $Eval(s) = w_1f_1(s)+w_2f_2(s)+\dots +w_nf_1(n)$
				- e.g $f_1(s) =(白皇后数量 - 黑皇后数量),etc.$
			- 复杂的非线性函数(e.g. NN) trained by self-play RL