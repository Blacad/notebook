## SAE

**标准 Sparse Auto-Encoder（SAE）里，功能向量/特征向量本身也是训练出来的，并不是事先给定一组固定的功能向量，只去寻找线性组合系数。**

可以把 SAE 理解成在同时学习两件东西：

$$x \approx \sum_{i=1}^{m} z_i d_i + e$$

其中：

- x：原始模型某一层的 activation；
- $d_i$：第 i 个**特征向量 / dictionary feature**；
- $z_i$：这个特征在当前样本上的激活强度；
- e：重构误差。

关键是：

> **d_i 和如何得到 z_i 的 encoder 参数，都会在训练过程中更新。**

SAE 通常是：

$$z = \operatorname{SparseActivation}(W_{\text{enc}}x+b_{\text{enc}})$$

然后：

$$\hat{x}=W_{\text{dec}}z+b_{\text{dec}}$$

这里 $W_{\text{dec}}$ 的每一列就可以看成一个 learned feature vector：

$$W_{\text{dec}} = [d_1,d_2,\dots,d_m]$$

所以训练开始时，这些 $d_i$ 一般是随机初始化的（或通过某种初始化策略得到），然后随着训练不断变化。训练目标大致是：


$$\mathcal L = \underbrace{\|x-\hat x\|_2^2}_{\text{尽量重构原 activation}} + \lambda \underbrace{\|z\|_1}_{\text{让特征激活稀疏}}$$

因此 SAE 做的事情实际上可以概括为：

> **从大量 activation 中，同时寻找一组“合适的基向量 di”和每个 activation 对这些向量的稀疏组合系数 zi**

$$\vec{w}=a\vec{u}+b\vec{v}$$


## 词典学习

不过和普通线性代数里的“找一组基”还有一个重要区别：SAE 通常是 **overcomplete** 的，比如原 activation 是 4096 维，但 SAE 可能学习 32768、65536 甚至更多个 feature vectors。因此它不是寻找普通的 4096 个正交基，而是在寻找一个很大的 **dictionary**，然后要求每个 activation **只使用其中很少几个 feature**：

$$4096\text{-dim activation} \quad\rightarrow\quad 65536\text{ 个候选 feature}$$

但某一个 token 可能只有：

$$z_{17},z_{305},z_{8211},\dots$$

少数几个非零。



$$\boxed{ \text{训练数据} \rightarrow \begin{cases} \text{学习 feature vectors }d_i\\ \text{学习如何产生 sparse coefficients }z_i \end{cases} }$$

等 SAE **训练完成以后**，我们才把这些固定下来的 $d_i$ 拿出来分析，例如发现某一个 feature 对「代码」「引用文本」「法国」「拒绝回答」等概念高度响应，于是给它赋予某种语义解释。

这也是为什么常说 SAE 是在做 **dictionary learning**：它不只是“在已有字典里查系数”，而是**连字典本身一起学**