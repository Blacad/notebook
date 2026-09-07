- 说明
	- 本次作业使用 self-attention 去做语音识别，即通过语音特征识别出speaker，简单来说还是分类任务，总共有600类
	- self-attention 的优势就是能够处理 Sequence 数据，即 单样本是(L, feature) 的数据，它能通过自注意力机制关注上下文信息，实现类似于RNN的效果且现在几乎已经替代RNN处理Sequence数据
	- [Slides](./Machine%20Learning%20HW4.pdf) 中写了很详细的各种技术


- 读取 json 文件
	- `mapping_path = Path(data_dir) / "mapping.json"`
	- `with open(path, 'r', encoding="utf-8") as f: `
		- `mapping = json.load(f)`
	- 上述方法是最标准的读取 json 文件的方式，可以自动关闭

- DataLoader 处理Batch中不同长度序列的方法 --- `collate_fn` 参数
	- 该参数需要传入一个函数，该函数接受一个batch，并将batch中的序列长度进行统一
		```python
	def collate_batch(batch):
	# Process features within a batch.
	"""Collate a batch of data."""
	mel, speaker = zip(*batch)
	# Because we train the model batch by batch, we need to pad the features in the same batch to make their lengths the same.
	mel = pad_sequence(mel, batch_first=True, padding_value=-20) # pad log 10^(-20) which is very small value.
	# mel: (batch size, length, 40)
	return mel, torch.FloatTensor(speaker).long()
		```
	- `from torch.nn.utils.rnn import pad_sequence` --- 进行对齐
		- `mel` 是一个 Tensor 列表，列表中每个 Tensor 的序列长度可以不同([80,40],[120,40])，但其余维度需要一致
		- 找到最长序列长度，把较短序列在末尾补到最长长度，其中补充部分的每个元素都是 `-20`
		- 最后输出 --- `(batch size, length, dim)`
	- 整个过程
		- 原始数据：[(特征1, 标签1), (特征2, 标签2), ...]  
		- collate后：(特征batch, 标签batch)

- `self.encoder_layer = nn.TransformerEncoderLayer(d_model=d_model, dim_feedforward=256, nhead=2)`  --- 完整的TransformerEncoderLayer
	- 内部结构是 `input -> Multi-Head Self-Attention -> Dropout -> 残差 + LayerNorm -> Linear(d_model->256)-> Activation -> Dropout -> Linear(256->d_model)->残差连接 + LayerNorm -> output`
	- 其默认 $d_k=d_v=d_{model}/n_{head}$
	- 它期待的输入shape 是 (seq_len, batch_size, d_model)，最后的输出也是 (seq_len, batch_size, d_model) 
		- ==但是==可以通过设置参数 batch_first=True，使得其输入和输出都是 （B,L,D)
		- 之所以原来将 L 放在最前面是因为RNN时代可以方便取同一时间步的数据运算
		- self-attention不需要这样因此可以设置 batch_first=True
	- `self.encoder = nn.TransformerEncoder(self.encoder_layer, num_layers=3)` 可以使用该方式构成多个encoder层

- 自定义LR_Scheduler --- `from torch.optim.lr_scheduler import LambdaLR`
	- 返回 `LambdaLR(Optimizer, lambdalr, last_epoch)` 就可用作LR_Scheduler，其中`lambdalr` 可以是自己写的函数
	- 详见代码


- Conformer --- `ConformerBlock(dim = d_model,dim_head = 4,heads = 4,ff_mult = 4,conv_expansion_factor = 2, conv_kernel_size = 20,attn_dropout = dropout,ff_dropout = dropout,conv_dropout = dropout)`
	- Conformer 作为 TransformerEncoder 的变体，它在多头自注意力机制后面加上了卷积模块，具体结构详见Slides
	- 它能在 自注意力机制 关注全局信息的同时，利用卷积关注局部信息，特别是在语音处理上经常使用
	- [[Conformer参数解析]]