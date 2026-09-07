- 说明
	- 本次作业是使用 Transformer 完成机器翻译任务，即将 英文翻译为中文
	- 这次的作业使用的版本过于陈旧，配环境有些费劲，另外我发现本次作业的训练数据和测试数据已经完全找不到了，所以本次作业应该无法直接上手实践，只能通过代码来理解了

- 数据预处理流程 --- Preprocess files
	- `strQ2B` --- 把字符串中的全角字符转换成半角字符
		- `inside_code = ord(uchar)` --- 把字符转成 Unicode 编码，半角 A 和 全角 A 的编码不同，前者是65后者是65313，因此需要进行编码转换
		- `rstring += chr(inside_code)` --- 将Unicode编码再转换为字符，此时全部都是半角字符了
		- 在中文的NLP任务中可能混有全角和半角字符，统一成半角后，可以减少词表混乱、匹配失败、重复字符等问题
	- `clean_s` --- 对英文或中文句子做文本清洗和标准化，让后续模型训练/推理时输入更统一
		- `s = re.sub(r"\([^()]*\)", "", s) # remove ([text])` --- `hello (hh) world` -> `hello  world`
			- 其中 `[^()]` 表示除了 `()` 的所有其他字符
			- 正则表达式 的 `r` 是让 Python 不提前处理反斜杠 `\`，而是把原文本交给正则表达式引擎处理，如果不写r可能要写成 `\\`
			- 第一个正则表达式匹配的就是类似 `(text)` 的部分 `\(` 是因为要转义
			- `re.sub`能够实现正则匹配式的替换和删除，但是 `s.replace` 只能实现固定字符产的替换和删除
		- `s = re.sub('([.,;!?()\"])', r' \1 ', s) # keep punctuation` --- 在标签前后加空格 `hello,world` -> `hello , world`
			- 第一个正则表达式 `()` 表示捕获分组，`[.,;!?()\"]` 匹配其中任何一个字符，`\1` 代表第 1 个捕获分组匹配到的内容

	- `print` 写入文件 --- `print(s1, file=l1_out_f)`
		- 与常规的 `l1_out_f.write(s1)` 的不同，`print` 写入是将屏幕中打印的内容直接写入，而 后者是写入 `s1`

- 子词单元 Subword Units 尽可能控制词表不过大

	- 这里直接用 sentencepiece 包进行分词，它会读取源语言和目标语言的训练/验证语料，然后学习一个大小为 `vocab_size` 的子词词表，并保存成模型文件，之后就可以用它来tokenize文本，空格也会被视作普通字符参与分词用 `_` 表示
		- 分词 --- `tokens = sp.encode("I love machine learning.", out_type=str)`
		- 词id --- `ids = sp.encode("I love machine learning.", out_type=int)`

	- `input_sentence_size` + `shuffle_input_sentence=True` 
		- 前者控制训练的语料总大小，避免完整语料过大，后者保证从打乱后的语料中采样，避免只取到文件前面一部分
	- `character_coverage=1`
		- 训练语料中出现过的字符，尽量都作为“字符级 token”保留在 SentencePiece 的基础词表里，比如训练预料中有 low，尽量保证 `l`、`o`、`w` 这些单字符都在词表里，这样最后的基础词表可能同时有  `l`、`o`、`w` 、`lo`、`low` 等
	- `normalization_rule_name='nmt_nfkc_cf'` 
		- 使用 NMT 常用的文本规范化规则，大致作用大致作用包括：Unicode 规范化、大小写折叠、符号统一等

	- 介绍主流的两种算法
		- unigram
			1. 先生成一个较大的候选子词集合 
			2. 用 EM 算法估计每个子词的概率 
			3. 计算删除某个子词后对整体语料似然的影响 
			4. 删除影响较小的子词 
			5. 重复直到词表大小达到 vocab_size
			- 大词表 -> 小词表
		- BPE
			- 字符级词表 → 不断合并高频相邻片段(得到合并规则) → 达到指定词表大小
			- 小词表 -> 大词表
			- BPE的使用细节
				1. 将文本拆成初始单元，例如字符或字节：abcd
				2. 查看当前序列中所有相邻 pair：(a,b)  (b,c)  (c,d)
				3. 查询这些 pair 在 BPE 合并规则中的优先级。
				4. 选择其中**优先级最高**的 pair 进行合并。
				5. 更新序列，重新查看新产生的相邻 pair。
				6. 重复，直到没有可应用的合并规则。
		- 两种分词算法 一般都会保留字符级 token作为兜底，避免无法分词

- Binarize the data with fairseq
	- fairseq preprocess 的过程
		1. 建立词典 --- 它会读取训练集中的 token，统计词频，建立词典，同时也会添加 `</s>` 作为句子结束符token(该token由fairseq添加)以及其他特殊token
		2. 把文本token转换为词典id 
		3. 保存成 fairseq 的二进制格式 --- 这一步被称为 Binarize
			- .bin 保存真正的 token ids
			- 数据 .idx 保存索引，方便快速定位每个样本 
			- dict.\*.txt 保存词典
		4. 过滤/检查异常句子 --- 做一些基本检查，比如 token 是否在词典中, 是否需要替换成 \<unk\>

	- 分词器 和 fairseq 各自都有一套 token ids，它们的 token ids 并不相同

- python中的 Namespace 是一个类，类似于 python中字典的用法

- 利用 fairseq 中的 翻译 task 构造 dataset 以及 dataloader(Dataset Iterator)
	- 教师强制：为了训练模型根据前缀预测下一个标记，我们将右移的目标序列作为解码器输入
		- 一般来讲开头补充 bos，但是fairseq 直接在开头补 eos，其实效果一样
		- 于是在fairseq中，假设翻译的结果是 我爱你 其 token ids 是 32 34 36 2(eos=2)，那么给 Decoder的输入(prev_output_tokens)应该右移变成 2 32 34 36，同时 Decoder的监督目标(target)就是 32 34 36 2
	- 同时 它还会补 pad 以保证每个batch中的序列长度相同


- Transformer 模型是直接使用 fairseq 提供的，而项目中基于RNN实现


- RNN Encoder
	- embedding 层 `nn.Embedding(len(dict), args.embed_dim, dict.pad())`
		- `len(dict)` --- 词表大小
		- `args.embed_dim` --- embedding 向量维度
		- `dict.pad()` --- 用于pad的特殊字符\<pad\>的token id
			- 对应行的 embedding 为全0 且不会更新
	- RNN的输入
		- (B T C) 其中 B 是 batch size，T 是 序列长度，C是embedding维度，padding后的序列长度T应该是一致的
	- RNN的输出 --- 这里我们考虑多层单向的RNN输出(双向相当于有两个)
		- `output, hidden = rnn(x)`
		- `output` --- 最后一层在所有时间步的输出(hidden states)
		- `hidden` --- 每一层最后时刻的 hidden state
		- 举一个单层单向的例子，input: c1,c2,c3 -> h1, h2, h3，那么 output = (h1, h2, h3), hidden = h3
	- `encoder_padding_mask = src_tokens.eq(self.padding_idx).t()`
		- `src_tokens.eq(self.padding_idx)` 
			- 生成 encoder 的 padding mask，用来告诉 attention 哪些位置是 `<pad>`，这些位置不应该被关注
		- `.t()`
			- 将结果转置


- Attention
	- 在计算完 attention score 矩阵后，需要进行 pad token 的 mask，然后再进行后续的 softmax等操作

- RNN Decoder
	- 它还有使用 weight tying 的选项，即 input 和 output embedding使用同一套参数


- Optimization
	- 在 Loss 计算上，传统方式是 输出的词表分布 和 ground truth 的 one-hot 向量 做交叉熵，但是这样可能过拟合，于是引入 Label Smoothing Regularization，对其他一些label保留一些概率
		- nll_loss --- ground truth 在输出词表概率的负对数
		- smooth_loss --- 输出词表概率的负对数之和
	- Adam + lr scheduling
		- 将优化器进行封装，把自己实现的学习率调整机制融入，其实也可以用 作业4中的 `LambdaLR` 闭包来做到这一点
		- `@property` 装饰器 把一个方法伪装成属性访问
		- `optimizer.param_groups` 是一个字典列表，其中字典的key有 params、lr 等等优化器会用到的量


- Training
	- 混合精度训练 automatic mixed precision (amp)
		-   `scaler = GradScaler()` 和  `with autocast():` 搭配 用于 混合精度训练，普通训练都是用 `fp32` 而 AMP 会在部分计算中使用 `bf16\fp16`以降低开销
		- `GradScaler()` 解决低精度下训练时梯度太小导致下溢变成 0的问题，它的思路很简单：在反向传播前，先把 loss 放大很多倍，让梯度也跟着放大，避免太小变成 0；真正更新参数前，再把梯度缩回去
		- `with autocast():` 决定哪些计算用 float16，哪些计算保留 float32