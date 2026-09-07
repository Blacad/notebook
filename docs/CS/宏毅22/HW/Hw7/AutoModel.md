
- `AutoModel` 也是一种“自动选择具体模型类”的工厂，但它和 `AutoModelForCausalLM` 自动选择的**目标类型不同**，因此通常不能直接互相替代


- AutoModel.from_pretrained(...)
	- 会返回基础模型，也就是只包含 Transformer 主干网络的模型。
	- 例如对于 GPT 类模型，通常返回类似：GPT2Model
	- 输出主要是隐藏状态：last_hidden_state: (batch_size, seq_len, hidden_size)
	
- AutoModelForCausalLM.from_pretrained(...)
	- 会返回“基础模型 + 因果语言模型输出头”
	- 例如：GPT2LMHeadModel
	- 它会把隐藏状态进一步映射到词表空间：logits: (batch_size, seq_len, vocab_size)
	- 结构大致是：
	- AutoModelForCausalLM= Transformer backbone + LM Head

- 因此我们依然需要不同类的 AutoModel，通过决定后续的任务头来适配不同的任务，常见的如下
- AutoModel
	- 只要主干网络
- AutoModelForCausalLM
	- 主干网络 + 自回归语言模型头
- AutoModelForMaskedLM
	- 主干网络 + Masked LM 头
- AutoModelForSequenceClassification
	- 主干网络 + 句子分类头
- AutoModelForTokenClassification
	- 主干网络 + token 分类头
- AutoModelForQuestionAnswering
	- 主干网络 + 起止位置预测头