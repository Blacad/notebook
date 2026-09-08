- 说明
	- 本次作业使用CNN做图片分类任务，需要使用数据增强和残差机制，数据增强可以使用 空间转换并基于此采用多种手段，同时还有mix-up 等数据增强方法
	- Test Time Augmentation 用训练的 空间转换对 测试图片进行改变然后多张图片一起输入集合投票(空间转换和CNN本身是end-to-end的训练)
	- 利用 cross-validation 
	- 集成的不同方法
		- 平均对数几率或概率：需要保存详细输出，减少歧义
		- 投票：更容易实现，需要解决平局
	- 可以使用 `Gradient Accumulation` 实现大 batch
	- 还有上次作业的 Dropout + BatchNorm

- 图片总共有11类，图片名字的第一个数字便是其类别，图片都是 `3*128*128` 


- 有关CNN的方法
	- `torch` 的卷积初始化 `torch.nn.Conv2d(in_channels, out_channels, kernel_size, stride, padding，groups)`
		- 在 `Conv2d` 中，假设输入有 `4` 个通道，卷积核空间大小为 `(3,3)`。当 `groups=2` 时，输入通道被分为2组，每个group为 $(4/2=2,H,W)$ ，单个 filter 的尺寸为：$(4/2=2,3,3)$ 即一个 filter 包含 `2` 个 `(3,3)` 权重切片，分别与group的对应输入通道卷积，再将各通道结果相加，最终得到一个输出 channel
		- `out_channels` 决定 filter 的数量，也就是输出 channel 数，而单个filter尺寸为$\left(\frac{in_{channels}}{groups}, K_h,K_w  \right)$ 
			- 其中`groups` 决定 channel 的分组方式；每个 filter 只处理一个 group 内的输入 channels 且 filter 的 channel 尺寸与该group的channel尺寸一致
			- 相当于一个输入被拆分成 groups 个组，每个组由一个filter处理
		- 因此，in_channels、groups、kernel_size 决定单个filter的完整尺寸，out_channels 决定 filter的数量，stride 和 padding 决定卷积方式
		- 这里的padding是指同时像两侧填，比如 `padding=1` 就意味着 上下左右各补一个
		- 因此，最后输出的shape 应该是 (out_channels, (width + 2 \* padding_width - kernel_width)/stride_width + 1, (height + 2 \* padding_height - kernel_height)/stride_height + 1)
	- `torch.nn.BatchNorm2d(channels)`
		- 对图直接进行 BatchNorm，它期望的输入是 channels 个通道的图，比如我的batch size是32，那么batch是 (32, channel, height, width)
		- 由于每个通道都是分开独立进行BatchNorm，因此对 `channels` 个通道分别维护独立的均值、方差、缩放参数和偏移参数，也就是说对channels个 (32, height, width) 分别做 BatchNorm
	- `torch.nn.MaxPool2d(kernel_size, stride, padding)`
		- 最大池化，它也是对输入图的不同channels分别做池化，因此不会改变channels数量
		- 比如输入 (channels, height, width) -> (channels, (height+2 padding - kernel_height)/stride +1,  (width+2 padding - kernel_width)/stride +1)
		- kernel_size 决定单个pool的空间大小，stride 和 padding 决定 pool 的方式
	- Flatten ---  `out = out.view(out.size(0), -1)`
		- 由于卷积层的结果的shape一直是三维的(channels, height, width)，不能直接给MLP去处理(MLP希望每个样本表示成一个特征向量)，需要进行flatten，变成一维的即 (channels \* height \* width)，带上 batch的话就是 (batch, channels, height, width) -> (batch, channels \* height \* width)
		- 因此上述Flatten就是保留 batch 维，其他维度变成同一维，这样就可以交给MLP去处理了
		- 总体而言，CNN 输出保留通道和空间结构，形状通常为 (N,C,H,W)。在接入普通全连接分类器前，需要保留 batch 维，并将每个样本的 C,H,W 展平为一个特征向量，即 (N,CHW)


- `grad_norm = nn.utils.clip_grad_norm_(model.parameters(), max_norm=10)`
	- `clip_grad_norm_` 将模型所有参数的梯度作为一个整体限制其范数，避免一次参数更新过大，提高训练稳定性；它不会限制参数本身的大小
	- 末尾的下划线 `_` 表示这个函数会**原地修改参数的 `.grad`
	- 具体而言，`clip_grad_norm_` 会先把所有参数的梯度看成一个整体G，计算总梯度范数。默认使用 L2​ 范数，如果总梯度范数不超过 10，则梯度保持不变；如果超过 10，则把所有参数的梯度按相同比例缩小，$g_i = g_i \cdot \frac{10}{L_2(G)}$
	- 使用位置 --- loss.backward() 之后，optimizer.step() 之前

- 数据增强 ---  变换 `torchvision.transforms.Compose([])`
	- [Compose](Compose.md) 的使用说明
	- 可以在 DataSet 的 `__getitem__` 方法中对图像进行变化，这样多训练几个epoch就可以训练到经过不同变换的图片，实现数据增强
	- 这个只对训练数据使用，不要对验证和测试数据使用

- 残差缓解梯度消失问题，并用 cross-validation 更充分训练(可以使用验证集训练)
	- 因为 cross-validation 可以得出不同fold的最优模型，最后我们可以使用不同fold的最优模型进行投票，因为模型输出都是 (B,Classes)，那么我们把所有的模型结果相加，再取最大下标就相当于选出所有模型集合起来最认同的类 
	- (B,Classes) -> `np.argmax(test_preds, axis=1)`->(B)

- python 读取文件夹中所有文件路径，例如
	```python
	train_dir = "./food11/training"
	train_files = [os.path.join(train_dir, x) for x in os.listdir(train_dir) if x.endswith('.jpg')]
	```

- `test_pred = model_best(data.to(device)).cpu().data.numpy()` --- 放进cpu并转成numpy
	- 因为 NumPy 不能直接处理 CUDA Tensor，因此把输出张量从 GPU 搬回 CPU，方便CPU侧的工具处理
	- 将 PyTorch Tensor 转成 NumPy 数组，方便后续使用 NumPy、`argmax`、保存 CSV 等操作

- 注意，当我们不需要梯度且运行模型的 forward 需要使用 with torch.no_grad():，避免梯度产生占空间



