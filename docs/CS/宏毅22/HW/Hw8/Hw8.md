- 说明
	- 本次作业基于 AutoEncoder 做异常识别任务
	- AutoEncoder 就是先通过 Encoder 降维，然后通过 Decoder 重建
	- AutoEncoder做异常识别的场景
		- 假设我们的训练数据集全部都是正常数据(且没有标签)，而异常数据从没见过，那么 AutoEncoder 的重建损失就会很大，这样就可以识别哪些是异常的
		- 无标签且异常数据很少甚至没有
	- 有标签的数据可以采用分类器做异常识别

- 数据集准备 -> 模型搭建 -> 训练器搭建 -> 推理过程
	- 训练器搭建中涉及 数据读取、优化器/损失函数设计以及学习率规划器等等环节


- 模型搭建
	- 实现了五种 auto-encoder 分别是 fcn、cnn、vae、ensemble 以及 resnet
		- fcn 
			- encoder和decoder都是简单的前馈神经网络(MLP)，直接把图片flatten直接处理
		- cnn 
			- encoder 使用卷积+MLP
			- decoder 使用MLP+反卷积
		- vae --- vae 相关的知识在 [vae](../../Lectures/Lecture6/S4VAE.md)
			- encoder 使用 卷积 + 2个MLP 分别生成 均值 和 方差，然后进行重参数化处理
			- decoder 使用 MLP + 反卷积
			- 由 重参数化的实际代码 可以知道输出的方差实际上是 $log\sigma^2$
			- 重参数化的代码值得学习
		- ensemble
			- 有两个 encoder，fcn 和 cnn 的 encoder，将两者的输出拼接到一起
			- decoder 使用 Linear Projection + 反卷积
		- resnet
			- 构建了 卷积形式的残差连接网络 `Residual_Block`
				- 我们可以看到 残差在这里对原始输入有个下采样的操作，这个主要为了和卷积的输出进行维度对齐
				- 有残差的网络可以画一条竖线当原始输入的线
			- encoder 使用 卷积 + 卷积残差网络 + Linear Projection
			- decoder 使用 Linear Projection + 反卷积

- 数据集准备、训练器搭建以及推理都无甚好说

- [ROC&AUC](../../HW/Hw8/ROC&AUC.md) 评估标准

- [flatten&unflatten&cat](flatten&unflatten&cat.md)