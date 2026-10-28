***Accumulate and then awaken rather than force***


- PreTrain&Alignment
	- PreTrain
		- 通常就是 Language Modeling，需要非常大量的资料(TB-PB级别)
	- Alignment
		- Alignment可以理解为 finetune 通常包含 SFT 和 RLHF/VR，需要的资料不必很多(less is enough)，但是(回答)质量需要较高(quality is all)，资料量往往 10k-100k 之间 [LLaMA2的经验](https://arxiv.org/abs/2307.09288)
		- SFT 后模型的能力就已经很强了，RL 对模型能力的提升不再明显
	- Alignment 后会使得模型的可用性大大提升


- Knowledge Distillation
	- Knowledge Distillation 是 Alignment 做法中的关键手段之一
	- 方法细节
		1. 选取一系列问题
		2. 由较强的模型生成问题的答案
		3. 收集上述的 问题-答案 对
		4. 利用 问题-答案对 采用 SFT 方法 训练目标模型
	- 方法变体
		- 用其他专用模型对 生成的 问题-答案 对 进行质量筛选，只保留质量高的数据
		- 有些时候不需要较强的模型生成 问题的答案，只需要收集网络上的句子，然后截取前半句，由强模型生成后半句，用这样的信息进行 SFT依然能够提升能力


- Alignment 分析
	- Alignment 后模型的前后行为其实变化不大，只有少量token有偏差，这些token往往是连接词和停止符(`</s>`)
		- Instruction Following without Instruction Tuning 论文依据上述的发现，只提高或者降低部分token的产生概率，也发现有类似于 SFT 的功效，但是效果并没有SFT好(有效但不多)
	- [Self-Rewarding Language Models](https://arxiv.org/abs/2401.10020)
		- 自己打分自己的回答，然后采用 DPO 训练


- PreTrain 分析
	- [Physics of Language Models: Part 3.1](https://arxiv.org/abs/2309.14316)
		- 研究语言模型可解释性的系列工作，PreTrain 使其 3.1 部分
	- Data Augmentation 数据增强在PreTrain中依然有效且重要
		- 某条咨询如果只出现一次会使得模型在预训练时造成误解，因为它只知道接龙嘛，比如 a 属于 A是 c，它只会学到 `a + 属于 A -> c`，而不会学到 `a -> c`，因此我们需要对这条咨询进行数据增强，对它多次同义改写，并用这些咨询 PreTrain 模型，使得它意识到 `a->c`
		- [例子31-35](../../Slides/Lecture5/pretrain.pdf#page=31)
	- 大资料是很重要的
		- 可以在 [FineWeb](https://huggingface.co/HuggingFaceFW) 获得大量数据
	- 网络资料的过滤
		- [图示](../../Slides/Lecture5/pretrain.pdf#page=45)
		- 过滤方法
			1. Rephrase Model 依据网络资料，生成同语义版本的合成数据集
			2. 将合成资料数据集和原始网络资料一起用作 PreTrain
		- 在有限算力、固定模型下应该尽量看不同的咨询
		- [一些数据清洗的文章](../../Slides/Lecture5/pretrain.pdf#page=47)
			- [FineWeb](https://arxiv.org/abs/2406.17557)值得注意，它就是生成 Huggingface FineWeb 数据的论文，数据清洗的pipeline说明的非常详细分析也很到位


- Alignment 的极限
	- [The False Promise of Imitating Proprietary LLMs](https://arxiv.org/abs/2305.15717)
		- 指出 Alignment 只是看起来强了，实则效果可能并没有很好
	- [Alignment四类数据](../../Slides/Lecture5/pretrain.pdf#page=50)
		- HighlyKnown - MaybeKnown - Weakly Known - Unknown
		- [论文](https://arxiv.org/abs/2405.05904) 发现 Alignment 学习 Unknown 知识的时候反而容易使得模型性能变差，Alignment 学 MaybeKnown 的知识会得到最好的性能提升，而 HighlyKnown 仅次之
	- [Alignment三类数据](../../Slides/Lecture5/pretrain.pdf#page=53)
		- LLM 会的问题 + LLM 答案 - LLM不会问题 + 正确答案 - LLM不会问题 + LLM自己的错误答案
		- [论文](https://arxiv.org/abs/2402.18243) 发现 case 2 反而最差，case1 和 case 3 都比较好
		- 基于上述研究，RL 其实是好的方法，它没有强迫模型去学习新东西，而是让它偏向自我的哪个部分


- PreTrain 的后遗症
	- [Embers of Autoregression](https://arxiv.org/abs/2309.13638)
		- 该论文发现，让 GPT-4 做 Rot-13 解码可以很快做出来，而做 Rot-8 解码却会出错(就是做凯撒密码的解密问题Rot-13就是移动13)
		- 似乎是因为 Rot-13 的文章最多，但是 Rot-1和Rot-3 又很好不太清楚具体是什么原因



