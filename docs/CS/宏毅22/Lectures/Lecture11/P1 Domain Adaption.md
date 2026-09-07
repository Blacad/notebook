
- Domain shift 领域偏移
	- 训练资料(source domain)和测试资料(target domain)有不同的分布 


- Domain Adaption
	- 直接 fine-tune
		- 在 Target domain 的资料上进行 fine-tune(Target domain可能很少有标签，大多数无标签)
	- Domain Adversarial Training(DAT)
		- [DAT](../../Slides/Lecture11/da_v6.pdf#page=8)
			- 我认为 label predictor 的参数应该冻结才有意义
		- Feature Extractor 使得 source domain 和 target domain 的数据经过它产生的 latent representation 分布相近
		- 两个 domain 的[类不同](../../Slides/Lecture11/da_v6.pdf#page=12)
			- 可以参见 Universal domain adaptation
	- 如果整体 Target 数据就很少甚至没有
		- Domain Generalization 领域泛化
			- [领域泛化](../../Slides/Lecture11/da_v6.pdf#page=15)
				- 上面是训练数据多，测试数据少，单纯训练可能就好
				- 下面是训练数据少，而测试数据多，可能需要进行数据增强