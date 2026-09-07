
- 假设序列长度为N，self-attention的计算量与 $N^2$ 正比，当 N 很大时，self-attention的计算会在整个计算中占主导地位，因此我们需要减少self-attention的计算量以提高模型整体的运算速度

- self-attention中需要得到 attention matrix 这是主要的计算过程，因此==减少得到 attention matrix 的计算量== 可以提速推理
	- [Local Attention](../../Slides/Lecture5/C1xformer%20%28v8%29.pdf#page=7) --- 只算部分attention matrix 而不是全局的attention matrix，比如某个位置只算它前一个和后一个的attention score，这样构成的 attention matrix 计算量会小非常多，但是这样就和CNN差不多了
	- [Stride Attention](../../Slides/Lecture5/C1xformer%20%28v8%29.pdf#page=8) --- 延续上面的思路，我设置一个stride，每隔该stride计算对应的attention score，最终构成 attention matrix
	- [Global Attention](../../Slides/Lecture5/C1xformer%20%28v8%29.pdf#page=9) --- 在 Sequence 中插入 special token，special token 会和所有 token计算 attention score，而普通token只会和special token算attention score

- 那么怎么用呢，现在都是 多头自注意力机制，大可以不同的头是不同的attention方式
	- Longformer --- Local + Stride + Global
	- Big Bird --- Random Attention(随机选位置算attention score) +  Local + Stride + Global


- 另一个洞见是让 attention score 大的保留下来，小的直接set 0，也可以极大程度简化 attention matrix，那么如何知晓 attention score 的大小呢 --- [Clustering](../../Slides/Lecture5/C1xformer%20%28v8%29.pdf#page=14)
	- Clustering 聚类 即 将得到的 q 和 k 进行聚类，这样在同一类的 q k 往往是较大的 attention score 而不是同一类的往往较小因此set 0，这样我们只需要计算同一类token的attention score


- 由另一个学习模块决定哪些地方需要计算 attention score --- [Sinkhorn Sorting Network](../../Slides/Lecture5/C1xformer%20%28v8%29.pdf#page=15)


- [Linformer](../../Slides/Lecture5/C1xformer%20%28v8%29.pdf#page=16-18)
	- 完整的 attention matrix 中有很多列都是重复的或者可以由其他列线性组合得到，因此整个 attention matrix 是 low rank 的，因此可以尝试将完整的 attention matrix 压缩为更 low rank 的形式
	- 具体做法，挑选 K 个最具代表性的 value 以及 K 个 最具代表性的 key，然后做正常的 self-attention
	- 选择最具代表性的 key 和 value 可以使用 CNN 选择

- Attention 机制本质是三个矩阵相乘 ---- Linear Attention
	- 在矩阵相乘中[减少计算量](../../Slides/Lecture5/C1xformer%20%28v8%29.pdf#page=25)，虽然矩阵计算结果相同，但是计算顺序不同所需的计算量是不同的
		- 矩阵相乘计算量 ---  (m n) 与 (n k) 相乘所需计算量是 (m n k)
	- 常规计算中，先算 q k 然后进行 softmax，最后算 v，因此在调转运算顺序时(k,q first -> v,k first)也需要引入[新的计算方式](../../Slides/Lecture5/C1xformer%20%28v8%29.pdf#page=28-37)
		- [图示](../../Slides/Lecture5/C1xformer%20%28v8%29.pdf#page=31) 讲的比较明白，$\phi(k)$ 的各个维对 v 加权依次得到分子上各列的值(注意 $k_1^j$ 是标量，$v^j$ 是向量)

- Synthesizer 合成器 --- 干嘛要 q k，我只产生 v 同时把 attention matrix 视作参数

- 总结各种[self-attention](../../Slides/Lecture5/C1xformer%20%28v8%29.pdf#page=40)

