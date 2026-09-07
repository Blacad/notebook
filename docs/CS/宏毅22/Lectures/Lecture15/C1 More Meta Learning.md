
- Meta Learning vs self-supervised Learning
	- MAML 与 BERT
		- 两者并不互斥可以[协作](../../Slides/Lecture15/More%20Meta_v6.pdf#page=7)



- Meta Learning vs Knowledge Distillation
	- [Learn to Teach](../../Slides/Lecture15/More%20Meta_v6.pdf#page=14)
		- 比如 learn softmax 的 temperature


- meta learning vs domain adaptation
	- [协作](../../Slides/Lecture15/More%20Meta_v6.pdf#page=18)
		- 让 source domains 的部分扮演 target domain，然后 用 meta learning 学到好的超参数调节方法，然后用 学到的调节方法在所有 source domains 上训练模型，期望 模型在 target domain 上表现好


- Meta learning vs Life-long Learning
	- Mitigating Catastrophic Forgetting 减轻灾难性遗忘
		- 比如用 meta learning 去学习guard等[协作](../../Slides/Lecture15/More%20Meta_v6.pdf#page=27)