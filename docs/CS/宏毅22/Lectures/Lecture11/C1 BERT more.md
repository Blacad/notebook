
- BERT 自监督学习的神奇能力
	- Cross-lingual  跨语言
	- Cross-discipline 跨学科
	- Pre-training without Human Languages 无人类语言的预训练


## 跨语言
- 模型预训练如果采用多语言的训练，那么在 fine-tuning 为解QA的model时，QA的训练数据只用某种语言比如英文，但是最终测试在中文QA中的表现能力也会提高，[如表](../../Slides/Lecture11/More-self-supervised%28v2%29.pdf#page=5)

- [XTREME benchmark](../../Slides/Lecture11/More-self-supervised%28v2%29.pdf#page=7) 就是验证PLM的跨语言能力

- 一般训练语言越多 跨语言的能力越强

- 那么它如果跨语言能力这么强，那么它怎么知道我输入英文的填空，它就用英文填空，而不用其它语言
	- 这是因为 它 依然编码了不同语言之间的 差异，这种差异有个朴素的方法寻找就是取所有中文 embedding 的均值和所有英文 embedding 的均值，均值距离视作 差异
	- [测试实验如图](../../Slides/Lecture11/More-self-supervised%28v2%29.pdf#page=13) 确实有点说法
	- 居然有这样的能力是否可以优化 在 英文 上训练的模型但是在中文上测试的模型的性能呢
		- [具体做法如图](../../Slides/Lecture11/More-self-supervised%28v2%29.pdf#page=18)

## 跨学科

- 模型预训练在 人类的语言上，但是 fine-tuning 的下游任务和语言无关，比如用来做 DNA 分类

- 其实我们需要做 verbalizer 即建立 DNA 的碱基对应的英文单词，然后将DNA序列预测问题转换为 文本填空
	- [做法如图](../../Slides/Lecture11/More-self-supervised%28v2%29.pdf#page=23)
	- [表现结果如图](../../Slides/Lecture11/More-self-supervised%28v2%29.pdf#page=24)

- 老师这里讲了讲做科研的经历 --- 想做到完全 speech 问 speech 答的 end-to-end 系统
	- 单纯 HuBERT x
	- HuBERT + 多几层 attention layer x
	- HuBERT + BERT(pre-trained on text) 成功 -> 正是看到BERT(pre-trained on text) + 简单的verbalizer 甚至可以做DNA分类突发奇想，可以把HuBERT的输出也做 verbalizer，这样不就可以用到BERT的强大性能吗
	- [做法如图](../../Slides/Lecture11/More-self-supervised%28v2%29.pdf#page=37)


## 用人造资料训练 BERT
- 利用某种规则构造人造资料，然后用 人造数据预训练 BERT，看看该 BERT 在语言上的表现如何
	- 该人造数据可以是各种稀奇古怪的东西，[做法如图](../../Slides/Lecture11/More-self-supervised%28v2%29.pdf#page=41)
	- 发现这样其实是不好的，[数据质量真的很重要](../../Slides/Lecture11/More-self-supervised%28v2%29.pdf#page=43)

