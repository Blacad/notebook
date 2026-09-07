
- 自适应学习率 --- 给每个参数不同的学习率

- 训练卡住了 不等于 Small Gradient
	- 即使在 gradient 还很大的时候 训练就卡住了 --- 这和学习率有很大的关系
	- 学习率太大参数震荡，loss会很大；学习率太小参数变化过小在平滑的地带，loss不能继续下降
	- 因此，我们需要为各个参数客制化学习率

- [参数的自适应](../../Slides/Lecture2/optimizer_v4P4.pdf#page=5-10)
	- $\theta_i^{t+1}=\theta_i^{t}-\frac{\eta}{\sigma_i^{t}}g_i^t$
	- $\sigma_i^{t}$ 有不同的算法
		- Root Mean Square
			- 公式 $\sigma_i^{t}=\sqrt{\sum_{i=0}^{t}({g_{i}^{t}})^2/(t+1)}$
			- 用于 Adagrad
			- 可以看到 gradient 小时，$\sigma$ 就小，等价于调大学习率；gradient 大时，$\sigma$ 就大，等价于调小学习率
		- RMSProp
			- 公式 $\sigma_i^{t}=\sqrt{\alpha (\sigma_i^{t-1})^2+(1-\alpha)(g_{i}^{t})^2}$
			- 相比与 Adagrad 有所改进，近期梯度影响更大，过去梯度影响较小(类似于TD)

	- 如今最常用的优化策略 --- [Adam = RMSProp + Momentum](../../Slides/Lecture2/optimizer_v4P4.pdf#page=11)
		- [计算偏差校正的一阶矩估计的原因](计算偏差校正的一阶矩估计的原因.md) --- 我们引入 m 和 v 的原因就是要考虑之前，假设是之前有梯度，但是初始化为0就是巨大的误差，因此需要此方法消除偏差

- LR Scheduling
	- 自适应LR也有[问题](../../Slides/Lecture2/optimizer_v4P4.pdf#page=12)
		- 这是因为之前在该方向的梯度很长一段时间都很小，因此导致学习率被调大
	- [LR Scheduling](../../Slides/Lecture2/optimizer_v4P4.pdf#page=13-16)能一定程度上缓解该问题
		- 将 $\eta$ 变成时间相关的，即 $\eta^t$
			- LR Decay
			- Warm up
				- 所谓的 warm-up 就是先让 $\eta^t$ 随时间步增大，到达 warm-up 设定的 step 后再进行 decay
			- 先warm-up类似于$\eta_t = \eta_{\max}\frac{t}{T_{\text{warmup}}}$，再 decay  $\eta_t = \eta_{\max} \cdot \text{Decay}(t)$。这里 $\eta_{max}$ 就是你在 optimizer中指定的学习率