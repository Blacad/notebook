
- Markov Models 又称 Markov Chain\ Markov Process
	- X在某一时刻的值称为状态State（通常为离散、有限）
	- 状态转移模型(transition model ) P(Xt | Xt-1)指定了状态随时间演化的方式
	- 主要构成
		- 初始分布概率 P(X0)
		- 概率转移矩阵\ 状态转移模型(transition model ) P(Xt | Xt-1)
	- 平稳性假设：在所有时间点的转移概率相同
	- 马尔可夫假设：“给定当前的情况下，未来与过去无关”
		- Xt+1 在给定 Xt 的情况下与 X0,.., Xt-1 独立
		- 这是一个一阶马尔可夫模型（一个 k 阶模型允许依赖于 k 个更早的步骤）
		- 我们在本节==考虑 一阶Markov model==
	- Joint distribution $P(X_0,...,X_T)=P(X_0)\prod_t P(X_t|X_{t-1})$
	- 形态
		- ![MC.png](../assets/MC.png)

- 在 cs70 中有Markov Chain(一阶)，它的重要三元素+重要性质：
	- 三元素
		- 状态空间
		- 概率转移矩阵
		- 初始分布概率
	- 重要性质
		- 不变量 invariance\ stationary distribution 平稳分布
		-  递推式 $P[X_n=i]= \pi_0P^n(i)$

- 一些应用的例子
	- n-gram models n元模型，较早的生词模型
		- 状态：文本中位置t的词（也可以构建字母n-gram）
		- 转换模型（概率来自经验频率）：
			- Unigram（零阶）：P(Wordt= i)
			- Bigram（一阶）：P(Wordt = i | Wordt-1=j)
			- Trigram（二阶）：P(Wordt = i | Wordt-1= j, Wordt-2= k) 

	- Web browsing
		- 状态：步骤t访问的URL
		- 转换模型：
			- 以概率p随机选择一个出站链接
			- 以概率（1-p）选择任意新页面


