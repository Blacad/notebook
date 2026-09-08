- [笔记](../Materials/Notes/19%20Geometric%20and%20Poisson%20Distributions.pdf)
- 首先回顾 Probability distribution 本质上是描述 随机变量的可能取值及其概率的二元对集合。
- 而我们一般写的不同分布的表达式本质上就是在算 随机变量不同取值下的概率通式，以此表示集合
- 因此，当我们考察一个随机变量 X 符合何种分布时，不妨写出 $P[X=i]$ 的式子一定能马上知道是什么分布 或者 看该事件与我们的典例事件是否是一致的也可以判断。泊松分布相对比较特殊，额外来看
- 我们目前学的分布也就五种
	- Bernoulli
	- Binomial
	- Hypergeometric
	- geometric
	- poisson

- ==Geometric Distribution==
	- A random variable X for which $P[X=i]=(1-p)^{i-1}p$  is said to have the geometric distribution with parameter p. This is abbreviated as X ∼ Geometric(p)
		- 常见的随机变量 --- 恰好第 i 次投硬币正面朝上(前i-1次均背面朝上)

	- ==Tail sum formula 尾部加和公式==
		- 如果一个随机变量的可能取值均取自自然数集，那么 $E(X) = \sum_{i=1}^{\infty}P[X \ge i]$
		- proof 比较简单，可以直接看note


	- 由尾部求和公式我们计算 图分布的期望 E
		- $P[X \ge i]=(1-p)^{i-1}$
		- 已知 $0 < p < 1$ ，等比数列求和，$E(X)=\sum_{i=1}^{\infty}(1-p)^{i-1} =\frac{1}{p}$

	- 由期望来求方差 Var
		- 我们需要用到 E 的线形特征 和 LOTUS规律$E[f(X)] = \sum_{x}f(x)P [X = x].$ 
		- 甚至我们需要通过对 E 等式两边求导得到进一步的结论，即对p两边求导$\sum_{i=1}^{\infty}(1-p)^{i-2}(i-1)(-1) = -\frac{1}{p^2}$
		- 再求一次导  ---- $\sum_{i=1}^{\infty}(i-1)(i-2)(1-p)^{i-3}= \sum_{i=2}^{\infty}i(i-1)(1-p)^{i-2} = \frac{2}{p^3}$
		- 依据LOTUS  ---- $E(X(X-1))=\sum_{i=1}^{\infty}i(i-1)(1-p)^{i-1}p=\frac{2(1-p)}{p^2}$
		- 依据E的线形特征 --- $Var(X) = E(X^2)-E(X)^2 = E(X(X-1))+E(X)-E(X)^2 =\frac{1-p}{p^2}$


	- Application --- ==The Coupon Collector’s Problem 谷物收集问题(抽池子问题)==
		- 问题描述 ---- 假设我们正在尝试收集一套n张不同的棒球卡。我们通过购买谷物盒来获得这些卡片：每个盒子包含一张卡片，这张卡片是n张卡片中的任意一张，概率均等。我们需要购买多少个盒子才能收集到至少每张卡片的副本？
		- 设 Sn 表示收集所有 n 张卡片所需的购买盒数，则 Sn = X1+...+Xn，Xi是在尝试获得第i张新卡时还需购买的盒子数量(前提是已经有i-1张新卡了)
		- 获得第一张新卡时，我们只要买了box就一定能获得第一张新卡，于是P[X1=1] = 1，因此 $E(X1)=1$
		- 考虑 X2，即获得第二张新卡时还购买的盒子数量，获得旧的概率是 $\frac{1}{n}$，新的概率是 $\frac{n-1}{n}$，再买第一个盒子就获得第二张新卡(相当于投一次就是head)依次类推符合图分布因此 $E(X2)=\frac{n}{n-1}$
		- 同理可推 Xi，$E(Xi)=\frac{n}{n-i+1}$
		- 因此 $E(Sn)= \sum_{i=1}^{n}E(Xi)= n\sum_{i=1}^{n}\frac{1}{i} \approx n(lnn+\gamma_E)$ --- ==欧拉常数==


- ==Poisson Distribution== --- 泊松分布
	- A random variable X for which $P[X=i]=\frac{\lambda^i}{i!}e^{-\lambda}$ for i =0,1,2,.... is said to have the Poisson distribution with parameter λ
	- 通过 $e^x$ 的泰勒展开或者说级数展开可以证明这个随机变量定义是合理的
	- 泊松分布也是所谓“罕见事件”的非常广泛接受的模型，例如误接的电话、放射性辐射、染色体交叉、疾病病例数、每小时出生数等。
	- 此模型适用于在连续区域（时间或空间）中，发生事件可以假设为随机发生且具有某种恒定密度的情况，使得不同子区域中的事件是独立的。然后可以证明，在单位面积区域中发生事件的次数应服从参数为λ的泊松分布。
		- Example，假设我们写文章时，每页平均有1个错别字。我们可以用λ=1的泊松随机变量X来模拟这种情况。因此，一页有5个错别字的概率是 $P[X=5]=\frac{1^5}{5!}e^{-1} \approx \frac{1}{326}$



	- ==泊松分布的E 与 Var==
		- $E=\lambda$
			- proof 还是使用级数展开比较简单证明
		- $Var = \lambda$
			- proof 也是借助 E 求得的结果通过两边同时求导再通过LOTUS+线形性质来得到 Var，也比较简单证明


	- 两个独立的泊松分布随机变量的加和
		- 设X服从参数为λ的泊松分布，Y服从参数为µ的泊松分布，且X和Y相互独立。那么，X+Y服从参数为λ+µ的泊松分布
		- 证明就采用独立性和Joint Distribution方法证明比较简单
		- 同时可以推广为 n 个相互独立的泊松分布加和


	- 泊松分布是二项分布的极限
		- 我们考虑这样一个场景，X 表示 1 min钟内中国人发起通话的人数量，我们把 1 min 分成 n 段小时间，每段小时间发起通话的概率为 p (重要假设，小时间发起通话的人数大于1的概率忽略不计，不同小时间发起通话独立)
		- X ～ Bin(n,p)
		- 那么假设 每分钟的平均呼叫次数 为 $\lambda$，则 $np=\lambda$ 即 $p=\frac{\lambda}{n}$
			- $P[X=i]=C_n^i (\frac{\lambda}{n})^i(1-\frac{\lambda}{n})^{n-i}$ 当 n 趋近 无穷 $P[X=i]=\frac{\lambda^i}{i!}e^{-\lambda}$ --- 极限学好了就很好证明
