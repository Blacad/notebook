- 动机
	- 你的模型需要应对人类欺骗，比如 游戏中必然有模型会检测不雅用语，但是有些隐晦的不雅用语仍然能绕过模型


- 攻击类型
	- Non-targeted
		- 让模型的输出错误就好了
	- Targeted
		- 让模型的输出按照预设错误，比如 将科技类误分为体育类

- [如何攻击](../../Slides/Lecture10/attack_v3.pdf#page=11)
	- Non-targeted：单纯找到让原始损失大的noise，并保证 noise 足够小人不可分辨
	- Targeted：相比 Non-targeted，不仅让原始损失大而且让新目标损失小
	- noise 足够小的[方法](../../Slides/Lecture10/attack_v3.pdf#page=12)
		- L2-norm
		- L-infinity-norm --- 往往更好
	- 具体[训练方法](../../Slides/Lecture10/attack_v3.pdf#page=14)
		- 梯度更新输入而不是参数
	- [FGSM](../../Slides/Lecture10/attack_v3.pdf#page=16)
		- 只 update 一次输入，同时取 sign且学习率直接用 $\epsilon$
	- [Iterative FGSM](../../Slides/Lecture10/attack_v3.pdf#page=17)
		- 相比 FGSM，增添 noise 约束在$[-\epsilon,\epsilon]$，并且多跑几次update(图中的红杠去掉)，学习率自己指定通常小于 $\epsilon$

- White Box vs Black Box
	- 不好意思上述的攻击方式需要知道模型参数，是 White Box Attack


- [Black Box Attack](../../Slides/Lecture10/attack_v3.pdf#page=19)
	- 用别人公开的训练资料在本地训练一个模型，然后通过White Box Attack 实现对本地模型的成功攻击，再通过本地的攻击成功案例去攻击别人的模型
	- 如果你不知道别人的训练资料怎么办 --- 蒸馏
		- 访问别人的模型，得到输入输出对，依此构成训练数据集
		- 基于此，在 A model 上的攻击 甚至能 成功攻击 B model

- One pixel attack
	- 只改变一个像素实现成功攻击，[如图](../../Slides/Lecture10/attack_v3.pdf#page=22)

- Universal Adversarial Attack 是可能的，即 找到通用的 noise 加入输入就可以实现对目标模型的成功攻击而不用为每个输入专门找noise去攻击目标模型，[如图](../../Slides/Lecture10/attack_v3.pdf#page=23)

- Speech 和 Language 也是一样都可以被攻击


- Adversarial Reprogramming
	- [做法](../../Slides/Lecture10/attack_v3.pdf#page=28)
	- 训练一个数方块的模型，不同的方块数量对应不同的类，将方块图片周围添加 noise后给模型，模型输出类结果而不是方块数量，貌似模型学会了分类实则只是学会了方块数量信息

- Backdoor in Model(训练阶段)
	- 在训练数据中进行人不可见的投毒(noise)，使得模型在某些输入时会出错，[如图](../../Slides/Lecture10/attack_v3.pdf#page=29) 



- Defense
	- Passive Defense 被动防御
		- 在模型前面加上 Filter 模块，将输入进行[模糊化](../../Slides/Lecture10/attack_v3.pdf#page=31-32)
		- [其它方法](../../Slides/Lecture10/attack_v3.pdf#page=33)
		- 被动防御被别人得知就会很容易被攻破，比如攻击者知道你用 Filter，那么我们就把 Filter 和 原模型看成一个整体，同样用上述方式攻破该整体就行了
	- Proactive Defense 主动防御
		- Adversarial Training 对抗训练
			- 训练一个对 adversarial attack 具有鲁棒性的模型
		- [做法](../../Slides/Lecture10/attack_v3.pdf#page=35)
			- 本质上是数据增强
		- [Adversarial Training for Free](https://arxiv.org/abs/1904.12843)

