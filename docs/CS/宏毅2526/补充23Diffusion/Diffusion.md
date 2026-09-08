
- Diffusion 的基础工作流程
	- 从与图片相同大小的几乎完全是高斯noise 的图片 一步步 降噪为 真实图片
	- [工作流程图](DiffusionModel.pdf#page=4)
	- [Noise内部](DiffusionModel.pdf#page=5)


- 训练资料的创建过程
	- 向真实图片一步步加噪到最后几乎完全是高斯noise
	- [如图](DiffusionModel.pdf#page=7)


- 增添 文字引导生成的能力
	- 在 noise predicter 中增添文字部分的输入
	- 训练资料中需要增添文字信息
	- [增添文字输入](DiffusionModel.pdf#page=11)
	- [增添文字信息](DiffusionModel.pdf#page=12)
	- [完整算法](DiffusionModel.pdf#page=13)

