- [笔记](../Materials/Notes/18%20Applications.pdf)

- 通过balls-bins问题 以及 Union Bound 拓展分析 Hashing 和 负载均衡问题

- Hashing
	- 本质上就是让 m 个 balls 分配到 n 个 bins中，并且尽量保证每个bin中只有一个ball
	- 根据Union Bound分析  m = O(根号n)，更精确一些 $m\le \sqrt{2\varepsilon n}$ This means that the probability of having a collision is less than ε
	- 生日悖论也是hashing的一种特殊情形，它的n是365

	- A more accurate bound
		- 当 $m \le n$ 时，可以在 uniform的probability space中计算出 $\mathbb{P}[A]$ 于是就可以有非常精准的方法估算出不发生碰撞的概率
		- 于是再根据 ln的泰勒展开方法，进行替换得到 $m \le \sqrt{2ln(\frac{1}{1-\varepsilon})n}$


- Load Balance
	- 我们讨论的环境是 job 会随机选择一个 processor，相当于没有沟通情况下的 load balance
	- 我们依然假设 m 个 jobs 和 n 个 processors，设立一个事件 $A_k$ 是某个processor的load >= k
	- 从一般到特殊分析 $A_k(1)$ 表示 processor 1 的 load >=k，再通过逆推
		- 我们需要 $P[A_k]= P[U_{i=1}^nA_k(i)] \le \sum_{i=1}^{n}P[A_k(i)] \le n*P[A_k(i)] =\frac{1}{2}$
		- 于是 需要满足 $P[A_k(i)] \le \frac{1}{2n}$
		- 我们设立一个事件 $B_S$ 表示所有在集合S中的jobs都放在 processor 1 里了(S的基数为k)
		- 于是 $P[A_k(1)] \le P[U_SB_S] \le \sum_SP[B_S] = C_n^k\frac{1}{n^k} \le \frac{1}{2n}$
		- 于是 $k$ 约等于 $\frac{lnn}{lnlnn}$
	- 更紧密的分析似乎还不如上面的分析更有洞见

