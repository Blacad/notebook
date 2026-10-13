***Self-reflection is a good method to solve forgetfulness.***


- PostTraining 基础知识
	- 概念 --- 将 通用模型通过 PostTraining 使之更擅长某种领域知识，在 概念上更像 Continual Learning 持续学习(Life-Long Learning,LLL)
	- Foundation Model -> PostTraining -> Fine-tuned Model
	- PostTraining 可以看作 Alignment，但是 它所讲的范围可以更加广，在Alignment 中，往往就是 Base -> Instruct 的过程，而 PostTraining 不必是基于 Base 做的，它也可以基于 Instruct 模型去做，核心在于使模型在某项能力上更突出
	- 三种类型的训练
		- PreTrain-Style
		- SFT Style
		- RL Style


- PostTraining 会产生灾难性遗忘
	- 不论是那种类型的训练都会产生灾难性遗忘的问题，你希望的领域能力提升了，但是其他领域的能力却可能大幅下降


- 解决灾难性遗忘的问题
	- 发现 模型遗忘的知识在再次训练到相关知识时能被唤醒
	- Experience Replay 重演
		- 先训练 Task 1，然后训练 Task 2 的时候混入部分 Task 1 的资料(大概 5%左右就足够好了)
		- 如果没有之前的训练资料，就让模型自己生成一系列资料，然后混入 Task2 的训练过程
			- 好的实践 [Magpie](../../Slides/Lecture6/post_training.pdf#page=45)
	- Paraphrase 释义
		- 用 Foundation Model 本身改写新训练资料后，再用改写后的新训练资料去训练模型
	- Self-output 自生成
		- 用 Foundation Model 本身去生成新训练资料的答案，评估答案是否正确，如果正确就将 答案和新训练资料 纳入训练，不正确可以退回 Paraphrase 方法
	- [上述方法一览](../../Slides/Lecture6/post_training.pdf#page=48)
	- RL-Style 的方法与Self-output 方法非常像，这可能是 RL 往往不容易导致遗忘的原因

- 一些其它方法
	- [I Learn Better If You Speak My Language](https://arxiv.org/abs/2402.11192)
		- Base：直接用 强语言模型生成问题答案，而不要用人类写的 ground truth
		- Improve：使用 强语言模型在尽量不改变原文的情况下纠正 待训练模型产生的答案，然后用这样的训练数据训练模型





