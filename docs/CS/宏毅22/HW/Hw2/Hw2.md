- 说明
	- 本次作业需要使用预提取的MFCC特征进行帧级音素分类，音素是语言中区分不同单词的语音单位，依旧使用MLP

- 数据
	- 由于每个帧只包含25毫秒的语音，单个帧不太可能代表一个完整的音素
	- 通常，一个音素会跨越几个帧，将相邻的音素进行拼接以进行训练。
	- 41 个 类
	- 每个.pt文件都从一个原始的wav文件中提取出来
	- 使用torch.load()读取.pt文件为torch张量
	- 每个张量的形状为(T, 39) --- T是frame的数量

- 在 prepare data 中
	- 有预处理数据的历程 `preprocess_data` 很值得学习
		- 最终的X和Y应该包含所有数据 --- 用来构建 Dataset
	- 组合数据的历程 `concat_feat`
		- `concat_feat(x,3)` 相当于将每帧和它的前一帧和后一帧组合起来形成一个位置
		- `x.repeat()` --- 用来**按照指定次数复制张量在各个维度上的内容**，并生成一个新的张量
			- `x.repeat(2,3)` --- 在第0维重复2次，第1维重复3次
			- 如果 x 的维度低于repeat的维度
				- `x.shape == [n]`
				- `x.repeat(2,1)`
				- `会先把一维张量视为形状 [1, n]，然后：第 0 维重复 2 次;第 1 维重复 1 次得到[2,n]`
		- `x.permute()` --- 用来**重新排列张量各个维度的顺序**，它的参数指代 原维度的需要移动到该位置(CHW(012)->HCW(102)第1维移动到首个位置，第0维移动到次位置依次类推)
			- `x = x.view(seq_len, concat_n, feature_dim).permute(1, 0, 2) # concat_n, seq_len, feature_dim`
			- 关于permute后元素位置变化
				- `y = x.permute(1, 0, 2)` --- `y[i, j, k] = x[j, i, k]`

- 在 define model 处我们使用 BasicBlock 能够节省很多相似代码并用超参数 hidden_layer 来控制隐藏层数量，这个也很值得学习

- [nn中BatchNorm和Dropout应用](nn中BatchNorm和Dropout应用.md)

- 我喜欢增添 config 逻辑 和 early_stopping 逻辑
	- 前者帮我记录超参数
	- 后者让我能将epoch尽量设置大一些

- 增添 `scheduler` 使得学习率变化更加稳定


- 损失计算 `criterion = nn.CrossEntropyLoss()`
	- 有 `loss = criterion(outputs, labels)` ，其中 `outputs.shape=(B,Classes) labels.shape=(B)`，内部已经包含了 `Softmax + Log + NLLLoss`，所以传入的应当是原始 `logits`，不要提前手动做 `softmax`
	- 具体而言，`logits[i]` 是第 i 个样本对所有类别的预测分数 ，例如 `[0.1,0.3,...]`，`labels[i]` 是第 i 个样本的正确类别编号，例如 `0, 1, ..., class-1`
	- `logits.argmax(dim=-1)` --- 输出最大的下标，即最终标签


- `gc.collect()` 是 Python 中**手动触发垃圾回收**的函数，配合 `del` 使用

- 后续就是疯狂调参 --- 这个实验有点费时，所以比较慢


