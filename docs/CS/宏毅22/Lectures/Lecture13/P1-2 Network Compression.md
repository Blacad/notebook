
- 网络压缩的基本方法
	- Network Pruning 网络剪枝
	- Knowledge Distillation 知识蒸馏
	- Parameter Quantization 参数量化
	- Architecture Design 架构设计
	- Dynamic Computation 动态计算


- Network Pruning
	- [Pruning的基本做法](../../Slides/Lecture13/tiny_v7.pdf#page=6)
	- 两种剪枝对象
		- [剪枝 weights](../../Slides/Lecture13/tiny_v7.pdf#page=7)
			- 很难实现且难以加速
			- 因为在实现层面是矩阵(张量)，这样剪枝后会让网络不规则，一方面用 torch 不好写，另一方面 GPU也难以加速
		- [剪枝 neurons](../../Slides/Lecture13/tiny_v7.pdf#page=9)
			- 容易实现且容易加速
			- 一个 neuron↔ 一个长度为 $d_{in}$​ 的权重向量​
	- 为什么用剪枝呢
		- Lottery Ticket Hypothesis 彩票假说
			- 可以想象 大的model 包含许多小的model，只要其中一个小 model 成功，那么 大的model 就有成功的理论可能
			- [实验验证](../../Slides/Lecture13/tiny_v7.pdf#page=12-13)
				- 初始权重的正负号至关重要
			- [反对彩票假说](../../Slides/Lecture13/tiny_v7.pdf#page=14)


- Knowledge Distillation 知识蒸馏
	- [蒸馏的基本做法](../../Slides/Lecture13/tiny_v7.pdf#page=16-17)
	- [Temperature for softmax](../../Slides/Lecture13/tiny_v7.pdf#page=18)
		- 让 Sharp 的分布变得平缓一些

- Parameter Quantization 参数量化
	- [常见参数量化方法](../../Slides/Lecture13/tiny_v7.pdf#page=21)
		- 使用更少的 bits 去表示值
		- 用 聚类 的方式压缩同类参数进而使用更少的bits
		- 常出现的用少bits，少出现的用多bits --- Huffman encoding
	- 著名的方法
		- [Binary Weights](../../Slides/Lecture13/tiny_v7.pdf#page=22)


- Architecture Design
	- 著名方法
		- Depthwise Separable Convolution
			- [基本做法](../../Slides/Lecture13/tiny_v7.pdf#page=26-27)
				- Depthwise Convolution
					- 标准CNN中的 filter 应该是一个立方体，它的 channel 应该和输入的channel一样，然后每个 filter 产生输出的一个channel
					- Depthwise Convolution 的 filter 只是一个平面，它的每个 filter 只能看到输入的 单个 channel 的信息，同样每个filter 产生输出的一个channel
				- Pointwise Convolution
					- 由于 filter 只能看到同 channel 的信息，因此再引入 pointwise filter，它的 channel 与 输入的 channel 一样，但是平面只是 1 x 1 的，这是因为它的职责是跨channel 信息，同 channel 信息已经处理好了
			- [节省计算](../../Slides/Lecture13/tiny_v7.pdf#page=28)
	- Low rank approximation
		- M x N <-> M x K · K x N
	- [其它方法](../../Slides/Lecture13/tiny_v7.pdf#page=31)



- Dynamic Computation 动态计算
	- [Dynamic Depth](../../Slides/Lecture13/tiny_v7.pdf#page=31) 动态深度
	- [Dynamic Width](../../Slides/Lecture13/tiny_v7.pdf#page=32) 动态宽度
	- [动态决定](../../Slides/Lecture13/tiny_v7.pdf#page=36) 深度 or 宽度

