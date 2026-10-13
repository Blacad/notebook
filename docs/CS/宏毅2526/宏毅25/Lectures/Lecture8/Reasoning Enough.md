***Too much is as bad as too little.***

- Reasoning 太多并不是好事，这往往需要耗费大量的算力资源和时间，有意思的想法是将 Reasoing 的实践分问题难易去做
	- 同时有研究表明 Reasoning 越长最终的结果反而可能不好

- 下面针对 Reasoning 的各种方法 来控制 Reasoning 的长度


- CoT
	- [Chain of Draft](https://arxiv.org/pdf/2502.18600)
		- `Think step by step, but only keep a minimum draft for each thinking step, with 5 words at most.`


- Imitation Learning
	- Teacher Model 生成多个带 Reasoning 的回答，学生只学习最短的那个


- RL
	- 增加长度惩罚
		- 一个问题让模型输出 多组Reasoning + Answer，然后取 平均长度，超出平均长度的 施加长度惩罚
	- 直接控制 模型的推理长度
		- input：question + Reasoning length
		- Reward 定义为 正确率 和 实际Reasoning长度 与 我指定长度的差异


