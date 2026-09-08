- [笔记](../Materials/Notes/21%20Markov%20Chains.pdf)
- Markov Chains 介绍
	- 马尔可夫链是有限或可数集上的随机运动模型。这些模型之所以强大，是因为它们捕捉到了我们在应用中遇到的大量系统。然而，这些模型又很简单，因为它们许多性质通常可以使用初等矩阵代数来确定。在本课程中，我们将讨论限制在有限马尔可夫链的情况，即在有限集上的运动


- 第一个简单例子 --- 只有01两个取值的Markov chain
	- 时间n=0,1,2,...时的位置是Xn ∈ {0,1}，我们称Xn为马尔可夫链在第n步（或时间）的状态
	- {0,1} 是该markov chain 的 state space，Xn的运动，即时间演化，遵循以下规则 --- 即：给定一个数a ∈ [0,1]和两个非负数π0(0)和π0(1)，它们的和为1
	- $P[X_0=0]=\pi_0(0)$ 和 $P[X_0=1]=\pi_0(1)$ 这是初始状态的概率
	- 从上一个状态转移到下一个状态的概率称为==马尔可夫链的转移概率==
	- ==马尔可夫链的状态转移图==中描绘了状态的转移概率 和 所有状态
	- 对于这个简单的例子可以用一个2x2矩阵描述 $P(0,0)=1-a,P(0,1)=a,P(1,0)=a,P(1,1)=1-a$
		$$
		\begin{bmatrix}
		  1-a & a \\
		  a & 1-a
		\end{bmatrix}
		 $$


- ==有限Markov Chain==
	- 定义有限Markov Chain 即：状态空间是 $\mathcal{X}= \{1,2,...,K\}$，其中 K 是某个有限数。转移概率矩阵 P 是一个 K × K 矩阵，使得
		- $P(i,j)\ge 0, \forall i,j \in \mathcal{X}$ and $\sum_{j=1}^{K}P(i,j)=1,\forall i \in \mathcal{X}$
	- 初始分布是一个向量 $\pi_0 = \{\pi_0(i), i \in \mathcal{X}\}$，其中对于所有 $i \in \mathcal{X}，\pi_0(i) \ge 0$，且 $\sum_{i\in \mathcal{X}} \pi_0(i) = 1$

	- 基于上述的 ==状态空间 + 概率转移矩阵 + 初始分布概率向量==，可以描述随机序列
		- $P[X_0=i]=\pi_0(i),i\in \mathcal{X}$ 和 $P[X_{n+1}=j|X_n=i,X_{n-1},...,X_0]=P(i,j),i,j \in \mathcal{X}$
		- 于是 $P[X_n=i]= \pi_0P^n(i)$ 表达式是行向量π0与矩阵P的n次幂的乘积的$i$分量
		- ==其实根据矩阵乘法的性质，我们可以得知 $P^2$  中 $P^2(i,j)$ 是 初始状态是i的情况下，第二步后状态是 j 的概率 ，依次类推 $P^n(i,j)$ 是 初始状态是i的情况下，第n步后状态是 j 的概率，于是就自然得到Markov Chain 最重要的递推式

	- ==如果我们用πn表示Xn的分布，即使得$P[X_n = i] = \pi_n(i)$，那么最后的推导证明了以下结果 --- Markov Chain 递推式==
		- $\pi_n=\pi_0P^n,n \ge 0$ 
		- 其实这是很自然也非常有意义的，$\pi_0$ 表示初始时各状态出现的概率，而 $\pi_i$ 表示第 i 步时各状态出现的概率，是 Markov Chain 最终的递推式



- ==Balance Equation==
	- ==不变分布的重要概念 invariant distribution --- $\pi$==
	- 分布π对转移概率矩阵P不变，如果它满足以下==平衡方程==
		- $\pi = \pi P$
	- 由平衡方程可知
		- 当且仅当 $\pi_0$ 满足平衡方程(invariant)时，$\pi_0=\pi_n ,\forall n \ge 0$ --- 还是比较好证明的

	- 因此一个有意思的问题就是寻找 $P$ 的 invariant $\pi$，它提供的额外条件正是 Balance Equation，比如上面那个 第一个简单例子的 invariant $\pi=\{1/2,1/2\}$

	- 那么 一个 Markov Chain 的 invariant 是否唯一呢，当然不是唯一的，但是大多数情况下是唯一的，后面会有个定理来限制的(不可约的有限Markov Chain的invariant 唯一)，一个例子比如就是上面那个简单的例子 如果 a=0，那么状态一直不变不就是不唯一吗


