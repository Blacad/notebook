- [笔记](../Materials/Notes/15%20RV-distribution%20and%20expectation.pdf)

- RV
	- 定义： (Random Variable). A random variable X on a sample space Ω is a function X : Ω → R that assigns to each sample point ω ∈ Ω a real number X(ω).
	- 我们将限制我们的关注于离散随机变量，即它们在有限或可数无限的范围中取值，这意味着尽管我们定义X将Ω映射到R，但X实际取的值集{X(ω)：ω ∈ Ω}是R的离散子集

- Probability Distribution
	- 定义：The distribution of a discrete random variable X is the collection of values {(a,P[X = a]) : a ∈ A }, where A is the set of all possible values taken by X
		- X 为Ω中的每个可能的样本点分配一个唯一的概率值，总和恰好为1

 
	- Bernoulli Distribution
		- 两个可能值，一个p，一个1-p，参数为 p 的 bernoulli 分布
	- Binomial Distribution
		- n个可能值，从Set采样是independent的，参数为 n、p的Binomial 分布
	- Hypergeometric Distribution
		- n个可能值，从Set采样是dependent的，参数为 N、B、n的超几何分布


- Multiple RV和Independence
	- Joint Distribution定义：The **joint distribution** for two discrete random variables X and Y is the collection of values {((a,b),P[X = a,Y = b]) : a ∈ A , b ∈ B}, where A is the set of all possible values taken by X and B is the set of all possible values taken by Y .
		- 当给定X和Y的联合分布时，X的分布P[X = a]被称为边缘分布, 有$P[X = a] =  \sum_{b\in B} P[X = a,Y = b]$

	- Independence定义：Random variables X and Y on the same probability space are said to be independent if the events X = a and Y = b are independent for all values a,b. Equivalently, the joint distribution of independent r.v.’s decomposes as P[X = a,Y = b] = P[X = a]P[Y = b], ∀a,b.
		 - 如果$I_i$表示第i次掷硬币得到H的指示随机变量，那么$I_1,...,I_n$是相互独立的随机变量。这个例子激发了常用短语“独立同分布（i.i.d.）的随机变量集”。在这个例子中，$\{I_1,...,I_n\}$是一组独立同分布的指示随机变量集。 --- 同分布就是指各随机变量都是一样的概率分布，这里都是Berboulli Distribution
	 - 对于随机变量而言，独立的含义是指，一个随机变量取任何可能值对另一个随机变量取值的概率无影响


- Expectation
	- 定义：The expectation of a discrete random variable X is defined as $E[X] = \sum_{a\in A} a×P[X = a]$, where the sum is over all possible values taken by the r.v.


- Linearity of Expectation
	- 定理：For any two random variables X and Y on the same probability space, we have $E[X +Y] = E[X] +E[Y].$ Also, for any constant c, we have$E[cX] = cE[X].$

	- 一些应用