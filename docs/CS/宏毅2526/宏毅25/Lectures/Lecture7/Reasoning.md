
***Thinking Twice***

- Reasoning 基础
	- Prompt -> LLM -> \<think>Reasoning\</think>  Answer
	- Reasoning 的主要组成
		- Verification --- check answer
		- Explore --- try different approach
		- Planning --- first, ..., second, ...
	- Reasoning 属于 Test-Time Compute 的一种



- 四种 获得Reasoning 的方法
	- 不微调
		- CoT
		- 给模型推理工作流程
	- 微调
		- Imitation Learning 教模型推理过程
		- RL


- CoT
	- Few-shot CoT
	- Zero-shot CoT
		- `Let's think step by step`
	- Supervised CoT
		- [详细说明CoT的流程](../../Slides/Lecture7/reasoning.pdf#page=9)



- 给模型推理工作流
	- 尽可能多的 Explore (Pass@k)
		- [Large Language Monkeys](https://arxiv.org/abs/2407.21787)
	- 如何选出正确答案
		- [Majority Vote(self-consistency)](https://arxiv.org/abs/2203.11171)
			- 多数投票，同一问题生成多个候选答案，最终选择 多数一致的答案
		- [Confidence(CoT decoding)](https://arxiv.org/abs/2402.10200)
			- 定义了 置信度 $\Delta_{k,answer}=\frac{\sum p(x_t^1|x_{<t}) - p(x_t^2|x_{<t})}{|answer|}$
				- 即 用答案范围内位置 的 top-1 token 的概率 - top-2 token 的概率 并进行长度的 normalization 作为置信度，由此可见置信度越高，该步骤越被模型认可
			- CoT decoding 就是选用置信度最高的top-10个路径
		- Best-of-N
			- 加上 Verification 给每个答案一个分数，选择分数最高的那个
	- Sequential Re-thinking
		- 不断反思改进之前的答案
		- [Parallel + Sequential](https://arxiv.org/abs/2408.03314)
	- Process Verifier
		- 过程验证器，可以给每个步骤分数，尽而选择合适的步骤继续往下
		- Process Verifier 的训练
			- 原始训练资料 prompt+answer
			- 经过模型自己 蒙特卡洛搜索 获得每个步骤的分数，比如从某一步开始进行 蒙特卡洛搜索，共进行5次，对了3次，那么这个步骤的分数就是 (3/5)
			- 如此得到 蒙特卡洛后的 训练资料用来训练 Process Verifier
			- [Math-Shepherd](https://arxiv.org/abs/2312.08935)
	- [Beam-Search](https://arxiv.org/abs/2401.17686)

	- Heuristic Search Algorithm 启发式搜索算法用于模型 Decoding
		- [各种相关研究](../../Slides/Lecture7/reasoning.pdf#page=29)



- Imitation Learning 模仿学习
	- 利用 包含 reasoning 的训练资料
	- 可以用 模型 生成 包含 reasoning 的训练资料
		- Improve1 --- 用Verifier 来决定哪些训练资料是正确的
		- Improve2 --- 用 Process Verifier 更细粒度地生成 包含 reasoning 的训练资料
	- 当然，上述得到 训练资料后采用 SFT-style 进行训练，也可以依靠 Verifier 采用 RL-Style 进行训练
	- 但是 Reasoning 中不应该让所有 Step 都是对的，它应该学会 在 错误 Step 后反思回 正确答案
		- [Steam of search](https://arxiv.org/abs/2404.03683)
		- 相关的 [训练数据生成过程](../../Slides/Lecture7/reasoning.pdf#page=38)
	- 直接 Knowledge Distillation 会 Reasoning 的 Model


- Reinforcement Learning 强化学习
	- input -> LLM -> Reasoning + Answer
		- Answer 对 则 RL 是正反馈
		- Answer 错 则 RL 是负反馈
	- RL 甚至不关注 Reasoning 的过程
		- Aha Moment --- RL 自行得到的 Reasoning 反思过程，但是似乎原模型本身就有类似Aha这样的过程



- [DeepSeekR1训练过程47-48](../../Slides/Lecture7/reasoning.pdf#page=47)
	- 综合运用了上述的几乎所有方法
	- Foundation Model 很重要，似乎 RL 无法大幅强化较小模型的能力，这是因为 RL 只是 唤醒模型原有的能力罢了






