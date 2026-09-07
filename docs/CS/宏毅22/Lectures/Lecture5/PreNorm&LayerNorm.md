
- 本文依托两篇论文阐述为什么是 LayerNorm 和 PreNorm
	- LayerNorm --- https://www.alphaxiv.org/overview/2003.07845
	- PreNorm --- https://www.alphaxiv.org/zh/overview/2002.04745


- 为什么是 LayerNorm
	- 为什么要 Norm 和 LayerNorm与BatchNorm的区别，在[Normalization](../../Lectures/Lecture5/P1%20Normalization.md) 已经说明了
	- 简单来说
		- LayerNorm 是对单样本的所有特征维度做Norm
		- BatchNorm 是对Batch中的所有样本的同一特征维度做Norm
	- 具体到NLP任务中
		- LayerNorm 是对每个token的所有特征维度做Norm
		- BatchNorm是对Batch内所有tokens的同一特征维度做Norm
	- 在Transformer中不同Batch的均值和方差变化剧烈(这是NLP的特性)，使得BatchNorm很容易组间不均使得训推不一致等问题出现


- 为什么是 PreNorm
	- PreNorm --- LayerNorm 放在残差分支内部，==残差主路径保持为恒等映射==，使梯度能够更稳定地跨越多个 Transformer block
		- 残差主路径提供了不经过 LayerNorm 的近似恒等梯度通路，减少 LayerNorm Jacobian对梯度的反复影响，使深层 Transformer 更容易稳定优化
	- PostNorm --- 梯度会反复受到 LayerNorm Jacobian 的影响，容易出现层间梯度不均衡
		- LayerNorm Jacobian 就是 LayerNorm 对输入向量的导数矩阵