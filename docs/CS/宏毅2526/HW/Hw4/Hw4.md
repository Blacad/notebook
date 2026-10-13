## 提要
- Exploring GenAI

- 本次作业采用 25 年秋机器学习，主要是研究 LLM 内部的可解释性，相比 Hw3，这次作业是对上次的补充，Hw3在可解释性上，只研究了 embedding 聚类可视化、attention map 的可视化分析以及 SAE，而 Hw4 将研究更多可解释性主题

- 3个作业
	- Logit Lens
		- 把每层的 hidden state 放进 LM head 看看输出什么 (残差流)
		- 分析 每层 可能在干什么
	- Function Vector
		- 利用 功能向量 + 原向量 获得功能能力
		- 分析 功能向量 作用
	- Patch Scope
		- 将一个句子的最后一个token某层的 hidden state 替换另一个句子最后一个token的hidden state，然后看 后面一个句子的输出会怎么改变
		- 分析 最后一个 token 已经融入了哪些信息

## 实践

- 用 gemma-2-2b-it 来做的实验


- Logit Lens
	- 读取 model 的层数共有 26 层，我们需要在 `from_pretrain` 的时候配置 `output_hidden_states=True`，然后就能在 `outputs.hidden_states` 得到完整的隐藏层
	- 步骤
		1. 选定特定层，并取它的最后token的 hidden state，`hidden_states[target_layer][0, -1]`
		2. 经过 model 的norm `model.model.norm(hidden)`
		3. 经过 model 的 lm_head `model.lm_head(hidden).to(torch.float32)` 得到 `logits` 
		4. 最后分析 topk 的 token 
			1. `topk = torch.topk(logits, k=5)`
			2. `token_ids = topk.indices.tolist()`
			3. `tokens = tokenizer.convert_ids_to_tokens(token_ids)`

- Function Vector
	- 由于这里的数据集我下载不下来就分析它的做法好了
	- Function Vector 分为两个大阶段，获得 FV 和 使用 FV
	- 获得 FV
		1. 收集一系列正常的 prompts 和 会产生拒绝的 prompts
		2. 所有正常 prompts 依次喂给模型，提取模型某层最后一个token的hidden state `(D,)`，最终拼接成 `(N,D)`，求均值 `.mean(dim=0,keepdim=True)` -> `(1,D)`。对所有拒绝prompts进行同样的操作
		3. 将 拒绝的均值 减去 正常的均值 得到 FV `(1,D)`
	- 使用 FV
		- 通过 `model.model.layers[layer].register_forward_hook(patch_fn)` 将 FV 注入到该层的 hidden states 中 (通过广播机制，该层所有token的hidden state都会被注入FV)
			- 这个 hook 注册机制之前也用过，宏毅22的Hw13 有更详细的描述，这个 hook 是发生在该层计算完成后触发因此有 output

- Patch Scope
	- 作业里没有实现 Patch Scope 我自己写了一版简单的实现
	- 分为两个大步骤，获得 源hidden state 和 替换目标hidden state
	- 获得源 hidden state
		- 很简单类似于 Logit Lens 里的做法
	- 替换目标 hidden state
		- 这里最难的部分是设计 target prompt，你总是不能很好地替换掉占位的token(占位token后面的token会编码占位token的信息)，于是这里采用 ==空格== ，这是因为分词器往往将 `空格+token` 也额外当成另一个token，于是可以使得占位token就是最后一个token便于替换，即 `cat cat\ndog dog\nx`，其中 `x` 是占位符
		- 替换方式就很简单了，使用 hook 技术 与 使用FV 中的做法类似，只是这里直接替换而不是注入
