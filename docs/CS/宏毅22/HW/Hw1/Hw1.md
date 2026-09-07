
- 使用 MLP 完成预测新冠病毒的回归任务

- `tqdm` --- tqdm 是一个 Python 进度条库，用于在循环或迭代过程中显示进度条，让你直观地看到代码执行的进度和预估剩余时间。

- [确保可复现性](可复现性.md)

- [数据分割中的新RNG作用](数据分割RNG.md)

- [模型保存](模型保存.md)

- [常用特征选择方法](特征选择.md)
	- 这里只列出了两种方法，在 sklearn 的特征选择中还有更多方法，但是我并不想在这种特征工程上浪费太多时间，一般情况下就选用 第二个就好了，然后就不要考虑特征选择的问题了，而去做超参数和模型架构相关的问题
	- 数据清洗和特征选择重要但是属于dirty work，我们用常见方法进行处理即可不要花费多余时间干复杂工作帮助不大

- 在训练过程中， `loss.detach().item()` 中
	- `.detach()` 是返回一个新的张量，这个新张量**与当前计算图彻底断开**
	- `.item()` 只能用于**仅包含一个元素**的张量（标量），它会将张量内的值提取为 Python 原生的 `float` 或 `int` 类型

- `optim.SGD()` 的部分参数
	- momentum --- 动量可以加速收敛，如果动量=0那么 v = gradient
		- $v_0=0$ --- 我们常说的动量其实指的是 v
		- $v_{k}=momentum*v_{k-1}+gradient$
		- $w = w - v_{k}*lr$
	- dampening --- 动量阻尼
		- $v_{k}=momentum*v_{k-1}+gradient*(1-dampening)$
		-  $w = w - v_{k}*lr$
	- weight_decay --- 权重衰减系数，相当于 $\lambda$
		- $gradient = gradient + w*weight\_decay$
		- 在SGD中，这就等价于在Loss后加上L2范数进行正则化
	- 一般来讲就会用 momentum、weight_decay 其它默认

- [optim.AdamW](AdamW.md)

- [schedule](schedule.md)


- 在 Training Loop 中，我们还引入了 tensorboard 记录训练结果方便查看

- 整体流程就是 特征选择 + 改结构 + 调参

- 训练中的术语
	- 一个 step 就是 一个 batch (相当于更新一次)
	- 每个epoch 算一次 valid 和 train 的 loss