- ==Fraction of time in States --- 状态的时间分数==
	- ==一个马尔可夫链在状态i上长期停留的时间有多长？而我们给该状态的时间分数又该是多少==
		- 我们通过式子$\lim_{n\to \infty}\frac{1}{n}\sum_{m=0}^{n-1}I\{X_m=i\}$ 来表示 马尔可夫链在状态 i 上的时间分数
		- 其中 $\sum_{m=0}^{n-1}I\{X_m=i\}$ 表示前 n steps 是 状态 i 的数量，然后 /n 表示前 n steps 的均分
	- 为了研究该问题引入一个概念 ==Irreducible (不可约)  ---- 马尔可夫链是不可约的，如果它可以从每个状态i到达每个其他状态j，可能需要多步==
	- ==不可约Markov Chain invariant 唯一且是状态时间分值定理 ---- 考虑一个有限不可约马尔可夫链，其状态空间为X和转移概率矩阵为P。那么，对于任何初始分布π0==
		- $\lim_{n\to \infty}\frac{1}{n}\sum_{m=0}^{n-1}I\{X_m=i\}= \pi(i)$ 其中 $\pi$ 是 该Markov Chain 存在的唯一的不变分布
		- proof的话在附录这里就不赘述了

	- ==不可约Markov Chain 周期定理 --- 考虑在X上的一个不可约马尔可夫链，其转移概率矩阵为P==
		- 定义 $d(i) := g.c.d\{n > 0 | P^n(i,i) = Pr[Xn = i|X0 = i] > 0\},i \in X$ --- 注意初始状态为 i 的前提下
		- 1 ==d(i)对所有i∈X具有相同的值。如果这个值是1，则称马尔可夫链为非周期的。否则，它被称为周期为d的周期性链==
		- 2 ==如果马尔可夫链是非周期的，那么==
			- 如果马尔可夫链是非周期的，那么 $Pr[X_n=i] \to \pi(i),\forall i \in X, \quad as \quad n \to \infty$ 其中 $\pi$ 是该Markov Chain 的唯一的 不变分布
		- 其中 $d(i)$ 表示的是能从状态 i 到 状态 i 可能的所有步的集合的公约数
		- 例如：
			- 以上述简单的two-state Markov Chain 为例
				- 如果 $a \in (0,1)$ ，那么 d(0)= g.c.d{1,2,3,.... }=1, d(1)=g.c.d{1,2,3,....}=1(所有步都可能是0或者1)，于是 $Pr[X_n=0]=\pi(0)=1/2$
				- 如果 a=1，那么 d(0)=g.c.d{2,4,6,...}=2, d(1)=g.c.d{2,4,6,..}=2，周期为2

		- 这里的证明也在附录不再赘述




- Hitting Time --- 碰撞时间
	- ==考虑 笔记 中的 Figure 3，如果我们想从状态 A 到 状态 E 平均需要多少步==
		- 我们定义 $\beta(i), i \in \{A,B,C,D,E\}$  表示从状态 i 到状态 E 的平均步数
		- 我们需要 求 $\beta(A)$，但是我们发现需要求它就需要求 $\beta(B)$ 和 $\beta(D)$
		- 可以得到式子 $\beta(A) = 1 + \beta(B)* 1/2 + \beta(D) * 1/2$ (走每个出度的概率都相同)
		- 同样的，我们可以得到一系列式子 从 $\beta(A)$ 到 $\beta(E)$ 这些式子被称为 ==first step equations 简称 FSE==
			- 这样我们通过连立我们可以依次得到  从 $\beta(A)$ 到 $\beta(E)$的所有值
	- 上述的FSE式子得到的结果就是(到状态E的) Average Hitting Time
	- 同样的，我们还可以扩充定义，从状态空间中寻找一个 subset 寻找各状态到 subset状态的 Average Hitting Time，方法也是一样的使用 FSE式子求解

	- 将问题归约为 Markov Chain 也需要很好的设计思维 --- 一般引入新状态 S和T
		- 比如连续两次硬币正面向上的 Average Hitting Time
		- 比如投骰子最后点数和为8 的 Average Hitting Time


- Probability of A before B
	- ==我们从状态空间中选取两个不相交的子集 A，B，我们想问从状态i开始，马尔可夫链在进入B中的任何一个状态之前进入A中的任何一个状态的概率α(i)==
	- 同样的，我们也可以列出 FSE 进行计算




