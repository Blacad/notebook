- Local Search algorithm
	-  ==在许多优化问题中，路径无关；目标状态就是解决方案==。比如 填空解密问题，我们不关注你是怎么解的，只关注最终的答案 (goal state)
	- 状态空间 = “完整”配置的集合
		- 找到满足约束的配置，例如n皇后问题
		- 或者找到最优配置，例如旅行商问题
	- 在这种情况下，可以使用迭代改进算法：保持一个“当前”状态，尝试改进它
	- 常量空间，适用于在线和离线搜索
	- 几乎不可避免，如果“State”指的是你自己（即学习） 


- simple local search --- ==Hill-climbing algorithm==
	-  从任何地方开始
	- 重复：移动到最佳相邻状态
	- 如果没有比当前更好的邻居，则退出
	- 一般采用的 “好” 的评估是使用 Heuristic


- Heuristic for n-queens problem
	- 目标：棋盘上没有冲突的n个皇后，即没有皇后攻击另一个皇后
	- 状态：棋盘上有n个皇后（每列一个）
	- 行动：移动一个皇后（在其列内）
	- 启发式值函数：冲突数量

- ==Global and local maxima==
	- Local Search 天然容易遇到全局最大值和局部最大值问题，即它可能最终导向局部最大值
	- 一些解决方案
		- Random restarts -- 随机初始状态重启
		- Random sideways moves --- 随机侧向移动
			- ==Simulated annealing 模拟退火==
				- 依“温度”偶尔允许“坏”的走法
				- 高温度 => 允许更多坏走法，摇动系统跳出局部最小值
				- 按照某个计划逐渐降低温度
				- 理论保证
					- 稳态分布（玻尔兹曼）：$P(x)\alpha e^{E(x)/T}$
					- 如果T缓慢降低，将收敛到最优状态！


- ==Local beam search== 
	- K个Local Search算法副本，随机初始化
	- 对于每次迭代
		- 从K个当前状态生成所有后继状态
		- 从这些状态中选择最好的K个作为新的当前状态
	- 相比与单纯的并行K次随机初始化 hill-climbing algorithm 它有==不同初始状态延伸的通信==进而提高搜索效率

- ==Genetic algorithm== --- 使用自然选择隐喻 evolution
	- 执行步骤
		- fitness
		- selection
		- pairs
		- cross-over
		- mutation
	- 首先依据 fitness 函数计算个体的目标值，由目标值和selection 函数决定要选择的K个个体(可被复选)，进行 Pairs(配对)，在Pairs中进行Cross-Over，最后还会进行 Mutation 模拟突变
	- 可以运用至N皇后问题，具体见[Note](../Notes/cs188-sp24-note04.pdf#page=4)


- ==Local Search in continuous world==
	- 在现实世界中，问题一般是连续的(实数的)，比如在城市之间规划机场应该设置在哪里
	- 规划机场问题
		- 我们需要规划3个机场位置，使得各城市到最近机场的欧式距离之和最小
		- Airports set  --- $x=(x_1,y_1),(x_2,y_2),(x_3,y_3)$
		- Cities $(x_c,y_c)$ 用 $C_a$ 表示最接近 Airport a 的城市集合
		- Objective --- minimize $f(x) = \sum_a\sum_{c\in C_a}(x_a - x_c)^2+(y_a - y_c)^2$
	- ==处理continuous state/action space的方法==
		- 离散化 --- 定义一个增量为 $\sigma$ 的网格，使用任何离散算法 --- 常用
		-  对状态进行随机扰动 --- 不常用
			- 首选爬山法：不断尝试直到状态有所改善
			- 模拟退火
		-  计算 $f(x)$ 的梯度（解析法） --- 最常用
			- $\bigtriangledown f =(\partial f/\partial x_1,\partial f/\partial x_2,\dots)^T$
			- Gradient descent 梯度下降 --- $x = x-\alpha \bigtriangledown f(x)$