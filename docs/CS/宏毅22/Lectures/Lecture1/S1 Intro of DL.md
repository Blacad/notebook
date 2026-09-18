
- DL 的沉浮
	- 1958：感知机（线性模型）
	- 1969：感知机有局限性
	- 1980年代：多层感知机
		- 与今天的深度神经网络没有显著差异
	- 1986：反向传播
		- 通常超过3个隐藏层没有帮助
	- 1989：1个隐藏层“足够好”，为什么还要深度？
	- 2006：RBM初始化（突破）
	- 2009：GPU
	- 2011：开始在语音识别领域流行
	- 2012：赢得ILSVRC图像竞赛


- 一旦网络结构给定就定义了一个函数集合function set，不同的参数就是不同的函数

- Fully Connect Feedforward Network(FFN) 全连接前馈网络 --- 最常见的神经网络结构

- 所谓 Deep 就是有很多hidden layer，但是具体多少不好说
	- AlexNet 8 layers
	- VGG 19layers
	- GoogleNet 22layers
	- ResNet 152layers

- 网络里的运算被表示为 Matrix Operation 即矩阵操作

- Layers
	- 中间的 hidden layers 作用可以被视作 特征工程
	- output layer 往往根据任务类型决定用途和设计，被用作多类分类器时会加上softmax

- 那么到底要多深多宽呢
	- 试错 + 直觉


