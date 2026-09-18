
每种架构的存在都有其意义
- CNN --- 根据image的特性减少不必要的参数
- Residual --- 使得梯度更平缓
- RNN、Attention --- 建立序列内的相关性

序列架构
- [RNN-style12-13](../../Slides/Lecture4/mamba.pdf#page=12-13)
	- 优势：上下文的增长并不会带来单点计算/存储开销上涨(fixed固定大小H)
	- 劣势：需要按序列顺序计算
- [Attention-style](../../Slides/Lecture4/mamba.pdf#page=17)
	- 优势：能够并行处理整个序列的计算
	- 劣势：上下文增长会显著增大计算/存储开销

随着 对超长序列的追求(Agent/RAG) 比如 1M等，Attention 的劣势逐渐显现，对RNN的呼喊又回来了

RNN 并行化
- [原始RNN](../../Slides/Lecture4/mamba.pdf#page=33)
	- 一连串的 function 难以并行
- [RNN_v1](../../Slides/Lecture4/mamba.pdf#page=34)
	- hidden state 不处理
- [RNN_v2](../../Slides/Lecture4/mamba.pdf#page=35)
	- 将 $f_{B,t}(x_t)$ 表示成 $D_t$
	- 将 $f_{C,t}(H_t)$ 表示为 $H_{t}q_t$
	- $q_t=W_Q \cdot x_t$
- [RNN_v3](../../Slides/Lecture4/mamba.pdf#page=36)
	- 将 $D_t$ 改造成 $v_t \cdot k_t^T$
	- $v_t=W_v\cdot x_t$ 与 $k_t=W_k\cdot x_t$
	- 变成 [Linear Attention](../../Slides/Lecture4/mamba.pdf#page=41) 相比Attention
		- 没有attention score 的 softmax
		- 先计算kv再考虑 q
- $RNN \sim Linear Attention$ 而 Linear Attention 就是少了 softmax 的 Attention
	- [图示](../../Slides/Lecture4/mamba.pdf#page=39)

Linear Attention 无法战胜 Attention
- 可能原因：RNN的存储能力不如Attention
	- 如果RNN的hidden state 是 $d * d_{model}$，那么最多存 d 个不干扰的 $d_{model}$ 信息
	- 但是 Attention 也存在同样的问题，比如 序列的长度 t 超过了 v 的维度 $d$，那么它也只能表征 $d$ 个维度 的互不干扰的信息
	- *似乎这个并不是主要的原因啊*
- 更可能的原因 - 少了 Reflection 记忆永不变
	- Linear Attention 没有 Reflection 的过程
	- Attention 中 softmax 相当于做了 Reflection 
	- 那么 让 Linear Attention 加上 Reflection 的过程 -> [Retention Network49-50](../../Slides/Lecture4/mamba.pdf#page=49-50) -> [Gated Retention51-52](../../Slides/Lecture4/mamba.pdf#page=51-52) -> [更复杂的Reflection](../../Slides/Lecture4/mamba.pdf#page=53)
		- [相关的工作图示](../../Slides/Lecture4/mamba.pdf#page=55) 已经汗牛充栋了
			- 图示同时记录了它们的公式方便查看
		- 其中 Mamba 能力很强，似乎真能打赢Transformer
			- Mamba 公式挺复杂的，但是 Mamba-2 反而是简单的类似 Gated Retention的公式



DeltaNet
- DeltaNet 也是上述的Linear Attention的一种形式，非常有趣
	- DeltaNet 公式 - $H_t = H_{t-1}(I-\beta_t k_t k_t^T) + \beta_t v_t k_t^T$
	- DeltaNet 的思维
		- 原始Linear Attention $H_t = H_{t-1} + v_t k_t^T$
		- 想去除部分过去的值再把新的值放进去
			- $H_t = H_{t-1}-\beta v_{t,old}k_t^T+ \beta v_t k_t^T$
			- $v_{t,old}=H_{t-1}k_t$
			- 整理得到 $H_t = H_{t-1}-\beta_k (H_{t-1}k_t - v_t )k_t^T$ -> 很像*Gradient Descent*
- Titans 其实就是继承了 DeltaNet 这种 Gradient Descent 的思想 https://arxiv.org/abs/2501.00663
	- Titans 将 $H_t$ 直接变成可训练神经网络并引入了 Momentum等机制


Linear Attention vs Attention
- Linear Attention 只需要保留固定的 $H$ 存储过去的 kv 信息，不会随着序列的增长，存储预算不会很大
- Attention 就不必说了，序列越长 KV cache 的历史信息就越长

两种思考 Linear Attention 的方式
- 之前22年讲过 Attention 推 [Linear Attention](../../../../宏毅22/Lectures/Lecture5/C1%20各种%20self-attention.md)
- 这里是 RNN 推 Linear Attention
- 我更青睐后者的理解，前者不本质

即使研究基模模型架构也不要从头训练，而是直接套用预训练好的模型参数，再加上你自己的设计
- [不要从头训练](../../Slides/Lecture4/mamba.pdf#page=62)
