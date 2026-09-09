
- flow-based GM 的目标函数
	- 最大化 生成数据分布与原始数据分布 的相似度(最大似然)
	- [图示](../../Slides/Lecture6/FLOW.pdf#page=4)
	- 那么我们怎么去计算生成分布和数据分布的最大似然呢，这是个问题

- 数学背景
	- Jacobian 雅可比
		- Jacobian Matrix 就是一阶矩阵导数
		- [图示](../../Slides/Lecture6/FLOW.pdf#page=7)
	- Determinant 行列式
		- 方阵的行列式是一个提供矩阵信息的标量
		- [图示](../../Slides/Lecture6/FLOW.pdf#page=8)
		- 行列式可以看成矩阵所包含空间的容量
		- [图示](../../Slides/Lecture6/FLOW.pdf#page=9)
	- Change of Variable Theorem 变量替换定理
		- 原始随机变量出自一个分布，该随机变量经过 Operator 后形成另一个计算后分布，需要探究 计算后分布和原始分布的关系
		- [图示](../../Slides/Lecture6/FLOW.pdf#page=12-14)

- 回到 计算 生成分布和数据分布的最大似然 的问题
	- 可以依据 变量替换定理 得到 生成数据的分布，然后就可以计算与目标数据分布的最大似然了
	- 但是发现，需要 Generator 的雅可比矩阵的行列式 以及 Generator 的反函数，因此为了让 Generator的上述值方便求取，输入数据和输出数据往往 shape 一致，且 Generator设计一般相对有限
	-  [图示](../../Slides/Lecture6/FLOW.pdf#page=16)

- 由于 Generator 设计相对有限，就需要很多个 Generator 组成 Flow，于是得名 Flow-Based Generator
	- [图示](../../Slides/Lecture6/FLOW.pdf#page=17)

- 再次审视 目标函数
	- [图示](../../Slides/Lecture6/FLOW.pdf#page=18)

- Coupling Layer
	- 将前 d 维 直接 copy 过去，之后的维度 与 前 d 维经过F和H计算(多复杂都可以)的结果 ($\beta$ 和 $\gamma$)，组合计算 $z*\beta + \gamma$ 得到最后输出
	- [图示](../../Slides/Lecture6/FLOW.pdf#page=19)
	- Flow-based Generator Model 中的特殊层，在可以增加 Generator 能力的同时，让反函数和 Jacobian 的计算非常容易
	- [反函数图示](../../Slides/Lecture6/FLOW.pdf#page=20)
	- [Jacobian图示](../../Slides/Lecture6/FLOW.pdf#page=21)
	- 堆叠的时候可以使用些小技巧比如循环颠倒copy
	- [循环颠倒copy图示](../../Slides/Lecture6/FLOW.pdf#page=22)
		- 上面一直不变地copy则前 d 维一直 copy 不好
		- 下面采用 循环颠倒copy 更好
	- [图像分成两部分的方法](../../Slides/Lecture6/FLOW.pdf#page=22)
		- 左侧是 奇偶patch法
		- 右侧是 奇偶channel法等


- 1 x 1 Convolution
	- [图示](../../Slides/Lecture6/FLOW.pdf#page=24)
	- 输入是所有channel同一pixel的tensor
	- 输出是经过计算后所有channels对应pixel的tensor
	- 1 x 1 Convolution可以shuffle channel
	- 1 x 1 Convolution 的反函数和Jacobian det 比较好算
	- [Jacobian det 计算](../../Slides/Lecture6/FLOW.pdf#page=25-26)


- flow-based GM的典型
	- GLOW 
		- 运用了 Coupling Layer + 1 x 1 convolution
		- [OpenAI GLOW的Demo](../../Slides/Lecture6/FLOW.pdf#page=27)
	- Parallel WaveNet
	- WaveGlow


