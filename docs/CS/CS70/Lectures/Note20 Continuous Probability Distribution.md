- [笔记](../Materials/Notes/20%20Distributions.pdf)
- 本节我们又学习了三个分布，针对Continuous RV
	- uniform 的 continuous r.v
	- Exponential
	- Normal

- continuous uniform probability distribution
	- 此前我们讨论的discrete RV 都聚焦于Countable的集合，我们希望向 Real-number 进行讨论
	- 考虑这样一个 fair 指针转盘，指针停在转盘位置的随机变量取自 $[0,l]$ 其中 $l$ 是转盘的周长，如果我们还是以离散视角看，假设指针只能停在 m 个位置，这样我们可以得到一个 discrete 的概率空间且是uniform的$\frac{1}{m}$，似乎当 $m \to \infty$ 时，每个概率点的概率都是0，如果这样讨论将会没有意义，因此我们需要定义新的方法 --- 区间的概率与其长度成正比
	- 请注意，区间是样本空间Ω的子集，因此是事件。所以与离散概率不同，在离散概率中我们为样本空间中的点分配概率，而在连续概率中，我们为某些基本事件（在这种情况下是区间）分配概率


- Continuous RV
	- 与上面的Continuous一样，我们需要重新定义RV即也是采用区间定义，这就需要 介绍概率密度函数的概念（有时简称为“密度”或“p.d.f.”）
	- (概率密度函数)。一个实值随机变量X的概率密度函数（p.d.f.）是一个满足以下条件的函数f：R → R：
		- f是非负的：对于所有x∈R，f(x)≥0。
		- f 的总积分为 1：$∫_{−∞}^{∞} f(x)dx = 1$
	- 然后，X 的概率分布由以下给出 
		- $P[a \le X \le b] = ∫_a^bf(x)dx$ for all a < b

- Cumulative Distribution Function 累积分布函数(c.d.f)
	- c.d.f 是这样的 $F(x) = P[X ≤ x] = ∫_{-\infty}^{x}f(z)dz.$
	- 因此 p.d.f 可以表示为$f(x) = \frac{dF(x)}{dx}$


- Continuous 的 E 和 Var
	- $E = ∫_{−∞}^{∞} xf(x)dx$
	- $Var = ∫_{−∞}^{∞} x^2f(x)dx - (∫_{−∞}^{∞} xf(x)dx)^2$

- Joint Distribution
	- 联合密度函数。两个随机变量X和Y的联合密度函数是一个函数f：$R^2 → R$，满足：
		- f是非负的：对于所有x，y属于实数，f(x, y) ≥ 0。
		- f的全积分为1：$∫_{-\infty}^{\infty}∫_{-\infty}^{\infty} f(x, y)dx dy = 1$
	- 然后，X和Y的联合分布由以下给出：
		- $P[a \le X \le b, c \le Y \le d] = ∫_c^d∫_a^bf(x,y)dxdy$ for all a < b and c < d


- Independence
	- （连续R.V.的独立性）。两个连续随机变量X，Y是独立的，如果对于所有a ≤ b和c ≤ d，事件a ≤ X ≤ b和c ≤ Y ≤ d都是独立的：
		- $P[a \le X \le b, c \le Y \le d]=P[a \le X \le b]\cdot P[c \le Y \le d]$



- Exponential Distribution 指数分布
	- 指数分布 是 几何分布的连续版本
	- 我们经常不是处理离散事件或离散时间，而是处理连续时间：例如，如果我们正在等待一个苹果从树上掉下来，它可以在任何时候掉下来，而不一定是在离散时钟的滴答声中。这种情况自然可以用指数分布来建模，定义如下：
		- 对于λ > 0，一个具有概率密度函数的连续随机变量X
		$$
			f(x)=
			\begin{cases}
			  \lambda e^{-\lambda x}& \text{ if } x \ge 0 \\
			  0 & \text{ if } others
			\end{cases}
		$$
		- 称为参数为λ的指数随机变量，我们记作X ∼ Exp(λ)。
	- E 和 Var --- 积分学好就好算
		- $E = \frac{1}{\lambda}$
		- $Var = \frac{1}{\lambda^2}$


	- 作为几何分布的连续时间模拟
		- 依据上述的式子，我们等待事件发生超过时间t的概率是$e^{−λt}$，这是一个以λ为速率的指数衰减



- Normal Distribution 正态分布(高斯分布)
	- 对于任何µ属于实数集R和σ大于0，连续随机变量X具有概率密度函数:
		- $f(x)=\frac{1}{\sqrt{2\pi\sigma^2}}e^{\frac{-(x-\mu)^2}{2\sigma^2}}$
	- 称为具有参数 µ 和 σ² 的正态随机变量，我们记作 X ∼ N(µ,σ²). 标准正态分布是指 $\mu=0,\sigma^2=1$
	- 正态分布的平移、放缩性质
		- 如果X服从$N(µ,σ^2)$，那么$Y = (X−µ)/\sigma$服从N(0,1)。等价地，如果Y服从N(0,1)，那么X = σY + µ服从$N(µ,σ^2)$ ---- 任何正态分布都可以通过标准正态分布平移和缩放得到
		- 推论 --- $X \sim N(\mu,\sigma^2), 则Y=aX+b \sim N(a\mu+b, a^2\sigma^2)$
	- E 和 Var
		- $E = \mu$
		- $Var = \sigma^2$
	- 正态分布贯穿于自然科学和社会科学，因为它是大批独立观察同一随机变量（如伯克利蚊子的重量或物理实验中的观测误差）所得到的汇总数据的标准模型。众所周知，这类数据倾向于围绕其平均值在“钟形”曲线上聚集，随着观察数量的增加，这种对应关系变得更加准确。这种现象的理论解释是==中心极限定理==

	- sum of independent normal distribution RV
		- 设X ∼ N(0,1)和Y ∼ N(0,1)为独立的正态分布随机变量，且a,b ∈ R为常数。则Z = aX + bY ∼ N(0, a² + b²)
		- 推论 ---- 设X服从$N(µ_X,σ_X^2)$分布，Y服从$N(µ_Y,σ_Y^2)$分布，且X和Y相互独立。那么对于任意的常数a,b∈R，随机变量Z=aX+bY服从正态分布，其均值$µ=aµ_X+bµ_Y$，方差$σ^2=a^2σ_X^2+b^2σ_Y^2$。


- Central Limit Theorem
	- (中心极限定理)。设X1,X2,...是一列独立同分布的随机变量，具有共同的有限期望E[Xi] = µ和有限的方差$Var(Xi) = σ^2$。令$Sn = \sum^n_{i=1} Xi$。那么，当n → ∞时，$\frac{S_n-n\mu}{\sigma\sqrt{n}} \le c$ 的分布收敛于N(0,1)。换句话说，对于任何常数c ∈ R，
		- $P[\frac{S_n-n\mu}{\sigma\sqrt{n}} \le c] \to \frac{1}{\sqrt{2\pi}}\int_{-\infty}^{c}e^{-x^2/x}dx$ 当 $n \to \infty$

- Buffon’s Needle 很有趣的逼近 $\pi$ 的方法

