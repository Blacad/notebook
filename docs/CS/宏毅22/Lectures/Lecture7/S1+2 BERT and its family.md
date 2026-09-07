
- 以美国动漫《芝麻街》的角色命名 BERT 和 它的家人
	- ELMo(Embeddings from Language Models)
	- BERT(Bidirectional Encoder Representations from Transformers)
	- ERNIE(Enhanced Representation through Knowledge Integration)
	- Grover(Generating aRticles by Only Viewing mEtadata Records)
	- ...


- [Pre-train Model](../../Slides/Lecture7/S1+2BERT%20train%28v8%29.pdf#page=7-13)
	- 在 ELMo 等之前就已经有，它是将每个 token 重表示为 embedding vector
	- 但是前期比较粗糙几乎不考虑上下文信息，比如 Word2vec仅仅考虑基础的上下文信息
	- 发展到后来就是 Contextualized Word Embedding (RNN或者self-attention等)

- [Smaller Model](../../Slides/Lecture7/S1+2BERT%20train%28v8%29.pdf#page=16)
	- BERT中比较work的压缩参数方法 ALBERT(共享参数)

- Network Architecture 网络结构
	- Transformer-XL
	- Reformer
	- Longformer
	- 上述都是处理长上下文信息

- 预训练 Pre-train
	- Predict Next Token 预测下一个Token
		- ELMo
			- 使用[双向 LSTM](../../Slides/Lecture7/S1+2BERT%20train%28v8%29.pdf#page=50), w4用前向和后向的计算值拼接作为真实的预测值
		- GPT/Megatron/Turing NLG
			- 使用 self-attention
			- 在 预测下一个token的情形中使用 self-attention 需要小心，不要让模型看到它要生成的内容，否则可能引起作弊(causal attention)
	
	- BERT 可采用不同 Masking Input策略 进行多种预训练
		- [SpanBERT](../../Slides/Lecture7/S1+2BERT%20train%28v8%29.pdf#page=54-56) 是其中比较出名的
	
	- BERT 不适合给定部分序列，预测下一个Token
	
	- Seq2Seq 模型，比如 [MASS+BART](../../Slides/Lecture7/S1+2BERT%20train%28v8%29.pdf#page=61) ，但是输入需要进行扰乱，否则Decoder部分很容易作弊
		- [MASS的扰乱方法](../../Slides/Lecture7/S1+2BERT%20train%28v8%29.pdf#page=62)
	
	- [UniLM](../../Slides/Lecture7/S1+2BERT%20train%28v8%29.pdf#page=64)
		- 将 BERT、GPT 和 Seq2Seq 进行结合
		- 我私认为 BERT 适合理解和填空任务，GPT适合生成任务，Seq2Seq适合机器翻译等任务

	- [Replace or Not](../../Slides/Lecture7/S1+2BERT%20train%28v8%29.pdf#page=65-66)
		- 判断各 token 是否应该被替换

- Sequence Level 
	- 得到 Sequence 的 整体的 embedding
	- [Skip Thought & Quick Thought](../../Slides/Lecture7/S1+2BERT%20train%28v8%29.pdf#page=69)
		- Quick Thought 绕过自回归生成