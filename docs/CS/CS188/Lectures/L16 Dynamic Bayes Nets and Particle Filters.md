- 动态贝叶斯网络 DBNs
	- DBNs 本质上就是 HMMs + BNs，在每个时间步重复固定的贝叶斯网络结构，时间t的变量可以具有时间t或t-1的父母
	- 主要构成
		- 多个初始概率分布 P(X0)P(X1)....
		- 多个状态转移矩阵
		- CPTs + 网络拓扑结构
	- 形式
		- ![DBNs.png](../assets/DBNs.png)
	- 一个DBNs的简单例子
		- ![DBNsExample.png](../assets/DBNsExample.png)
	- 每个HMM都是一个单变量DBN，因此DBNs其实是更为复杂的HMMs

- Exact Inference in DBNs
	- 离线方法 --- 对T时间步，将网络展开然后消除变量以找到 $P(X_T|e_{1:T})$
	- 在线方法 --- 消除之前时间步的所有变量；仅存储当前时间的因子
	- 精确推断在DBNs里往往都太大，因此更常用 Approximate Inference

- Approximate Inference --- Particle Filters 粒子滤波算法
	- 用一组样本表示信念状态
		- 样本被称为粒子
		- 每一步的时间与样本数量成线性关系 
		- 但：所需数量可能很大
	- 表示 
		- $P(X)$ 是一系列 particles
		- predict step
			- 粒子j在状态xt直接从转移模型中采样新状态：$x_{t+1}^{(j)} \sim P(X_{t+1}|x_t^{(j)})$
		- update step - after observing et+1
			- 像likelihood weighting，我们对每个 sample 基于 evidence 加权 $w^{(j)}=P(e_{t+1}|x_{t+1}^{(j)})$
		- Resample --- 在权重分布中进行重采样
	- 与精确推理相比，精确推理是维护真正完整的X的分布，但是近似推理只保存一定数量的粒子(样本)，X的分布由这些样本得到(某种粒子数量/总粒子数)
		- 因此一开始，我们采样一定粒子保存(初始化采样)，使用时可以用这些粒子得到X的分布
		- 用predict和update完成X分布的更新后，根据新X重新采样一定数量粒子，这些粒子表明X的新分布
		- 如此反复
	- 由此可知，假设X有$10^{20}$种状态，而我们的粒子数量选定 $10^{5}$ 个，这样我们能节省很多空间也能提高计算效率，逐渐将概率低的粒子剔除
	- 基本思路就是上面的逻辑，但是想看更加详细的算法，可以看[Project4 Ghostbusters](../Project/Proj4/Project4%20Ghostbusters.md#hmmsdbns-approximate-inference) 的


