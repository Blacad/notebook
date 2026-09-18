
- 不同的任务类型
	- Regression 回归 --- 输出 scalar
	- Classification 分类 --- 输出 class
	- Structured Learning 结构学习 --- 输出结构内容，比如图像、文档等等


- [Function with Unknown Parameters](../../Slides/Lecture1/regressionP-1+2.pdf#page=9)



- [Define Loss from Training Data](../../Slides/Lecture1/regressionP-1+2.pdf#page=10-13)
	- Loss 是关于参数的函数，反映一组值的好坏
	- MAE --- $e=|y-\hat{y}|$
	- MSE --- $e=(y-\hat{y})^2$
	- Cross-entropy --- 当 $y$ 和 $\hat{y}$ 都是概率分布时


- [Optimization](../../Slides/Lecture1/regressionP-1+2.pdf#page=14-18)
	- 这里介绍的是 Gradient Descent 很常规了


- [Linear models](../../Slides/Lecture1/regressionP-1+2.pdf#page=22-38)
	- 将输入逐渐从前1天变成 前7天，再变成前 28 天，可以看到效果越来越好但是后续边际效应递减，这也就构成了线性模型

	- 但是线性模型过于简单了，它永远无法模拟非线性规律，这也被称为 Model Bias(这个Bias和参数的Bias不一样)

	- 我们需要更加复杂的模型
		- 第一，在中间输出层(hidden layer)后面加上 激活函数，这里用的是 Sigmoid写作$\sigma$，这样就能拟合非线性
		- 第二，增大模型的宽度Fat，这样同一层就会有更多的激活函数，能够拟合更加复杂的曲线

	- $\theta$ 用来表示所有未知参数

- [ML Framework](../../Slides/Lecture1/regressionP-1+2.pdf#page=39-61)
	- gradient 梯度 --- $g=\nabla L(\theta^k)$
	- batch 更新策略 --- 每个batch计算loss并进行更新而不是整个epoch
	- 除了Sigmoid 外，还有其它激活函数比如 ReLU
	- 我们不仅可以加宽Fat还可以加深Deep，但这并不是永远好的，在训练上会更加困难，很容易出现各种问题
	- overfitting 过拟合 --- 训练数据上表现好，未见数据上表现差



