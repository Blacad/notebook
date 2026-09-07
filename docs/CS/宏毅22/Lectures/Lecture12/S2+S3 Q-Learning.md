- 我们之前学习的 Critic 是计算 $V^\pi(s)$，但是在 Q-Learning 场景下的 Critic 直接计算 $Q^\pi(s,a)$

- 先前Critic两种算 $V^\pi$ 的方法
	- MC
		- $V^\pi \rightarrow G_t$
	- TD
		- $V^\pi(s_t) \rightarrow \gamma V^\pi(s_{t+1}) + r$
	- 我们还知道第三种算的方式 GAE 它是结合 MC 和 TD 的方式
		- GAE 就是权衡了 MC 和 TD 对于未来预期的使用情况，MC是使用大量的未来样本信息，而 TD 是指采用本步样本信息，GAE就是做了平衡由 $\lambda$ 控制采用多少未来信息


- 第二种 Critic 计算 $Q^\pi(s,a)$
	- $Q^\pi(s_t,a_t) = r_t + Q^\pi(s_{t+1},\pi(s_{t+1}))$
	- [训练方式](../../Slides/Lecture12/QLearning.pdf#page=14)
	- 通过不同探索方式解决采样固定问题
		- [不同探索方式](../../Slides/Lecture12/QLearning.pdf#page=15)
		- 这个探索方式在CS188中介绍更多，其中最有趣的我认为是 UCB1
	- Replay Buffer
		- 存储 $\pi$ 与环境互动的结果，每个样本都只是一个 experience 即 $s_t, a_t, r_t, s_{t+1}$
		- [如图](../../Slides/Lecture12/QLearning.pdf#page=16)

	- [经典实现算法](../../Slides/Lecture12/QLearning.pdf#page=18)


- Tips for Q-Learning
	- Double DQN
		- 问题 --- Q值通常被高估
		- 经典算法更新
			- $Q^\pi(s_t,a_t) \leftarrow r_t + max Q(s_{t+1},\pi(s_{t+1}))$
			- 如果后续的 Q 被高估了，也会导致前面的 Q 高估，非常容易放大高估链条
		- Double DQN 修改
			- $Q^\pi(s_t,a_t) \leftarrow r_t + Q'(s_{t+1},arg max Q(s_{t+1},a))$
			- Q 是 Target Network(会更新的)，Q‘ 是不更新的那个
			- 修正后，即是 Q 高估了，但是只要 Q‘ 没有高估就还好，缓解了高估链条
	- Dueling DQN
		- [Dueling DQN修改](../../Slides/Lecture12/QLearning.pdf#page=23)
			- 上面是正常 Q-Learning 的架构
			- 下面是 Dueling DQN 的架构
		- 为了防止 Dueling DQN 在训练过程中退化
			- 会将 A 进行 Normalize 再相加
			- [如图](../../Slides/Lecture12/QLearning.pdf#page=25)

	- Prioritized Reply
		- 在之前训练中TD误差较大的数据有更高的被采样的概率
		- 训练过程也有修改，可以看看论文如下
		- [如图](../../Slides/Lecture12/QLearning.pdf#page=28)
	
	- Multi-step
		- Buffer中的每个样本变长，即由多个 experience构成
		- [如图](../../Slides/Lecture12/QLearning.pdf#page=29)

	- Noisy Net
		- 之前我们是对行为采样，即不同的探索方式 比如概率贪心
		- Noisy Net 直接在参数中注入 noise 进而实现随机性
			- 在每个episode开始时将噪声注入Q函数的参数中，每个 episode 是同一个 noise，下一个 episode 再采样噪声注入

	- Distributional Q-function
		- 原先的 $Q(s,a)$ 只是在 状态 s 下 执行动作 a 的期望值，它不能反应分布
		- 该方法输出 $Q(s,a)$ 分布，将 action分布 分成多个 bin，分析 action 落入不同bin的概率进而得到分布

	- Rainbow 把上述诸多方法进行综合


