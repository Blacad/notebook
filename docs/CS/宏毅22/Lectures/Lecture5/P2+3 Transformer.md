
- Sequence-to-Sequence(Seq2Seq)
	-  [Seq2Seq](../../Slides/Lecture5/P2+3seq2seq_v9.pdf#page=2) 基础
		- Seq2Seq 主要有两种形式
			- 输入和输出的Seq是同样长的 --- Sequence Labeling(Hw2)
			- 输入与输出的Seq长度不同
		- 本处探讨的主要是 输入与输出的Seq长度不同，即机器自己知道何时该停止输出 (一般就是结束符 \<EOS\>)
	- Seq2Seq 的应用
		- Seq2Seq 的应用非常广泛，除了上述提到的语音识别、机器翻译、语音翻译、文字转语音等，还有 聊天机器人和QA场景，基本[覆盖了NLP的各个方面](../../Slides/Lecture5/P2+3seq2seq_v9.pdf#page=8)
		- 可以做[语法分析](../../Slides/Lecture5/P2+3seq2seq_v9.pdf#page=11)
		- 可以做 [Multi-label Classification](../../Slides/Lecture5/P2+3seq2seq_v9.pdf#page=13)，它与 Multi-class Classification 不同，前者是一个样本属于多个类，而后者是将一个样本分到一个类
		- 可以做[对象识别](../../Slides/Lecture5/P2+3seq2seq_v9.pdf#page=14)
		- 它还可以做更多任务，Seq2Seq Model 可以被看成大乱炖Model，基本啥活儿都能硬解



- Seq2Seq 的模型结构
	- [Encoder-Decoder 架构](../../Slides/Lecture5/P2+3seq2seq_v9.pdf#page=15)
	- 当然，有 Encoder-only(BERT) 和 Decoder-only(GPT) 架构，但是一般说 Seq2Seq 是指 Encoder-Decoder

- Encoder
	- Encoder 的作用就是给它 n 个向量，它输出 n 个向量，[图示](../../Slides/Lecture5/P2+3seq2seq_v9.pdf#page=17)
	- EncoderBlock 的[细节](../../Slides/Lecture5/P2+3seq2seq_v9.pdf#page=19-20) --- 原始版本
	- 更进一步的优化
		- [LayerNorm的位置](../../Slides/Lecture5/P2+3seq2seq_v9.pdf#page=21) 左侧是原始版本，右侧是改进版本，现在普遍用的都是右侧的版本，效果更好 --- PreNorm
		- 同时也有说为什么要用LayerNorm而不是BatchNorm，论文见上 --- LayerNorm
		- [PreNorm & LayerNorm](../../Lectures/Lecture5/PreNorm&LayerNorm.md) 讲解

- Decoder
	- Decoder 实际上有两种架构 ，一种是基于 自回归(Autoregressive)，另一种 不基于自回归
	- 基于 Autoregressive(AT) 的 [Decoder](../../Slides/Lecture5/P2+3seq2seq_v9.pdf#page=24-26) --- 最常见的
		- 自己的输出又变成自己的输入，如此往复，自回归
		- masked self-attention
			- 最常见的就是因果掩码，即 在计算当前位置的结果时，只考虑自己和前面的，而不把后面的计算在内，符合 自回归的特点
	- 不基于Autoregressive([NAT](../../Lectures/Lecture5/S1%20NAT%20Sequences%20Generation.md))的 [Decoder](../../Slides/Lecture5/P2+3seq2seq_v9.pdf#page=36)
		- 一次性输入，然后一次性输出完整的输出

- Encoder-Decoder 连接 --- cross attention
	- [cross attention](../../Slides/Lecture5/P2+3seq2seq_v9.pdf#page=40)
	- Encoder 提供 kv，Decoder 提供 q
	- [cross attention的变体](../../Slides/Lecture5/P2+3seq2seq_v9.pdf#page=43)
		- 原始版本是只用 最后一个EncoderBlock的输出当DecoderBlock的输入
		- 变体引入多种组合


- Training
	- [Training的方式](../../Slides/Lecture5/P2+3seq2seq_v9.pdf#page=45)
	- 最小化 交叉熵
	- 还要补全 完整的特殊标签，比如开始和结束标签等
	- Tips
		- [Copy Mechanism](../../Slides/Lecture5/P2+3seq2seq_v9.pdf#page=48) --- 需要从输入中复制一些词汇作为输出
			- [Pointer Network](S2%20Pointer%20Network.md) 可以做到这一点
		- Guided Attention --- 强迫Attention的分数遵循特定规律
		- Sampling 
			- Beam Search
			- Random
			- 有些任务 Beam Search可能好，而有些任务的 Random 可能好
		- exposure bias --- 在训练时往往全部是正确的，但是在实际推理过程中，可能会推理出错误的结果，导致错误一直累积
			- 可以在训练过程掺入一些错误 --- [Scheduled Sampling](../../Slides/Lecture5/P2+3seq2seq_v9.pdf#page=57)
