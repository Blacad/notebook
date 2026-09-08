# 李宏毅机器学习 2022

- 2022 Spring 国立台湾大学李宏毅教授
	- 为什么选择 2022 年版 --- 2022年版更偏向广泛的深度学习领域，介绍了非常多经典的深度学习模型，而之后的版本课程更多围绕LLM展开，而不是经典的深度学习领域，因此我们选用 2022年版(21年版也可以但是两版差不多)
	- 李宏毅老师是国立台湾大学的教授，其风趣幽默的授课风格深受大家喜爱，并且尤其喜欢在 PPT 中插入宝可梦等动漫元素，是个非常可爱的老师。
	- 这门课挂着机器学习的牌子，但其课程内容之广实在令人咋舌，其作业一共包含 15 个 lab，分别是 Regression、Classification、CNN、Self-Attention、Transformer、GAN、BERT、Anomaly Detection、Explainable AI、Attack、Adaptation、 RL、Compression、Life-Long Learning 以及 Meta Learning。可谓是包罗万象，能让学生对于深度学习的绝大多数领域都有一定了解，从而可以进一步选择想要深入的方向进行学习。
	- 大家也大可不必担心作业的难度，因为所有作业都会提供助教的示例代码，帮你完成数据处理、模型搭建等，你只需要在其基础上进行适量的修改即可。这也是一个学习别人优质代码的极好机会，大家需要水课程大作业的话，这里也是一个不错的资料来源。


- [课程网站](https://speech.ee.ntu.edu.tw/~hylee/ml/2022-spring.php)
	- 课程视频虽然网站上有，但是 [B站](https://www.bilibili.com/video/BV1K341157jN)上也有可以对照着看(B站的这个地方似乎没有完整的作业视频)，但是这个 [B站](https://www.bilibili.com/video/BV1Wv411h7kN/) 上有完整的作业视频但是其它视频很混乱，所以学习课程用前面那个视频，看作业用后面那个视频
	- 课程Slides在网站上有，我会保存一份在笔记里，并将Slides划归其对应的Lecture中
		- 有些课程Slides并不在这个网站上，可以查看21/20/19版的网站，上面可能有对应的Slides，但是确实Slides似乎不全，看到哪节课先去找找实在找不到就算了
	- Lecture 和 作业或者说 Project 总共有15个

- 学习模式(有note的课程看note+记笔记；没note的课程看slides+课程+记笔记)
	- 看课+Slides +记note
	- 完成HW 
		- 指标至少通过Strong Baseline
		- 注意每次作业会有些许提示
	- 流程差不多是 先看 P 再看 C 然后做作业 最后看 S
	- 作业的话，我不在本地做而是在209上做，同时会同步到github仓库
		- 由于 作业是通过 Jupyter 做的，因此在作业的笔记中并不会记录特别多东西，大部分东西都在 Jupyter 上


- 文档名字说明
	- P 表示 preparation 其实也就是预修课，C 是正课，S表示Extra Materials 就是补充知识
	- Intro 就是把01C1的总览介绍单独拎出来了，其它不变


- 课程大纲
	- [大纲](./Lectures/Intro.md)
	- L1-2 介绍DNN的各种基础知识、组件和工作流，以最基本的MLP展开介绍
	- L3 --- CNN
	- L4 --- self-attention、RNN 和 GNN
	- L5 --- Transformer(Encoder-Decoder)、Pointer Network 以及 更多 self-attention
	- L6 --- GAN、VAE、Flow-based Model [只剩3节更详细介绍GAN]
	- L7 --- Self-Supervisor Learning and Pre-train、BERT and its family、GPT
	- L8 --- Auto-Encoder + Anomaly Detection + Unsupervised(传统 PCA + t-SNE)
	- L9 --- Explanation + Attack
		- 9 的 Attack 部分放在 L10了
	- L10 --- Attack
		- P 系列的攻击大致讲了讲各种攻击(从image和speech角度)，而 C系列的攻击具体讲了 evasion attack(从NLP角度)，其它没讲完，感兴趣可以继续看 Slides
			- 数据类型不同加入 noise 的方法都是有所差异的，noise的本质就是难以察觉但会造成较大影响
		- 其实本质上各种攻击都是要依赖 小noise
	- L11 --- Domain Adaptation + BERT自监督学习的更进一步知识
	- L12 --- DRL + PPO + Q-Learning
		- PPO 和 Q-Learning 虽然在 L13但是更贴合 L12 的主题
	- L13 --- Netword Compression
	- L14 --- Life-long Learning
	- L15 --- Meta Learning[S未看]

- 虽然 GAN 还有三节课以及 Meta Learning 没有完全看完，但是先这样吧，后续有时间可能来补，除此之外 15个作业和全部课程已经完成
