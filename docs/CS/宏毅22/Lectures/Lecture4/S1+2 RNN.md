- [1](../../Slides/Lecture4/RNNS1+2.pdf)

- [RNN的概念图](../../Slides/Lecture4/RNNS1+2.pdf#page=12-13)
	- 同样的颜色表示同样的weight
	- 一个例子程序如下
	```python
	hidden_state = relu(xs[0] @ self.w1 + self.b1)
	
	for i in range(1, len(xs)):
		hidden_state = relu(xs[i] @ self.w1 + self.b1 + hidden_state @ self.w2 + self.b2)
	
	result = hidden_state @ self.w3 + self.b3
	
	return result
	```

- 当然可以[Deep RNN](../../Slides/Lecture4/RNNS1+2.pdf#page=14)

- [Elman Network & Jordan Network](../../Slides/Lecture4/RNNS1+2.pdf#page=15)
	- 前者存hidden
	- 后者存output

- [Bidirectional RNN](../../Slides/Lecture4/RNNS1+2.pdf#page=16)
	- 双向RNN 就是两个相反方向的搭起来

- [LSTM](../../Slides/Lecture4/RNNS1+2.pdf#page=17) 
	- 典型的RNN已经是RNN的代名词了
	- [LSTM的单体结构](../../Slides/Lecture4/RNNS1+2.pdf#page=18)
		- a 就是LSTM的 hidden state
		- LSTM 不仅会输出 a 还会输出 c' ，作为下一个时间步的memory
	- [LSTM的例子](../../Slides/Lecture4/RNNS1+2.pdf#page=20-26)
	- 把LSTM整体想象成一个neuron，但是会有四倍参数
	- [LSTM的网络结构](../../Slides/Lecture4/RNNS1+2.pdf#page=30-31)
		- 第30页图很清晰展现了LSTM
	- [LSTM的learning Target](../../Slides/Lecture4/RNNS1+2.pdf#page=32-33)

- [RNN是很难训的](../../Slides/Lecture4/RNNS1+2.pdf#page=34-35)
	- RNN的梯度变化非常陡峭
	- gradient clipping --- 规定gradient的上下限，能够缓解RNN的训练问题

- [预估gradient的大小方法](../../Slides/Lecture4/RNNS1+2.pdf#page=36)
	- 把参数大小稍微改变一点，看看损失结果，如果损失变化大则gradient大，反之亦反

- LSTM 能够解决RNN的部分问题
	- 能够解决 gradient 消失 的问题但是不能解决 gradient 爆发 --- “RNN memory 被覆盖”是对其更新机制的形象描述，并不是说上一时刻的信息必然完全消失，而是它没有 LSTM 那种**可控的线性记忆通道**。

- [RNN的应用](../../Slides/Lecture4/RNNS1+2.pdf#page=40-50)
	- Auto-Encoder 概念在1980年代就已经提出
		- Encoder 把输入压缩成隐表示 z
		- Decoder 根据 z 重建原始输入
		- 训练目标通常是让 $\hat{x}\approx x$
	-  Encoder-Decoder 结构不一定是Auto-Encoder，主要看训练目标是否是重建输入，比如在机器翻译中的Encoder-Decoder结构可能就是 输入是语言1，输出是语言2，类似于端到端训练，这不属于Auto-Encoder




