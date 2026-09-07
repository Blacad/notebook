
- ==Forward Chaining Algorithm== --- 根据已知事实KB验证字面量是否能被蕴含 --- 属于 theorem proving 的 proof 方法
	- KB相当于该逻辑系统的前提/条件(已知事实)
	- Forward Chaining Algorithm 就是一种**基于规则的自动推理方法**，主要用于**从已知事实出发，不断应用规则，推导出新的事实，直到得到目标结论或无法再推导为止**
	- 不多说，贴个图![](assets/forward-chaining.png)
		- count 记录 KB中子句的前提个数
		- inferred 记录已经推出的symbols(开始全为false)
		- agenda 记录KB数据库
		- 我们可以看到 agenda是不断更新的，inferred 存储最终的完整结果
	- 课上还提到 Backward Chaining 算法，本质上思路有些像，只不过是从 q 往前找

- Satisfiability
	- Satisfiability --- 句子在至少一个可能世界中为真时，则称具备 可满足性
	- a |= b 
		- 等价于 在已知事实的所有可能世界中 $a \implies b$ 为真
		- ==等价于 在已知事实的所有可能世界中 $a \wedge \neg b$ 为假 也就是说 $a \wedge \neg b$ 不具备可满足性==
	- 所以，将否定结论添加到你所知的内容中，检验其（不）可满足性，也称为归谬法，==同样可以用来看字面量是否能被蕴含==
	- 因此两个问题规约了 ==验证蕴含能被转化为验证CNF可满足性==

- ==Conjunctive normal form 合取范式==
	- 特点
		- 每个句子都可以表达为子句的结合  a conjunction of clauses --- $\wedge$
		- 每个子句都是字面量的析取 disjunction of literals
		- 每个字面量都是一个符号或一个否定符号 a literals is symbol or negated symbol
	- ==任何命题逻辑都可以转换为CNF！==
		- $a \implies b$  可以被替换为 $\neg a \vee b$ 
	- distributivity 分配律
		- $(a\wedge b)\vee c = (a \vee c)\wedge (b \vee c)$ ---> $(a1 \wedge a2 \wedge a3 ...)\vee b = (a1 \vee b)\wedge (a2 \vee b)\wedge (a3 \vee b)...$
		- $(a \vee b)\wedge c = (a \wedge c)\vee (b \wedge c)$ ---> 同上理
		
	- 一般命题句子化简为CNF步骤
		1. 替换 等价
		2. 替换 imply
		3. 将 $\neg$ 放进去
		4. 任何 $\vee$ 放进 clauses 用 $\wedge$ 连接 clauses


- ==CNF检测== --- 检查一个CNF是否具备 satisfiability，具备返回 True 否则 返回 False，最后的part-model就是一个使其为真的可能世界。假设我一开始给了一个partmodel那么就是检测在已知的partmodel情况下是否还具备可满足性  --- 属于 model-checking的proof方法
	- 第一个版本DFSS 使用 DFS来实现![](assets/DFSS.png)
		- 第一行是在 partmodel的情况下，任何子句都是真
	- ==DPLL 现代求解器的核心算法 --- 在 DFSS上增加了一些 tricks==
		- 三个重要 tricks
			- Early termination --- 当所有子句都满足 或者 某个子句不满足时 提前终止
			- Pure literals --- 如果一个字面量在所有尚未确认的子句中的符号都是一致的就将其设为那个符号值(比如，全部都是 $a$ 则将 a 设为 true，全部都是 $\neg a$ 则将 a 设为 false )
			- Unit clauses --- 当子句还有(一个)未确定的字面量时，将它设置为能使子句满足的值(T or F)
		- DPLL的实现![](assets/DPLL.png)
			- 第三行就是把所有 unknown的子句加进去
			- 下面给出一个运行DPLL的算法实例图![](assets/DPLLE.png)
		- Efficiency
			- 朴素的DPLL实现通常在解决 100 个左右变量问题时是可以在合理的时间中完成的
			- 通过一些额外的tricks，可以让它解决 1 亿个左右变量问题

- ==A knowledge-based agent==
	- 基于知识库(KB)的代理的核心历程![](assets/KBAgent.png)
		- 其中 percept 应该就是运行SAT 求解器，感知周围环境 --- SAT 求解器以DPLL算法为核心
	- ==Planning as satisfiability 将 Plan 视作 可满足性==
		- 从动作变量(action variables)的真值中获得解
		- 从SAT求解器解中读取动作变量
		- ==successor-state axiom 继承状态公理==
			- $X\_t \iff [X\_t-1 \wedge \neg(action\_t-1 \ made \ it \ false)] \vee  [\neg X\_t-1 \wedge (action\_t-1 \ made \ it \ true)]$
		- 对于Pacman来讲的logic形式化 --- 详情回顾[[L7 Logic]]![](assets/Pacman_logic.png)
			- 我们可以使用这种方法 Plan-to-find-path 也可以有以下的更多作用，详情见 [Project 3](../Project/Proj3/Project3%20Logic%20and%20Classic%20Planning.md)
	- ==Pacman KB logic agent ==
		- 可以这样问自己 $KB \wedge <actions> \wedge <percepts> |= At\_2,2\_6$ 是否成立
		- 在每次 action 和 percept 之后将已验证的字面量或称状态放入KB，比如 $KB \wedge action\_t-1 \wedge percept\_t |= X\_t$ 就可以将 $X\_t$ 添加进KB
		- 我们不蕴含准确的状态而是往往蕴含对状态的约束
		- Localization 定位问题 (让Pacman感知到自己的位置)
			- 吃豆人的可能位置是那些不能被证明为假的地点 --- 我们不蕴含准确的状态而是往往蕴含对状态的约束
			- 开始Pacman可能在任何位置且知道完整的map
			- percept and action
				- 每次 percept --- 感知哪些会被堵
				- 每次action --- 往某个不会堵的方向走
				- 如此循环直到明确自己的位置
		- Mapping 问题(让Pacman绘制出整张地图墙)
			- 吃豆人开始不知道Map的样子，或只知道部分Map样子
			- percept and action
				- 每次 percept --- 感知哪些会被堵(这里就有墙)
				- 每次action --- 往某个不会堵的方向走
				- 如此循环直到整个Map的墙都被找到
		- SLAM问题(一边定位一边绘图) --- 与上面类似
