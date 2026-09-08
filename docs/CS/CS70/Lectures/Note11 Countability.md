- [笔记](../Materials/Notes/11%20countability.pdf)

- 集合间的映射关系
	- one-to-one
		- 每个输入的都有对应的输出且不同
	- onto
		- 每个输出都有对应的输入
	- Bijection
		- one-to-one + onto


- 基数 Cardinality
	- 证明两个集合的基数(大小)相同的方法
		- 寻找两者间是否存在 `bijection` 
		- 或者 `one-to-one` vice verse
		- 或者 `onto` vice verse
		- 这样对于有限集间的基数相同问题可以 自然过渡到 无限集间的基数相同问题
	- 集合Countable的定义
		- 如果集合的基数与自然数集合或其子集有相同的基数，则该集合Countable
	- 一些有关基数的基本洞见
		- $if \ A \subseteq B, then \ |A| \le |B|$
		- $if \ 存在 \ f:A->B \ one-to-one, then \ |A| \le |B|$
		- $if \ 存在 \ f:A->B \ bijection, then \ |A| = |B|$
		- $if \ 存在 \ f:A->B \ onto, then \ |A| \ge |B|$
	- 四个有趣证明
		- 整数集是Countable的 --- odd even
		- 有理数集是Countable的 --- $\mathbb{Z} * \mathbb{Z}$ 图
		- 二进制字符串集(字符串有限长)是Countable的 --- list
			- 注意 整数集的元素位数都是有限的，但是它没有上界且基数是infinite的 --- [限与界](补充/限与界.md)
			- 同理，二进制字符串集合基数是infinite的，总能找到更大的n+1
			- onto的证明是更重要的一环，可以考虑按照长度分层，利用假设法反证一定映射到所有字符串
			- 企图用同样的方法证明 无限长字符串则会有超实数出现，明显不是整数，因此该方法失效，事实上无限长字符串集合Uncountable(Cantor's diagonalization易证)
		- 自然数系数的多项式集合是Countable的 --- Z --> 多项式集--> {0,1,2}\*


- Cantor's Diagonalization 康托尔对角化 --- 证明得非常漂亮
	- 证明实数集是UnCountable的
		- 任何实数都可以唯一写成不尾随0的无限小数形式(1.00 ---> 0.999...)
		- 假设存在双射，然后构造一个实数，证明该实数不在这个双射中，证伪
	- 一个小谬论的说明
		- 对角线法成功的典型场景是证明实数（或 \[0,1\] 上带无限小数展开的数）、或二进制的无限序列集合（集合中每个元素是一个无限序列）是不可数。原因是：改变第 n 位得到的仍然是同一类对象（仍然是无限序列 / 实数的小数展开），所以构造出的新元素一定仍在欲证明的集合内，从而产生真正的反例
		- 整数是有限位数，而无限序列不是整数的一员，所以对角法造出的对象不必然在整数集合内。你企图在整数左侧补无穷多个0来构造无限字符串，但是整数从定义上来讲就不是无限的，你构造的对象左侧有无穷多个0，根据构造法，通过改变位新构造出来的对象并不会有无穷多个0，因此不属于这一类
		- 因此使用Cantor's Diagonalization证明的前提是，集合中任何元素都能写成无限序列的形式且是唯一的
- Cantor Set
	- 该集合通过无限次重复移除每个线段的中三分之一来定义，从原始区间 $[0,1]$ 开始，比如第一轮移除$(\frac{1}{3},\frac{2}{3})$后就成了 $[0,\frac{1}{3}]\cup [\frac{2}{3},1]$ 
	- 根据极限，无穷轮后 Cantor Set 似乎是空的，但其实不然，这只说明了它的测度(measure)为0，它不仅不是空并且还是Uncountable的集合，它至少包含许多端点值比如0、1/3等等
	- 集合的测度(measure) 和 基数(cardinality)是不同的概念，基数描述“离散”意义上的数量，测度描述“连续”意义上的大小
		- **基数** 数点的个数
		- **测度** 量点所占的空间
	- 证明Cantor Set的UnCountable非常有趣，先使用三进制表示，然后形式化找到C，发现C是只包含0's和2‘s的集合，那么我们将2全部替换为1，构成二进制的所有0-1的表示，即得到 C到$[0,1]$的onto，于是C不可数

- Power Set and Higher Orders of Infinity
	- Power Set：设S为任意集合。那么S的幂集，记作$\mathcal{P}(S)$，是S的所有子集的集合
	-  定理：$|\mathcal{P}(\mathbb{N})|>|\mathbb{N}|$ 的证明
		- Note上的证明不错，仍然是Diagonalization证明
	- 剩下就是现代数学Set Theory还在探讨的了，我们就不多说了
