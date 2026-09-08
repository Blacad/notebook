- [笔记](../Materials/Notes/17%20Concentration%20Inequalities.pdf)


- Markov's Inequality
	- For a nonnegative random variable X (i.e., X(ω) ≥ 0 for all ω ∈ Ω) with finite mean,for any positive constant c.
		- $P[X \ge c] \le \frac{E[X]}{c}$
	- proof 使用指示函数的证明方法非常不错，不等式两边同时取期望的方法也可以学习，建议看看note


- Generalized Markov's Inequality
	- Let Y be an arbitrary random variable with finite mean. Then, for any positive constants c and r
		- $P[|Y| \ge c] \le \frac{E[|Y|^r]}{c^r}$


- Chebyshev's Inequality
	- For a random variable X with finite expectation E[X] = µ, and for any positive constant c.
		- $P[|X-\mu| \ge c] \le \frac{Var[X]}{c^2}$


- Inequality Application
	- 对于知道 方差和期望的RV，可以使用Chebyshev's Inequality来评估不同规模采样下频率代替概率的 误差 与 可信度


- The Law of large number
	- Let X1,X2,..., be a sequence of i.i.d. (independent and identically distributed) random variables with common finite expectation E[Xi] = µ for all i. Then, their partial sums Sn = X1 +X2 +···+Xn satisfy for every ε > 0, however small.
		- $P[|\frac{S_n}{n}-\mu| < \varepsilon] \to 1$    $n \to \infty$