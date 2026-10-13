
- Model Editing 的目标
	- 给模型植入一项知识，比如 现任美国总统是特朗普而不是拜登了
	- 与 PostTraining 区别
		- PostTraining 是让模型学习新的技能，比如 reasoning、math、tool use 等等


- Model Editing 的三个评估维度
	- Reliability
		- 目标要达成
	- Generalization
		- 目标换句话说也要达成
	- Locality
		- 其它无关的输入不应该被影响


- Model Editing 的方法
	- 不动参数
		- RAG
		- [IKE](https://arxiv.org/abs/2305.12740)
			- few-shot 告诉模型怎么用新知识，否则模型可能固守己见
	- 改变参数
		- 人类编辑
			- [ROME](https://arxiv.org/abs/2202.05262)
				1. 利用 LLM Explanation 中的知识找出网络中与要编辑知识最相关的部分
				2. 只修改该部分的参数(gradient descent or other methods) 
				- [ROME Method 13-20](../../Slides/Lecture10/edit.pdf#page=13)
		- AI学习如何编辑
			- Meta-Learning --- Hyper-network
			- [编辑模型与待编辑模型](../../Slides/Lecture10/edit.pdf#page=27)
			- [MEND](../../Slides/Lecture10/edit.pdf#page=31)
				- 单token对线性层产生的 gradient 的秩为 1


- [KnowEdit](https://zjunlp.github.io/project/KnowEdit/)