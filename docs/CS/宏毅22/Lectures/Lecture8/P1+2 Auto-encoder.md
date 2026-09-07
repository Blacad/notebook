
- [Auto-encoder 架构](../../Slides/Lecture8/auto_v8P1+2.pdf#page=4)
	- 输入 x -> encoder -> vector -> decoder -> 输出 x‘
	- 训练目标： x 与 x’ 尽可能相近，又称为重建损失
	- 下游应用
		- Encoder 的本质是将 高纬度信息 转成 低纬度信息，即降维
	- Encoder 寻找高纬中的pattern然后压缩为低纬，Decoder根据低纬利用pattern还原高纬
	- 特殊用法 [De-noising Auto-encoder](../../Slides/Lecture8/auto_v8P1+2.pdf#page=10)


- Feature Disentangle 特征理清
	- 将 Encoder 的低维向量中的各部分代表的特征理清，[如图](../../Slides/Lecture8/auto_v8P1+2.pdf#page=14)
	- 应用比如 [变声器](../../Slides/Lecture8/auto_v8P1+2.pdf#page=18)

- Discrete Latent Representation 离散潜在表示
	- 依据特征理清的思路，有[离散特征表示](../../Slides/Lecture8/auto_v8P1+2.pdf#page=22)
	- 最著名的思路 [VQVAE](../../Slides/Lecture8/auto_v8P1+2.pdf#page=23)
		- 限制 Decoder的输入一定来自 Codebook

- Auto-Encoder 有非常多的应用
	- 压缩
		- 有趣的洞见：缩写。Encoder 不压缩为 latent，而直接压缩为文字
	- 生成
	- Anomaly Detection 异常检测
		- 为什么不用分类器？
			- 异常检测中往往异常数据少，而正常数据多，因此常规分类器很难训练直接分开 异常和正常
		- 使用 Auto-Encoder 可以做到只用正常训练数据，做到异常检测
			- 比如 用大量真人人脸训练 Auto-Encoder，这样在 测试时就看重建损失是否过大，如果重建损失过大则证明异常