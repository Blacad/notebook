- 本节主题 --- 神经网络的应用和训练

- 神经网络 Neural Network --- 逻辑回归的自然推广
	- 神经网络示意图![NN.png](../assets/NN.png)
		- 这里用的激活函数是Sigmoid，现在大多数用的是ReLU
		- 如果不用激活函数深度学习将没有意义

- 讲了很多关于神经网络的矩阵运算，这些东西都很熟了，贴个图
	- 其余类似![NNMC.png](../assets/NNMC.png)

- 常用的激活函数 非线形
	- Sigmoid
	- Hyperbolic Tangent 双曲正切
	- ReLU

- Batch Computation 批计算
	- 图例![BatchComputation.png](../assets/BatchComputation.png)



- Loss 损失
	- ![Loss.png](../assets/Loss.png)


- Optimization --- 基于 Hill Climbing
	- Gradient Ascent 梯度上升优化
		-  $w=w + \alpha \cdot \nabla logP(y|x;w)$
	-  Gradient Descent 梯度下降优化
		- $w=w - \alpha \cdot \nabla logP(y|x;w)$
	- Stochastic Gradient Ascent 随机梯度上升优化
		- 之前是所有数据点的梯度都会用来更新，随机梯度优化则是只随机选择部分数据点的梯度进行更新，更新公式不变依然是 $w=w + \alpha \cdot \nabla logP(y|x;w)$
		- 

- 微分计算和反向传播
	- 微分计算就是 自动微分程序
	- 反向传播利用 微分的 Chain Rule