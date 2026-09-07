
- 我们相信，在预训练之后，PLM(pre-trained model)学习了一些知识，这些知识编码在其隐藏表示中，可以迁移到下游任务

- The Problems of PLMs
	1. Data scarcity in downstream tasks 数据稀缺
	2. PLM is too big
		1. 不同 fine-tuning 的任务可能都需要一个新模型
		2. 推理会变慢，而且空间占用会很大

- Solution of those problems
	1. 数据稀缺 -> Data-Efficient Fine-tuning -> 使用自然语言提示并添加特定场景的设计
		1. [prompt tuning](../../Slides/Lecture7/C1PLM.pdf#page=37-47)
			- prompt tuning 需要的三元素
				- [prompt template](../../Slides/Lecture7/C1PLM.pdf#page=40) 
					- 将数据点转换为 自然语言提示词
				- [PLM](../../Slides/Lecture7/C1PLM.pdf#page=41)
				- [verbalizer](../../Slides/Lecture7/C1PLM.pdf#page=42)
					- 构建标签和词表的 字典
			- 不断调整 prompt 提示词模板(相当于训练)
		2. [few-shot learning](../../Slides/Lecture7/C1PLM.pdf#page=48-51)
			- LM-BFF 中的核心观点是 prompt + demonstration(很像特权信息)
		3. semi-supervised learning
			- 使用 soft label
		4. zero-shot
	2. PLM Big -> Reduce the parameters
		1. Distillation 蒸馏
		2. Pruning [剪枝](剪枝.md)
		3. shared parameters
		4. Parameter-Efficient Fine-tuning
			1. [Adapter](../../Slides/Lecture7/C1PLM.pdf#page=83)
			2. [LoRA](../../Slides/Lecture7/C1PLM.pdf#page=88-91)
			3. [Prefix-tuning](../../Slides/Lecture7/C1PLM.pdf#page=97-98)
			4. [Soft Prompting](../../Slides/Lecture7/C1PLM.pdf#page=100-102)
		5. Early Exit
			- 在运行到某层后就检测一次置信度是否够了，够了就输出不再推了

- PLM 依然有很多问题，留待后续解决