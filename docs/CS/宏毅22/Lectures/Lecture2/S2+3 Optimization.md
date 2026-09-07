
- 优化器的真实应用
	- BERT、Transformer、Tacotron、 Big-GAN、MEMO --- Adam
	- YOLO、maskR-CNN、ResNet--- SGD with moment

- Adam 是 14 年提出的，为什么现在大家还在用，这些年没有新东西吗？
	- 因为 Adam 和 SGD with moment 足够好，近些年工作比较平庸
	- 尝试结合？
		- SWATS 2017年 --- 先用Adam再用SGDM(什么时候切换依然是问题)
	- 改善 Adam？
		- AMSGrad 2018年 --- Adam中真正梯度大的step对整体更新的影响可能小于众多的梯度小的step的影响，因此它让 $\hat{v_t}=max(\hat{v_{t-1}},v_t)$，这样能让之后的step更新时知道前面更新过大的梯度，使大梯度的影响扩大。但是这样依然有问题，因为这样更新学习率就变成单调递减的学习率，可能会迅速下降使得之后几乎不动
		- AdaBound 2019年 --- 使用 clip 函数，解决 Adam 中不同参数学习率要不然过大要不然过小的问题(依据实践中的发现)。但是它给出的clip函数的上下界基于经验得到，因此非常的没有道理


	- 改善 SGDM？--- 相当于做 LR schedule，将 $\eta$ 视作时间步相关的，即 $\eta^t =f(t)$
		- Cyclical LR 2017年 --- SGDM无法确定很好的LR，因此它决定使用$f(t)$规律变化LR的大小，通过 LR range test 找到合适的LR
		- SGDR 2017年 --- 思路和上文相同，但是这里用的规律性变化是 cosine annealing
		- one-cycle LR 2017年 --- warm-up + annealing + fine-tuning

- Adam 需要 warmup 吗？
	- 当然需要，warm-up 的作用是：训练初期先使用较小学习率，再逐渐升到目标学习率，避免参数更新过猛导致训练不稳定或发散。
	- RAdam 2020年 --- 做Adam的warm-up
	- Lookahead 2019年 --- k step forward and 1 step back
		- slow weight and fast weight
		- [Lookahead](../../Slides/Lecture2/OptimizationS2+3.pdf#page=43-44)

- 改善 Momentum？
	- NAG 1983年 ---- 看到未来防止过度动量
		- [NAG](../../Slides/Lecture2/OptimizationS2+3.pdf#page=47-48)
	- Nadam 2016年 --- NAG + Adam 
		- [Optimization](../../Slides/Lecture2/OptimizationS2+3.pdf#page=49)

- AdamW --- 解决 L2 正则化的问题

- 一些其它优化
	- Shuffling
	- Dropout
	- Gradient noise
	- Curriculum learning
	- normalization
	- regularization

- 建议
	- CV --- SGDM
	- NLP、Speech synthesis、GAN、RL --- Adam

