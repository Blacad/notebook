

- [Anomaly Detection的解决方案](../../Slides/Lecture8/Detection%28v9%29P3-9.pdf#page=8)
	- 依据三种情形分为两大类方法
		- With Labels
		- Without Labels
		- 这里的 Label 不是指 正常和异常的 Label，而是真实的类 Label
		- 当然也可以从 正常和异常的二分类来看，但是往往异常数据是不够的(远少于正常数据)，因此二分类基本做不了异常检测

- With Labels 使用 分类器
	- 分类器用于异常检测的[基本框架](../../Slides/Lecture8/Detection%28v9%29P3-9.pdf#page=12-13)
	- 分类器的[训练](../../Slides/Lecture8/Detection%28v9%29P3-9.pdf#page=17)
		- 还需要训练阈值
	- 分类器的[评估](../../Slides/Lecture8/Detection%28v9%29P3-9.pdf#page=21)
		- 用单纯的准确率是不行的，正常和异常数据如果不平均，那么准确率高并不能说明什么问题，需要同时考察 召回率
		- 一般两个表 --- Detect and Not Detect 表 + Cost 表


- Without Labels
	- [问题定义](../../Slides/Lecture8/Detection%28v9%29P3-9.pdf#page=27)
	- [异常检测器](../../Slides/Lecture8/Detection%28v9%29P3-9.pdf#page=28)
		- 比如用 Auto-Encoder，重建损失的大小可用于评估

