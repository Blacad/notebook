- 说明
	- 基于 GAN 实现 动漫图像生成任务
	- 为了达到 Strong 的要求，我们按照 WGAN-gp 来实现上述任务
	- 当我看到它真的生成了Anime图片的时候，那种激动之情瞬间使我澎湃不已


- 无外乎的核心历程依旧是
	- 数据集准备 -> 模型搭建 -> 训练器搭建 -> 推理过程
		- 训练器搭建中涉及 数据读取、优化器/损失函数设计以及学习率规划器等等环节


- 数据集准备
	- 读取数据
		- 在 Hw3中我们读取处理过图像数据，但是当时是直接用 PIL 的 `Image.open(fname)` 直接将图片读取以 PIL 的格式读取到内存中，而在本次作业中，`torchvision.io.read_image(fname)` 是将图片读取以 Tensor 的形式存储在内存中
		- 对图片数据增强的方法大多基于 PIL 格式，因此在本作业中需要先将其转换为 PIL 格式做数据处理，然后再转回 Tensor 格式方便后续操作
		- 其实作业三中还做了很多数据增强的工作，这里没有做，就是简单的标准化处理
		- 构建数据集DataSet其实就是记录数据的元信息，方便它的读取方法能够迅速读取，我认为本次作业的读文件方式相对Hw3更老道
		- Tips
			- Tensor 形式图片默认 (通道、高、宽)，PIL 和 Numpy 图像 默认 (高、宽、通道)
			- Tensor 的图像像素会被转换为 0～1 的浮点数
	- plt 的 图像可视化默认支持 (高、宽、通道)，因此 Tensor也需要转变 `grid_img.permute(1, 2, 0)`


- 模型搭建
	- 生成器
		- 反卷积 
			- 卷积常用于提取特征、减少空间尺寸，而反卷积则是上采样、增大空间尺寸
			- 卷积 --- 一块输入区域 → 一个输出值；反卷积 --- 一个输入值 → 一块输出区域
			- `nn.ConvTranspose2d(feature_dim, out_dim, kernel_size=5, stride=2,padding=2, output_padding=1, bias=False)` 
				- [反卷积](../../HW/Hw6/反卷积.md)
				- $H_{out}​=(H_{in}​−1)s−2p+k+output\_padding$
		- 噪声转图像 --- 将经过处理的高斯噪声特征转换为图像格式
			- `y = y.view(y.size(0), -1, 4, 4)` 
				- 由于 y 是 (B,N) 的形状
				- `y.size(0)`：保持 Batch size 不变
				- `4, 4`：指定空间尺寸为 $4\times4$
				- `-1`：自动计算通道数 $C$
		- 激活函数 `nn.Tanh()` --- 双曲正切
			- 将输出限定在 -1～1 之间，确保和上述图像处理后的数值在同样范围，常用于生成器的输出层
	- 判别器
		- `nn.LeakyReLU(0.2)` 
			- `LeakyReLU` 是 ReLU 的改进版本。它在输入为负数时，不会直接输出 0，而是保留一个较小的斜率，比如这里是 0.2
			- 原先 ReLU = max(0,x)
			- 改进后 LeakyReLU(0.2) = x(x>=0) 0.2x(x<0)
		- `nn.InstanceNorm2d()`
			- `InstanceNorm2d` 是针对每个样本、每个通道分别做归一化的层，即`InstanceNorm2d` 会对每个样本中的每个通道，单独统计该通道所有空间位置的均值和方差并做Norm
			- 它类似于2d层面的 LayerNorm 不依赖 Batch
			- WGAN不能使用BatchNorm2d
				- `BatchNorm2d` 会让一个样本的输出依赖同一 Batch 中其他样本，而梯度惩罚希望独立约束每个样本的输入梯度

- 训练器搭建
	- ==参数权重初始化==
		- `self.apply(weights_init)`
	- ==判别器的梯度惩罚 gp==
		- 流程
			- 构建插值图像
			- 输入判别器得到插值图像的输出
			- 求导得到梯度集合 $\partial D(x)/\partial x$
				- 其中 grad_outputs 控制了各个输出分量对最终梯度的权重
				- [梯度解析](../../HW/Hw6/梯度解析.md)
			- 约束梯度集合的 L1 范数靠近1(正常应该是L2范数)
		- 为什么要 输入梯度惩罚
			- 输入梯度：$\nabla_xD(x)$ 描述的是判别器函数在数据空间中的平滑程度，即衡量输入变化幅度对输出变化幅度的影响
			- 在WGAN中要求判别器对数据空间要足够平滑，这样优化的判别器目标才能够理论收敛
			- [判别器的梯度惩罚](../../HW/Hw6/判别器的梯度惩罚.md)
		- 损失
			- `loss_D = -torch.mean(r_logit) + torch.mean(f_logit) + ((gradients.norm(1, dim=1) - 1)**2).mean()`
			- `loss_G = loss_G = -torch.mean(self.D(f_imgs))`
		- GAN都是先训 D 再训 G