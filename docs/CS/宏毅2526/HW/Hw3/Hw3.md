
## 提要
Understanding Transformer

- 本次作业采用 25 年春机器学习，主要是研究 LLM 组件和内部的可解释性

- 4 个作业
	- chat_template 和 多轮对话
		- 比较有无 chat template 的影响
		- 多轮对话
	- tokenization
	- 可视化
		- Embedding 可视化
		- attention map 的可视化
	- SAE(Sparse AutoEncoder)

- 3 篇 paper
	- attention is all you need
	- differential transformer
	- scaling llm test-time


## 实践

- 虽然实验要下载新模型，但是我实在不想再下载模型了，直接用原来的模型吧

- Chat_Template
	- 比较 question 用 chat template 和不用的回答质量区别，使用 `cross-encoder` 得到回答质量得分
	- 当然使用chat_template的得分更高

- 多轮对话
	- 这里是通过 `seaborn` 可视化结果，看 model 第一个输出的top-10个最可能token的概率分布
	- 多轮就是上个loop+加个list 没什么特别

- tokenization
	- `token_ids = tokenizer(sentence, return_tensors='pt')["input_ids"].squeeze()`
		- 自带 Batch，返回 `(B,L)` 
		- 当 `type(sentence) === string` -> B=1
		- 当 `type(sentences) === list[string]` -> B=len(sentences)
	- `tokens = [tokenizer.decode(ids) for ids in token_ids]`
		- 依次解码
	- `model.generate(... use_cache=True)`
		- `generate`  配置参数后默认使用 KV_cache 不需要手动再去配置 KV_cache

- 可视化
	- Embedding 可视化 t-SNE
		- `outputs = model(**inputs, output_hidden_states=True)`
			- `outputs.hidden_states` -> N+1 个元素的tuple，每个元素是 (B,L,D)，其中N是Transformer的层数，B 是 batch 大小，L 是序列长度，D 是隐藏层维度
			- 注意，`outputs.hidden_states[-1]` 是在 LM head 之前的输出，而不是经过 LM head 的 logits
	- attention map 可视化
		- 初始化 KV_cache
			- `kv_cache = HybridCache(config=model.config, max_batch_size=1, max_cache_len=total_tokens, device="cuda", dtype=torch.float16)`
		- 配置输出
			- `outputs = model(next_token_id,attention_mask=attention_mask,cache_position=cache_position,use_cache=True,past_key_values=kv_cache, output_attentions=True)`
			- 第一次的 cache_position 需要是 `arange(input_ids.shape[1])`，后续只需要是新生成 token 的 绝对位置就好了
			- `outputs.logits` -> (B,L,V)
			- `outputs.attentions` -> N个元素的tuple，每个元素是 (B,H,Q,K)，其中N是Transformer的层数，B是batch_size, H 是 头的数量，Q 和 K 分别是 query_length(需要query的token数量)和key_length(提供key的token数量)
		- 更新kv_cache 和 cache_position
			- `kv_cache = outputs.past_key_values`
			- `cache_position = cache_position[-1:] + 1`

- SAE
	- 需要先下载 `sae-lens` 包
	- 下载别人针对 `gemma-2-2b-it` 特定层训练好的 SAE，然后我们只需要把 特定层的 hidden_state 提供给这个 SAE
	-  `sae.encode(hidden_state)`
		- (B,L,D) -> (B,L, num_features)
		- 得到 对于 decoder 的所有feature的激活值，更详细的内容请查看 [SAE&FV](../../宏毅25/Lectures/Lecture3/SAE&FV)


- Note
	- 使用 `gemma-2-2b-it` 时，用 `torch.float16` 不会有问题，但是 `gemma-3-4b-it` 时 会发生溢出现象，导致输出全为0