- UCS(又称Dijkstra's Algorithm) 很好，它 Optimal & Complete，但是它耗时，因为它没有利用目标的方位信息(而是地毯式搜索)，现在启发式Search(Informed Search) 聚焦于利用目标信息加速搜索

- Heuristics 启发式
	- 一个估计状态与目标接近程度的函数
	- 有很多种设计方法，比如采用 欧几里得距离、Manhattan距离(最短折线距离只能沿四个正方向行走)等

- ==Greedy Search 贪心搜索==
	- 通过 Heuristics 函数计算每个state到目标的距离
	- 策略：从Start State开始，优先选择距离最近的state，以此类推直到到Target
	- 实现方式：Fringe是一个优先队列(优先级：H)
	- 通常不会是 Optimal 或 Complete的


- ==A\* Search==
	- 结合 UCS + Greedy
	- 策略：UCS 通过 Cost 来选择下一个state、Greedy 通过 Heuristics 来选择下一个state，而 A\* Search 通过 $F=f(Cost,Heuristics)$ 来决定下一个state，一般来讲就是 $F=C+H$
	- 实现方式：Fringe是一个优先队列(优先级：F)
	- A\* Search 何时终止
		- 应当在 出队一个G时终止而非入队时
	- ==Complete & Optimal --- 当然这需要H合理，也就是 Admissible Heuristics==
		- ==A heuristic h is admissible if $0 \le h(n) \le h^*(n)$，where $h^*(n)$ is the true cost to the nearest goal== --- 这个理论的proof相当简单，note上的证明不错
			$$
				\begin{aligned}
	
				&我们假设有两条候选路径，路径一到G的祖先是A，路径二到G的祖先是B\\
				&这两条路径都会到目标G,假设路径一是最优路径 \\
				&  已知 f_1(G)=g_1+h_G=g_1 < f_2(G)=g_2+h_G=g_2, f_2(G) \ge f(B), f_1(G) \ge f(A)\\
				& 所以 f(A) < f_2(G),因此 A会比路径二的G先出队，这样路径一的G也会比路径二的G先出队，此时符合题设
				\end{aligned}
			$$
	- 比如 Manhattan 距离是 Admissible 的




	- ==Creating Admissible Heuristics --- 创建合理启发式函数很重要==
		- 一般来讲我们将难的问题进行relax得到简单易算的启发式函数，比如直接用欧几里得距离表示cost而不是实际公路距离

		- ==8 Puzzle 问题分析(华容道)==
			- 首先分析三要素
				- state space --- 各种state
				-  successor function(with actions，costs) + transition model
				- start state and a goal test(goal state)
			- 设置一个Heuristics
				- mismatch的方块数 --- 思考一下既然mismatch至少要移动一下吧，那么肯定小于真实cost
				- 到真实位置的manhattan距离之和 --- 思考一下至少需要manhattan距离吧
				- 想一想manhattan更具有指导意义吧 
		
		- ==马走棋 问题分析==
			- 马走棋会走日，因此马想到Goal位置时的Heuristic该如何分析呢
			- Heuristic 其实也是最先想到 Manhattan 和 欧几里得距离不过需要适配
				- Manhattan / 3
				- 欧几里得距离 / $\sqrt 5$
				- 水平或铅直距离 / 2
				- 以上三者取 max

		- Heuristics的说明
			- Dominance(优势)： $h_a \ge h_c$ 如果 $\forall n: h_a(n)\ge h_c(n)$
				- 大于号说明$h_a$优于$h_c$(想想8 puzzhle中manhattan更有优势)
			- Trivial heuristics
				- Start State的直接子节点 h 是精确的
				- Goal State的h 是 0
			- 其实大多数情况下，我们能找到几个 Heuristics，但是不一定有 Heuristic是绝对优势，因此我们可以采取 $h= max(h_1,h_2)$ 的方案来规约

- ==Tree Search and Graph Search== --- 概念性 policy
	- 在 Tree Search 中，我们会让同一个 state 出现两次，而Graph Search的核心就是 不会拓展已经拓展过的 state
	- 当引入 Graph Search的 理念时，我们就==可以处理循环问题，并且简化部分树==
	- 将已有的Tree Search 拓展为Graph Search，只需要  ==Tree Search + the set of expanded states(closed set)==，每当拓展state时就先看一眼closed set
	- 当引入 closed set 后，我们再反观 Heuristics，它需要比Admissible==更强的性质 Consistency==
		- Consistency --- 启发式“弧”成本 ≤ 每个弧的实际成本 即 $h(A) - h(C) ≤ cost(A \to C)$
		- 比如 A、C、G，A到C实际cost是1，C到G实际cost是3，A的h是4，C的h是3，虽然 Admissible，但是不 Consistency
		- 其实 Consistency使得我们path上的 F 值是非递减的
	- ==如果采用 Consistent Heuristic，那么 Graph Search 版本的 A\* 依然是 Optimal 的==
	- Tree Search
		- A\*在启发式函数 Admissble 时是最优的
		- UCS是特殊情况（h = 0）是最优的
	- Graph Search
		- A\*在启发式函数 Consistent 时是最优的
		- UCS是特殊情况（h = 0 is consistent）是最优的

- A\* 还有许多变体，主要是为了节省内存，比如类似前面提到的IDS，A\* 也有 IDA\* 算法但是使用 f 值作为限制，其他诸多的有 RBFS、SMA\* 就不赘述了


- 小结
	- 在这里我说明一下已经学习的Search方法 --- 同时分为 Tree Search 版 和 Graph Search 版 --- 在project 1 中我们实现了各种搜索算法的Graph Search版本，因此应该都是OK的，同时在Project1中我们实现了一版新的Graph的 A\* 算法能够确保在 heuristic 选择不当的时候仍然找到最优路径 [project 1](../Project/Proj1/Project1%20Search.md#2)
		- DFS
		- BFS --- Optimal(BFS是特殊情况的UCS)
		- IDS --- Great Optimal
		- UCS --- Great  Optimal(A\*的特殊情况)
		- Greedy
		- A\* ---- Great and Powerful  Optimal
	- 常用的Fringe
		- 栈 --- 每次入栈新探索的、每次出栈无路可走的
		- 队列 --- 每次入栈刚出栈的所有子state、每次出栈最优条件的state
		- 优先队列--- 每次入栈刚出栈的所有子state、每次出栈最优条件的state
		- closed set --- 使用 set 集合即可
	- 分析Search问题
		- 分析三要素 --- state space、successor function(actions、costs、transition model)、start state and goal test(goal state) --- ==我们要知道 就是找从 Start State 到 Goal State 的actions 序列==，明确State是如何的进而分析State Space等了解完善的初始条件
		- 💡==要有构建Search Tree的思维，每个节点都是State、弧是successor function(action+cost)== --- search tree 和 Tree Search 、Graph Search 无关，它是解决search问题的经典模型，相当于 mechanism 和 policy 的区别 
		- 采取合适的Search 方法解决 --- 一般都是选用 A\* 解决问题，因此这里需要想一个好的 Heuristic
	- 在以上学过的如此多种算法中，最通用且确保最优的Search 算法就是
		- Graph Search 版的 A\* 算法
