- [笔记](../Materials/Notes/14%20conditional%20probability.pdf)
- 本章是讲由已知概率推导未知概率
- 一般来讲，条件概率是难以直接计算的，我们需要用 交集概率 和 事件概率去算条件概率，不要想用条件概率去算交集概率(除非给出条件概率)

- 本章1-3首先介绍了三个重要公式
	- 条件概率公式 --- $P(A|B)=\frac{P(A \cap B)}{P(B)}$
	- 贝叶斯定理 --- $P(A|B)=\frac{P(B|A)P(A)}{P(B)}$
	- 全概率公式 --- $P(B) = P(A\cap B) + P(\bar{A}\cap B)$
	- 3.2节将 事件分割，定义了更加广泛的贝叶斯定理和全概率公式


- 本章4介绍了独立事件、product rule\chain rule、并集计算
	- 独立事件
		- 独立性定义 --- $P(A\cap B)=P(A)*P(B)$ ---> $P(A|B)=P(A)$
		- 互相独立定义 --- $P(\cap_{i\in I}{A_i})=\prod_{i\in I}P(A_i)$ ---> $P(A_{i}|\cap_{j\in I}{A_j})=P(A_i)$
	- 计算事件交集概率
		- Product Rule\Chain Rule --- $P(A\cap B)=P(A)P(B|A)$ 以及其平凡形式，可以用数学归纳法简单推演其平凡形式
	- 计算事件并集概率
		- Inclusion-Exclusion
			- $P(A_{1}\cup A_{2}... \cup A_{n}) = \sum_{k=1}^{n}(-1)^{k-1}\sum_{S\subseteq \{1,...,n\};|S|=k}{P(\cap_{i\in S} A_{i})}$ 原文中有展开式
			- 对于 互相排斥事件($A\cap B=\phi$)，就是简单相加
			- Union Bound  ---- $P(A_{1}\cup A_{2}... \cup A_{n}) \le \sum_{i}^{n}P(A_{i})$

