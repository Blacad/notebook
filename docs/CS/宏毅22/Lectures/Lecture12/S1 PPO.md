
- On-policy -> Off-policy
	- On-policy 需要用 $\pi_{\theta}$ 采样数据，然后训练更新 $\pi_{\theta}$
	- Off-policy 只需要用 原模型 $\pi_{ref}$ 采样数据，然后训练更新 $\pi_{\theta}$
	- 因此后者能省去非常多的采样数据的开销
	- Importance Sampling 重要性采样是 Off-policy 的重要理论基础
		- $E_{x \sim p}[f(x)]=\intop\limits f(x)p(x)dx = \intop\limits f(x)\frac{p(x)}{q(x)}q(x)dx=E_{x \sim q}[f(x)\frac{p(x)}{q(x)}]$
	- 但是二者的 $Var$ 不同
	- 因此，即使有 Importance Sampling 保证均值，但是由于 Var 不同，因此 $\pi_{\theta}$ 与 $\pi_{ref}$ 仍然不能差别太远(除非你能穷尽采样否则不一定逼近均值)
	- 将 ==Importance Sampling== 应用于 RL
		- $\nabla R = E_{\tau \sim p_{\theta}}[R(\tau)\nabla logp_{\theta}]$ -> $\nabla R = E_{\tau \sim p_{\theta'}}[\frac{p_{\theta}}{p_{\theta'}}R(\tau)\nabla logp_{\theta}]$
		- Gradient for update
			- $E_{\tau \sim p_{\theta}}[A^{\theta}(\tau)\nabla logp_{\theta}]$ -> $E_{\tau \sim p_{\theta'}}[\frac{p_{\theta}}{p_{\theta'}}A^{\theta'}(\tau)\nabla logp_{\theta}]$
	- $\nabla f(x)=f(x)\nabla logf(x)$ 很有意思的转换在我自己推的时候也发现过这样的问题
	- 由此根据上述的 gradient 反推目标函数
		- $J^{\theta'}(\theta)=E(\frac{p_{\theta}}{p_{\theta'}}A^{\theta'})$

- 上述所讲的仍然是 PO 即 policy optimization

- PPO 
	- 所谓 PPO 近端就是 让 $\pi_{ref}$ 与 $\pi_{\theta}$ 之间不要差距太远，因此 加上 KL divergence，我们约束的不是它们在参数上的差距而是约束它们行为的差距，即 outputs 的差距
	- $J^{\theta'}_{ppo}(\theta)=\sum \frac{p_{\theta}}{p_{\theta'}}A^{\theta'}-\beta KL(\theta,\theta')$
	- $\beta$ 是 Adaptive 的，当 $KL>KL_{max}$ 时 提高，反之降低

- PPO2
	- $J^{\theta'}_{ppo2}(\theta)=\sum min(clip(\frac{p_{\theta}}{p_{\theta'}},1-\epsilon,1+\epsilon)A^{\theta'},\frac{p_{\theta}}{p_{\theta'}}A^{\theta'})$
	- 可以不需要KL，这是因为 min+clip 也可以让 $\pi_{ref}$ 和 $\pi_{\theta}$ 相距别太远
	- 可以看到，如果 A > 0, 为了目标函数最大化，那么 $p_{\theta}$ 会上升，但是 ratio 最大不会超过 $1+\epsilon$ ，反之如果 A<0,  为了目标函数最大化，那么 $p_{\theta}$ 会下降，但是 ratio 最小不会超过 $1-\epsilon$
	- 但是如果 $\theta$ 与 $\theta'$ 已经相隔很远了，那么PPO2的方法并不能很好使得二者再靠近，但是 PPO 可以，实践上来讲，由于初始位置二者相隔很近，因此往往不会出现相隔很远的情况，而且相隔很远且不能改善的情况就两种：A>0且$\theta$显著高；A<0且$\theta$显著小，但是往往会让它们更慢的升或降


- 现代 PPO 中的 A 都是采用 GAE，形式上就是 TD+Discount Acc
	- [GAE与Value的数学分析](GAE与Value.md)
	- [GAE](./GAE.md)

- 上述过程中我们主要关注的都是 Actor 部分，但是还有 Critic 部分(Value function)，Critic部分损失就是拟合 $V^{target}$ 或者说 本次估计的 $Q(s,a)$
	- 最基本形式
		- $L_V(\phi) = \mathbb E_t \left[ \left( V_\phi(s_t)-V_t^{\text{target}} \right)^2 \right]$
		- 其中：$V_t^{\text{target}} = \hat A_t+V_{\text{old}}(s_t)$

	- 有些 PPO 实现有时也会对V进行 clip 即：
		- $V_t^{\text{clip}} = V_{\text{old}}(s_t) + \operatorname{clip} \left( V_\phi(s_t)-V_{\text{old}}(s_t), -\epsilon_v,\epsilon_v \right)$
		- $L_V = \sum \max \left[ (V_\phi(s_t)-V_t^{target})^2, (V_t^{\text{clip}}-V_t^{target})^2 \right]$

- 同时为了防止 policy 过早变得过于确定，PPO 经常加入 entropy：
	- $H(\pi_\theta(\cdot|s_t)) = -\sum_a \pi_\theta(a|s_t) \log\pi_\theta(a|s_t)$
	- entropy 越大，策略越随机

