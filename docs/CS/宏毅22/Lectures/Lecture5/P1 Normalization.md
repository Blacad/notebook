
- 当输入值较小时，权重更新后的变化就能[较小](../../Slides/Lecture5/normalization_v4P1.pdf#page=2)，但是输入值很大时，权重更新后的变化就会[很大](../../Slides/Lecture5/normalization_v4P1.pdf#page=3)
	- 我们需要一个方式能够将各输入值限定在适当范围，这样能够让 [error surface](error%20surface.md) 更加平缓，不至于非常陡峭
	- 这样的方式被称为 [Feature Normalization](../../Slides/Lecture5/normalization_v4P1.pdf#page=4)，其中 Batch Normalization 是方法之一

- [BatchNorm](../../Slides/Lecture5/normalization_v4P1.pdf#page=6-8)
	- BatchSize 尽量大，这样Batch拟合的均值和标准差更接近整体分布
	- BatchNorm 在Testing时可能会有问题，因为我Testing时可能就是一个个输入，没有所谓的Batch，那么你的BatchNorm就会有问题，但是实际中pytorch已经帮我们处理好了，在训练时它会统计所有batch的均值和标准差，然后通过 moving average(TD) 去算最终的均值和标准差，在测试时就直接用这个最终的均值和标准差，[正如图示](../../Slides/Lecture5/normalization_v4P1.pdf#page=9)

- [更多Norm](../../Slides/Lecture5/normalization_v4P1.pdf#page=13)

- 这里补充一下 LayerNorm，它不是对整个Batch做Norm，而是对单样本做Norm
	- LayerNorm 不是对整个 batch 做归一化，而是对单个样本内部的特征维度做归一化。比如输入向量是 40 维，batch size 是 30，那么 BatchNorm 会对每个特征维度，使用 batch 中 30 个样本的值来计算均值和方差；而 LayerNorm 会对每个样本自身的 40 个维度计算均值和方差