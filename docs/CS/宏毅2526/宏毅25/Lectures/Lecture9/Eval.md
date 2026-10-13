***Once a metric becomes a target, it is no longer a good metric.***


- 利用各种开源的 Benchmark 去评估模型的跑分，跑分越高，我们就认为模型能力越强

- 质疑点
	- 背答案 [论文](https://arxiv.org/abs/2402.08939)
		- 将 GSM8k 中与答案无关的信息替换掉，或者是新增一些无意义的句子，或者调换某些句子的顺序且不影响结果，利用改造后的Benchmark检查模型能力，看与原始Benchmark跑分相比是否下降

- 更有效的Benchmarks --- 往往也会被 hacking 掉
	- [ARC-AGI](https://arxiv.org/abs/1911.01547)
		- 利用 Few-shot 定义任务，让模型去完成任务，这些任务会很新，能避免模型训练过
		- 这些任务往往像 数独、数字游戏等等

	- [Chatbot Arena](https://lmarena.ai/)
		- 用两个匿名模型回答人类的同一个问题，然后人类选择更喜欢的答案，这样网站会给更喜欢的答案正分，另一个答案负分，进而统计出所有模型的分数进行排名
		- [Elo Score12-13](../../Slides/Lecture9/reason_eval.pdf#page=12)

- Goodhart's Law
	- 当一项指标一旦变为目标，它就不再是一个好的指标
