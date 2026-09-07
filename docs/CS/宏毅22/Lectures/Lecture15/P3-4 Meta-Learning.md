
- Meta Learning= Learn to Learn
	- 在这里主要是解决 Hyperparameters 调节的问题

- 三步走 Meta Learning
	- 第一步：[找函数](../../Slides/Lecture15/meta_v3.pdf#page=11)(学什么)
	- 第二步：[定义损失函数](../../Slides/Lecture15/meta_v3.pdf#page=19)
		- 注意 这里 meta-learning 的 loss 计算是基于 测试资料
	- 第三步：[优化](../../Slides/Lecture15/meta_v3.pdf#page=22)
	- [整体框架](../../Slides/Lecture15/meta_v3.pdf#page=23)
	- meta learning 由很多任务来学 超参数调节策略 --- Across-task Testing
		- 相当于 我学习一个超参数调节策略，你给我信息，然后我就输出各种超参数的设计，然后你用我的超参数设计去训练一个模型，然后在测试数据中看该模型表现，需要把测试数据中分成两部分一部分用来训练我的超参数调节策略，一部分用来测试超参数调节的效果(避免 overfitting)
		- 部分超参数的修改可能不用重新训练模型，而有些超参数的修改可能需要重新训练模型
		- 但其实我认为用的最多的似乎就是 grid search

- [ML vs Meta](../../Slides/Lecture15/meta_v3.pdf#page=24-31)
	- 我觉得这张图很好对比 [ML vs Meta](../../Slides/Lecture15/meta_v3.pdf#page=28)


- Meta Learning 的常见对象
	- Learning to initialize(MAML)
	- Optimizer
	- Network Architecture Search(NAS)
	- Data Augmentation
	- Sample Reweighting
		- 不同的样本权重不同
	- Beyond Gradient Descent


- 应用
	- N-ways K-shot
		- N 个 class，每个类有 K 个 example
	- [Omniglot](../../Slides/Lecture15/meta_v3.pdf#page=56)
	- [更多应用](../../Slides/Lecture15/meta_v3.pdf#page=57)