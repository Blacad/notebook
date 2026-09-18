
## 一个神经元

从矩阵视角看，单神经元近似是某层权重矩阵的一列/行
***操作单神经元***

验证单神经元的功能
- 激活神经元
- 移除神经元
- 不同程度激活神经元

[川普神经元18-19](../../Slides/Lecture3/model_inside.pdf#page=18-19)

单神经元很难解释最终输出，往往是 多对多 的情形，即 一个任务与多个神经元有关，而 一个神经元与多个任务有关



## 一层神经元
层神经元近似看成某层的权重矩阵
 ***功能向量 FV***

抽取某种功能向量
- [实际向量 = 功能向量 + 其他](../../Slides/Lecture3/model_inside.pdf#page=28)
- 抽取方式
	- 找满足功能的情形，收集所有向量
	- 找不满足功能的情形，收集向量
	- 朴素的方式 用 满足功能向量的均值 - 不满足功能向量的均值 = 功能向量均值
- 使用方法
	- 将功能向量 加入 该层原输出向量 [得到功能能力](../../Slides/Lecture3/model_inside.pdf#page=32)

常见的功能向量
- Sycophancy Vector
- Truthful Vector
- [Function Vector](../../Slides/Lecture3/model_inside.pdf#page=32)
	- 挺有意思的，构建某种功能 `f` 构造文本 `seg1fseg2`，提取 `f` 的 hidden，然后给新的 `seg1f` 的 `f` 中加入该 hidden，进而实现 `f` 的功能
	- https://arxiv.org/abs/2310.15213 本文中采用的方法可能更加复杂一点，它会寻找重要的 attention head 只对这些 attention head 的向量操作

- 将每层的向量拆解为 k 个 功能向量的线性组合
	- [功能向量线性组合成向量](../../Slides/Lecture3/model_inside.pdf#page=49)
	- 训练寻找功能向量 -> e 越小越好、全部功能向量权重 L1 越小越好
		- Sparse Auto-Encoder(SAE) -> 通过训练的方式寻找功能向量
		- [SAE&FV]()


## 一群神经元
群神经元就是多层、模块或者说整个模型
***语言模型的模型***

语言模型的模型 Circuit
- 系统化建构语言模型的模型 会使用 Pruning 技术，同时保证目标任务的答案不变
- 建构语言模型的模型 vs Network Compression
	- 前者 关注目标任务答案不变
	- 后者 关注所有任务的性能不下降
	- 两者共用的技术之一是 Pruning


## AI 解释 AI
直接让AI告诉你它是怎么给出答案的
- ~~我想了解你的心而不是你的外貌~~



## 语言模型的思维是透明的
***Residual Stream + 循环解析***


Residual Stream 视角
- 从 [残差流视角](../../Slides/Lecture3/model_inside.pdf#page=79) 看，其实通过每层都只不过是向 原始输入中 改变某些讯息
- 因此，其实每一层的输出都是透明的，我们都可以将其解码出来[图示](../../Slides/Lecture3/model_inside.pdf#page=80)

Transformer Feed-Forward Layers Are Key-Value Memories https://arxiv.org/abs/2012.14913
- 改变思考 Feed Forward 的方式 [图示](../../Slides/Lecture3/model_inside.pdf#page=87)

当然，你的每个输出其实是预测下一个位置的token的，因此中间状态的意义应该在于它对下一个输出字的能力体现 + 对原有信息的核心保留，Patchscopes 给出了很好的方式
- Patchscopes 给出了 一个方案 + 一个洞见
	- 方案 - [Patchscopes方法91-92](../../Slides/Lecture3/model_inside.pdf#page=91-92)
	- 洞见 - [LLM似乎是按层逐步解析的](../../Slides/Lecture3/model_inside.pdf#page=93)
	-  https://arxiv.org/pdf/2401.06102


由上述逐步解析的思路，依据假设对于一个长句子，它会逐步解析其实体，而可能存在某些实体在前面没有解析出来，而后续实体需要依赖的情形，因此需要进行循环解析，也就是 循环 Transformer
- [循环解析](../../Slides/Lecture3/model_inside.pdf#page=94)
- https://arxiv.org/abs/2406.12775

这一小节的内容震撼人心
- Residual Stream 让我重新审视所谓的 Denoise 与 Layers  manipulation 关系
- Patchscopes 的方法让我洞悉到 循环解析的真正内涵