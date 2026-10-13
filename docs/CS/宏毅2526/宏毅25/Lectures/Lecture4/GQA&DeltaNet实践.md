
- GQA(Grouped Query Attention) 顾名思义，就是将 Q 头分为多组，共享 K、V头
	- 比如 16 个 Q 头，K、V头各2个，那么 Q1-Q8 共享 K1、V1，Q9-Q16 共享 K2、V2，前后两组各产生 8 个 attention scores，于是会有 16 个 加权后的 V 产生，将其 Concat 后经过 Wo 即为最终结果
	- KV头往往设置的比较少可以减少 KV cache 的缓存
	- KV头往往设置数量一致，不一致并没有明显收益，一致可以让某些操作方便

- MQA 是 GQA 的特例，它只有各1个K、V头


- DeltaNet
	- 比如 各16个QK头，V头32个，那么 V1-2共享QK1，V3-4共享QK2，依次类推，共产生 32 个更新 hidden state
	- QK头往往设置为一致，是因为它们在语义上有相关性，Q是读，K是写
	- V头设计的更多有利于增强表达性

- 总结
	- GQA 是 按KV分组，一组KV共享多个Q，因此 GQA 的逻辑是多个 Q 共享的静态 KV memory
	- DeltaNet 是按QK分组，一组QK共享多个V，因此 DeltaNet 的逻辑是Q/K 是一套读写地址空间，保持相同 head 划分最自然