
- [1](../../Slides/Lecture4/self_v7P1+2.pdf)

- [复杂输入](../../Slides/Lecture4/self_v7P1+2.pdf#page=2-6)
	- 原先的输入是单个向量，复杂输入是一组向量(不同向量的形状可能不同)


- [复杂输入的输出](../../Slides/Lecture4/self_v7P1+2.pdf#page=7-9)
	- sequence labeling
	- sequence2label
	- seq2seq


- sequence Labeling 需要给sequence中的每个向量都输出一个label
	- 需要[考虑上下文](../../Slides/Lecture4/self_v7P1+2.pdf#page=10) --- 否则 前后的 saw 是无法区分的，windows不好选因此可以用self-attention
	- [self-attention概念描述](../../Slides/Lecture4/self_v7P1+2.pdf#page=11-14)
	- [self-attention结构](../../Slides/Lecture4/self_v7P1+2.pdf#page=18)
	- [self-attention矩阵角度](../../Slides/Lecture4/self_v7P1+2.pdf#page=21-25) 
		- 考虑序列单token`1*dk 1*dk 1*dv -> L个 1*1(标量) 1*dv -> L个 1*dv -> 加和L个1*dv即1*dv ` 
		- 第24页的图画的很准确，`A'` 中元素都是标量，`V`  和 `O` 中元素都是向量 `1*dv`
	- [Multi-head self-attention](../../Slides/Lecture4/self_v7P1+2.pdf#page=26-28)
		- 不同头负责不同的相关性，因此不同头的分开算(直接乘对应头的权重不会再乘公共权重slides有点问题)，最后再汇总

- [positional encoding](../../Slides/Lecture4/self_v7P1+2.pdf#page=29)
	- self-attention 本身没有顺序信息，它认为所有向量之间都是一样近的，因此需要我们用 位置编码 positional encoding

- [self-attention 应用](../../Slides/Lecture4/self_v7P1+2.pdf#page=31-34)


- [self-attention vs CNN](../../Slides/Lecture4/self_v7P1+2.pdf#page=35)
	- CNN可以被视作是self-attention的一种类别，self-attention会比CNN更复杂，因此在self-attention的训练中需要更多的数据

- self-attention 已经几乎替代了 RNN

- [self-attention用于Graph](../../Slides/Lecture4/self_v7P1+2.pdf#page=40)
	- 只计算有相关性(有edge连接)的attention score 其余的设置成0