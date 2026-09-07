
- 当模型到critical point时， gradient 很小接近零，导致模型训练无法继续优化
	- critical point 的两种情形 --- 局部极值点 or 鞍点

- [数学分析](../../Slides/Lecture2/small-gradientP-2+3.pdf#page=4-12)
	- 不同的 critical point 区分方法 local or saddle 
	- saddle point 问题是可以解决的，但是算 H 需要大量计算因此实践中很少算H而是用其它方法逃离 saddle point

- local minima 很少


- 训练中的技巧
	- Batch 和 Momentum


- Batch 批次
	- 1 epoch = see all the batches once → Shuffle after each epoch
	- [大小批次对比](../../Slides/Lecture2/small-gradientP-2+3.pdf#page=28)
		- 虽然上述的对比中表明 small Batch 在优化和泛化上更好，但是实际中 batch 还是选择适中更好，因为 small Batch 的梯度噪声大在优化和泛化上不一定优于更大的Batch，只是说可能带来优势

- Momentum 动量
	- 用来解决 local 问题的，它依据物理中惯性的概念冲破 local 
	- [momentum](../../Slides/Lecture2/small-gradientP-2+3.pdf#page=33) --- 实践中 momentum 被视作更新的单元而不是单纯的梯度