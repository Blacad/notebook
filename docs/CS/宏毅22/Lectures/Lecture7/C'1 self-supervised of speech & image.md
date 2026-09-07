
- Speech 和 Image 的自监督学习过程和微调与 Text 一致，只是下游任务相对 Text 来讲可能不太一样

- Speech 的任务
	- SUPERB 与 SUPERB-SG 是 宏毅团队自己做的有关语音的Bench工作

- Image 的任务
	- 图像识别
	- 目标检测
	- 语义分割
	- 视觉导航


- 五种自监督学习的常见方法

- Generative Approach 生成式方法
	- BERT 和 GPT 的方法完全套在 Speech 和 Image 中使用
	- BERT series --- Mockingjay(Speech)
		- 当然还是需要注意语音的特性，进行细节上的调整
			- 比如相近的语音向量之间的信息非常相近，因此mask可能需要mask连续的多个向量，这样模型的能力才能学得够强
			- 可以尝试 mask 语音向量的某些维度
	- GPT series --- APC(Speech)
		- 针对语音特性，应该预测更远的未来语音向量才有挑战性
	- [image-gpt & image-bert](../../Slides/Lecture7/SSL_speech_image%28v9%29C'1.pdf#page=13)

- Predictive Approach 预测方法
	- Image
		- Predicting Rotation --- 预测图片旋转角度
		- Context Prediction --- 将整个图片截出两小部分，问模型两个小部分的方位关系
	- Speech
		- Context Prediction --- 将整个音频中的两小部分问模型，两个小部分之间的距离
	- [Predict Simplified Objects](../../Slides/Lecture7/SSL_speech_image%28v9%29C'1.pdf#page=17)

- Contrastive Learning 对比学习
	- [对比学习概念](../../Slides/Lecture7/SSL_speech_image%28v9%29C'1.pdf#page=19)
		- 同类的输出越近越好，不同类的输出越远越好
	- SimCLR(Image)
		- 根据同张的不同 数据增强 方法(原始照片)得到同类的图片，这样就不需要label了
		- random cropping 是表现较好的数据增强方法
	- MoCo(Image)

	- [CPC & wav2vec(Speech)](../../Slides/Lecture7/SSL_speech_image%28v9%29C'1.pdf#page=22)
		- Predictor --- CPC 中使用 GRU，wav2vec 中使用 CNN
		- 近邻是 positive 不近邻是 negative，如图示，前4个的结果和后面紧邻的两个应该是positive尽量训练到相近，而与更远的两个不相邻锦鲤那个训练到相斥

	- Classification & Contrastive
		- 两者很像

	- negative example(不同类) 是很难选的，你很难把握阈值，为什么需要 negative，因为没有负样本，模型可能坍缩输出恒定，这样不管你输入什么都靠的很近，但是这明显是错的

- Bootstrapping Approach 自举方法
	- 自举方法可以在没有负样本的时候不让模型坍缩，它的做法[如右侧所示](../../Slides/Lecture7/SSL_speech_image%28v9%29C'1.pdf#page=33)
	- 关键
		- 左右结构不同
		- 只训练一边再copy过去


- Simply Extra Regularization 简单额外正则化
	- VICReg 可以在没有负样本的时候不让模型坍缩，它的做法[如右侧所示](../../Slides/Lecture7/SSL_speech_image%28v9%29C'1.pdf#page=37)
	- 关键
		- Variance --- 利用批次化技术，可以得到一批图片的一批结果vector，保证批次中的结果vectors的各个维度的 variance 足够大(大于某个阈值)