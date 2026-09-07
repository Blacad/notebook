
- Life long learning(lll)
	- [lll的基本思想](../../Slides/Lecture14/life_v2.pdf#page=4)
	- lll 的重大难题 --- [灾难性遗忘](../../Slides/Lecture14/life_v2.pdf#page=6-9)
		- 在学完 task 2 后，它会遗忘 task 1
		- 但是它 同时学多种任务(multi-task training)，它表现的效果会比 串行学多种任务的平均效果好
		- 因此，常常会把 multi-task training 视作 lll 的 upper bound


- [LLL vs Transfer](../../Slides/Lecture14/life_v2.pdf#page=13)
	- Transfer Learning 迁移学习，它的做法类似于 LLL，但是它只是想用 前面学习的知识让学习新知识表现更好，它不关注之前的任务是否还能做好，而只关注新任务做的效果
	- LLL 则是需要同时关注所有任务的效果，让新任务表现好的同时，旧任务尽量不遗忘

- Evaluation of LLL
	- 因为 LLL 的性能不只是当前任务的效果，它还需要考虑之前的多种任务的效果，因此它的评估也会有特色
	- [LLL 的 任务序列](../../Slides/Lecture14/life_v2.pdf#page=14)
		- 第一行是对 Task 1 的数字图片进行不同的规则处理得到后续 Task
		- 第二行更粗暴，就是把数字识别任务拆开
	- [评估方式](../../Slides/Lecture14/life_v2.pdf#page=15-16)
		- 每学完一个 Task 就在每个任务上测一次准确率


- ==三个LLL的研究方向==
	- Selective Synaptic Plasticity 选择性突触可塑性 --- Regularization-based Approach
	- Additional Neural Resource Allocation
	- Memory Reply


- 可视化讨论 [catastrophic Forgetting](../../Slides/Lecture14/life_v2.pdf#page=18)


- Selective Synaptic Plasticity
	- [基本思想](../../Slides/Lecture14/life_v2.pdf#page=19)
		- 模型中的一些参数对先前任务很重要，只更改不重要的参数
		- 引入 guard 参数，评估参数对先前任务的重要性
			- 可惜的是 guard 似乎往往是人为设置的而不是可训练的
	- [诸多工作](../../Slides/Lecture14/life_v2.pdf#page=24)
		- 这些工作的一个关键点就是 guard 如何更好确定
		- 而且任务的顺序也会影响结果，因此上述工作会在多种顺序上做，然后取均值
	- 一个基于 gradient 的做法 [GEM](../../Slides/Lecture14/life_v2.pdf#page=25)


- Additional Neural Resource Allocation
	- [Progressive NN](../../Slides/Lecture14/life_v2.pdf#page=27)
		- 渐进式神经网络
		- 每个任务由一部分网络去做，不断增添网络
		- 小 -> 大
	- [PackNet](../../Slides/Lecture14/life_v2.pdf#page=28)


- Memory Reply
	- 使用生成模型生成先前任务的伪数据，然后用之前的 伪数据和新任务数据一起训

- 本节只讲了 Life long Learning的小部分
	- 了解更多 [持续学习综述](../../Slides/Lecture14/life_v2.pdf#page=31)


