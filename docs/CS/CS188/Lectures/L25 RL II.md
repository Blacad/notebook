
- 本节主要讲述 RL 的基本概念

- RL 的构成和概念
	- RL的构成形式化 ---  [cs188-sp24-lec24.pdf](../Slides/cs188-sp24-lec24.pdf#page=12-13)
	- 可以理解为 缺失 R 和 T 信息的 MDPs，也可以理解为 ML + MDPs(缺信息)
	- RL的经典例子 --- Bandit 老虎机
	- Offline Planning --- MDPs
		- 智能体完全了解状态转移函数和奖励函数，以及他们需要预先计算在MDP编码的世界中最佳动作的所有信息，而无需实际采取任何动作
	- Online Planning --- RL
		- 代理对世界的奖励或转换没有先验知识（仍然表示为MDP）。在在线规划中，代理必须尝试探索，在此期间，它执行动作并接收反馈，反馈是它到达的后继状态和相应的奖励
		- 代理使用这种反馈通过称为强化学习的过程来估计一个最优策略，然后再利用这个估计的策略最大化奖励。
	- 一些术语
		- sample --- $(s,a,s',r)$ 称为一个 sample
		- episode --- 从 start state 一直行动直到 terminal state，这些样本集合被称为 episode
	- 两种类型的RL
		- model-based learning --- 基于模型的学习试图在探索过程中使用获得的样本来估计转移和奖励函数，然后再使用这些估计值通过值迭代或策略迭代解决MDP。
		- model-free learning --- 无模型学习则试图直接估计状态值或Q值，不使用任何内存来构建MDP中奖励和转移的模型。


- Model-Based Learning
	- 在基于模型的学习中，智能体通过记录进入每个Q状态（s,a）后到达每个状态s'的次数，生成状态转移函数$\hat{T}(s,a,s')$的近似 即：通过归一化它收集的计数——将每个观察到的元组（s,a,s'）的计数除以代理处于Q状态（s,a）的所有实例的计数之和
	- 我们可以让 agent 以初始策略 $\pi_{explore}$  去尝试几个 episodes，然后基于这些episodes的结果去估算 T 和 R --- [cs188-sp24-note24.pdf](../Notes/cs188-sp24-note24.pdf#page=2-3)
	- 当我们已经认为 agent 的 T 和 R 已经训练到位，就停止代理训练接着 使用当前的$\hat{T}$和$\hat{R}$模型生成策略$\pi_{exploit}$，并使用$\pi_{exploit}$ 来继续进行 MDP 过程使奖励最大化


- Model-Free Learning
	- 三种主要的Model-Free Learning
		- Direct Evaluation 直接评估
		- Temporal Difference Learning 时序差分学习
		- Q-Learning
	- 直接评估 和 时差学习 属于被动强化学习算法类别 passive RL
		- 在被动强化学习中，智能体被赋予一个要遵循的策略，并在产生 episodes 的过程中学习该策略下各状态的价值，这正是MDPs在T和R已知时进行策略评估(policy evaluation)所做的事情
	- Q学习属于第二类无模型学习算法，称为主动强化学习 active RL
		- 在此过程中，学习代理可以使用收到的反馈来迭代更新其策略，直到经过足够的探索后最终确定最优策略


	- Direct Evaluation
		- 直接评估所做的只是固定某些策略π，并让代理在遵循π的过程中产生几个 episodes
		- 随着代理通过这些阶段收集样本，它维护着从每个状态获得的总体效用计数以及访问每个状态的次数。在任何时刻，我们可以通过将来自状态s的总效用除以s被访问的次数来计算任何状态s的估计值。 --- [cs188-sp24-note24.pdf](../Notes/cs188-sp24-note24.pdf#page=4)
		- 尽管直接评估最终会学习到每个状态的状态值，但由于它浪费了关于状态之间转换的信息，它通常收敛得非常慢。

	- Temporal Difference Learning(TD)
		- 能在一定程度上缓解 DE 的问题
		- 在policy evaluation中，我们使用了由我们的固定策略和贝尔曼方程生成的方程组来确定该策略下各状态的价值
			- $V^{\pi}(s)=\sum_{s'}T(s,\pi(s),s')[R(s,\pi(s),s')+\gamma V^{\pi}(s')]$
		- TD学习试图回答如何在没有权重的情况下计算这个加权平均值的问题，巧妙地使用指数移动平均 exponential moving average 来实现
			- 我们首先初始化∀s，$V^π(s) = 0$
			- 在每一步，一个智能体从状态s采取一个动作π(s)，转移到状态s'，并接收奖励R(s,π(s),s')。我们可以通过将接收到的奖励与s'在π下的折现当前值相加来获得一个样本值：
				- $sample=R(s,\pi(s),s')+\gamma V^{\pi}(s')$
			- 遵循更新策略
				- $V^{\pi}_{k}(s)=(1-\alpha)V^{\pi}_{k-1}(s)+\alpha \cdot sample_{k}$
				- α 是一个参数，受限于 0 ≤ α ≤ 1，称为学习率，它指定了我们想要为 $V^{\pi}(s)$ 分配的权重，开始 $\alpha=1$ 然后逐渐趋向 0 
			- TD的优势
				- 给予较老、可能不太准确的样本更低的权重
				- 每个时间步都会学习，并会用到转移关系

	- Q-Learning
		- Direct Evaluation 和 TD 都只是评估 V 值，但是我们选择什么 action，是需要 Q值的，这就需要 Bellman 方程，因此 TD学习或直接评估通常与某些基于模型的学习方法结合使用，以获取T和R的估计，以便有效地更新学习代理遵循的策略
		- Q-learning的 更新策略
			- $Q_{k+1}(s,a)=\sum_{s'}T(s,a,s')[R(s,a,s')+\gamma max_{a'}Q_{k}(s',a')$
			- Q-value sample
				- $sample=R(s,a,s')+\gamma max_{a'}Q(s',a')$
			- Q-value 更新策略
				- $Q(s,a)=(1-\alpha)Q(s,a)+\alpha \cdot sample$

- Off-Policy Learning vs On-Policy Learning
	- 即使采取次优或随机动作，Q学习可以直接学习最优策略  --- off-policy learning
	- Direct Evaluation 和 TD 通过遵循策略来学习策略下的状态值，再通过其他技术确定最优策略 --- on-policy learning
	- On-policy 是学习“自己正在执行的策略”的价值；off-policy 是用一种行为策略采样，却学习另一种目标策略的价值

- Approximate Q-Learning
	- 近似Q学习试图通过学习少数几种一般情况来对此进行解释，并将其推广到许多类似的情形 --- Q-Learning 
	- 学习经验泛化的关键是基于特征的状态表示 the feature-based representation of states ，它将每个状态表示为一个称为特征向量的向量
	- 有了特征向量，我们可以把 V 和 Q 视作线形函数
		- $V(s)=\sum_{i=1}^n w_i \cdot f_{i}(s)=\overrightarrow{w} \cdot \overrightarrow{f}(s)$
		- $Q(s,a)=\sum_{i=1}^n w_i \cdot f_{i}(s,a)=\overrightarrow{w} \cdot \overrightarrow{f}(s,a)$
	- $difference=[R(s,a,s')+\gamma max_{a'}Q(s',a')]-Q(s,a)$
	- 更新策略
		- $w_i = w_i + \alpha \cdot difference \cdot f_i(s,a)$
	- 而不是为每个状态存储Q值，使用近似Q学习我们只需要存储单个权重向量，并且可以根据需要动态计算Q值