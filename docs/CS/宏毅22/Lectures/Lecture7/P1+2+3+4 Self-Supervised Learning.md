
## BERT Series
- self-supervised 自监督
	- 对于没有标签的数据，让数据自身组成 标签
	- [Masking Input](../../Slides/Lecture7/P1+2+3+4bert_v8.pdf#page=11-12)
		- 随机 mask 输入的部分 token
		- mask 有两种做法
			- mask 作为 special token
			- 随机选 token mask 需要mask的 token
	- [Next Sentence Prediction](../../Slides/Lecture7/P1+2+3+4bert_v8.pdf#page=13)
		- 将两个句子作为输入，构成 `[CLS]S1[SEP]S2` 的结构，判断句子是否是相连接的，如果是 输出 Yes 否则 输出 No
		- 后面发现该方法似乎没什么用

- BERT的结构和Transformer的Encoder结构一致 --- BERT = Transformer-Encoder Only

- [BERT的用法](../../Slides/Lecture7/P1+2+3+4bert_v8.pdf#page=14)
	- self-supervised Learning 作为 Pre-train
	- 后面可以 Fine-tune 做 Downstream Tasks
	- 上述的过程可以被称为 semi-supervised Learning，即 Pre-train 没有 label，而 Fine-tune 往往有 label

- 测量预训练模型的能力
	- GLUE(General Language Understanding Evaluation) 其中包含9个任务，做法是在9个任务上微调预训练模型，看最终的分数
	- 当然现在已经有更多的测量Bench

- BERT 的 四种用法示例
	- Task 1: [Sequence to Label](../../Slides/Lecture7/P1+2+3+4bert_v8.pdf#page=17)
	- Task 2: [Sequence Labeling](../../Slides/Lecture7/P1+2+3+4bert_v8.pdf#page=20)
	- Task 3:  [Natural Language Inference(NLI)](../../Slides/Lecture7/P1+2+3+4bert_v8.pdf#page=21-22)
		- 输入 前提和假设，输出 一个类别 (矛盾/不矛盾)
	- Task 4: [Extraction-based QA](../../Slides/Lecture7/P1+2+3+4bert_v8.pdf#page=23-25)
		- 答案一定出现在文本中

- Training BERT is Challenge
	- 预训练数据量巨大且模型本身也大就使得模型训练很难

- BERT 为什么有效
	- [理解一个词需要看它的上下文](../../Slides/Lecture7/P1+2+3+4bert_v8.pdf#page=35)
	- 我认为 BERT 本质上是学会 token 的序列关系，它在海量资料中学会了 token 的序列关系，它可以翻译 token 在不同序列关系中的真实表达


## GPT Series

- GPT 的结构和Transformer的Decoder结构一致 --- GPT = Transformer-Decoder Only

- GPT的自监督学习
	- [Predict Next Token](../../Slides/Lecture7/P1+2+3+4bert_v8.pdf#page=51)

- GPT 的用法
	- [Few-shot Learning](../../Slides/Lecture7/P1+2+3+4bert_v8.pdf#page=54) --- In-context Learning



- [Self-Supervised Learning 的更多内容](../../Slides/Lecture7/P1+2+3+4bert_v8.pdf#page=57)