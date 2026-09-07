- 说明
	- 基于BERT做QA问题，从输入的 Document 中提取答案片段

- 依旧无外乎是
	- 数据集准备 -> 模型搭建 -> 训练器搭建 -> 推理过程
		- 训练器搭建中涉及 数据读取、优化器/损失函数设计以及学习率规划器等等环节
	- 但是，本次作业我们使用了 `transformers` 库，但是它其实主要是被用作一个模型库，帮我们加载模型，也就是说用别人已经训练好的模型，我们在上面进行微调训练以适应特定任务 --- 我们不自己定义模型结构和分词器实现(Hw5中我们需要做分词器并自定义模型结构)

- 模型搭建
	- 本次直接使用 `transformers` 库加载模型，本次作业中可以看到加载模型的类型都非常详细是 `BertForQuestionAnswering` 和 `BertTokenizerFast` ，而常规上我们会使用 `AutoModelForQuestionAnswering` 和 `AutoTokenizer` 
	- `Auto` 是遵循工厂模式会根据模型来选择对应的对象，比如 `AutoModelForQuestionAnswering`对于本任务的模型就会实际应用 `BertForQuestionAnswering`，而 `BertForQuestionAnswering` 的 模型结构、forward 方法等已经由 transformers 库使用 torch 实现好了，同时 Hugging-face 上有对应模型的参数直接加载进来就好
	- [AutoTokenizer](../../HW/Hw7/AutoTokenizer.md)
		- 返回 BatchEncoding 对象
	- [AutoModel](../../HW/Hw7/AutoModel.md)
		- 需要不同的AutoModel来指定不同用途的任务



- 数据集准备
	- 特殊点
		- 由于这里的BERT模型最大上下文长度是512，因此我们需要对输入进行处理，主要是处理 paragraph
			- 在训练时，我们知道答案的位置，就以答案为中心展开 windows 大小作为paragraph
			- 在推理时，我们不知道答案位置，就用滑动窗口，看哪个窗口得分最高就取那个窗口的答案，doc_stride 就是控制滑动窗口每次滑动多少
		- 基于上述特殊点，针对推理和训练对取数据有不同的处理
	- 对数据进行 `padding` 处理，保证每次输入的长度都是 `max_seq_len`
		- input_ids、token_type_ids、attention_mask 都在这里设定好，而不是直接用 tokenizer的返回
	- 评估函数 evaluate
		- 评估推理时哪个窗口的得分最高，取该窗口的答案
		- model 会输出 start 和 end 的 logits，我们取两者最大的概率之和，然后比较不同窗口的和，找到最大的概率和即是答案

- 训练器搭建
	- lr schedule
		- 这里直接用了 `transformers` 库里其中一个 lr_scheduler 即`get_linear_schedule_with_warmup`
	- fp16混合训练 和 accelerate
		- 利用 accelerate 实现 fp16 混合训练
		- 需要在某些地方适配 accelerate，不影响整体流程
	- 有个小细节 --- `optimizer.zero_grad()`
		- 之前的作业基本是 `model.zero_grad()`，而本次作业采用 `optimizer.zero_grad()`
		- 两者往往效果相同，只是归零化的梯度范围不同，前者是归零化模型的所有参数的梯度，后者是归零化该优化器负责参数的梯度
		- 现在用  `optimizer.zero_grad()` 更多更推荐
	- 累积梯度
		- batch 分成多个mini-batch，每个mini-batch 的梯度不直接参与优化而是等整个batch做完后用累积梯度优化