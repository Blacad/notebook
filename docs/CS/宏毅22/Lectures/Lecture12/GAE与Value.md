***GAE与Value的论证用到了麦克劳林级数以及无穷级数的收敛等数学工具，更详细的从数学严谨上证明了GAE与TD和Gt的关系，非常有启发***，还有一个重要的提醒，根据上述推导收敛性，$|\lambda|<1$ 是收敛条件 


因为 GAE 本身就是一串 TD error 的指数加权和，而每个 TD error 展开后，会自然整理成不同长度的 n-step return 的加权平均。

定义：

$$\hat A_t^{GAE} = \delta_t+\gamma\lambda\delta_{t+1} +(\gamma\lambda)^2\delta_{t+2}+\cdots$$

其中：

$$\delta_t=r_t+\gamma V_{t+1}-V_t$$

于是 value target：

$$V_t^{target} = V_t+\hat A_t^{GAE}$$

先把前几项展开：

$$V_t^{target} = V_t + (r_t+\gamma V_{t+1}-V_t) + \gamma\lambda(r_{t+1}+\gamma V_{t+2}-V_{t+1}) +\cdots$$

消掉 $$V_t$$

$$= r_t+\gamma V_{t+1} + \gamma\lambda r_{t+1} + \gamma^2\lambda V_{t+2} - \gamma\lambda V_{t+1} +\cdots$$

整理 $$V_{t+1}$$

$$= r_t + \gamma(1-\lambda)V_{t+1} + \gamma\lambda r_{t+1} + \gamma^2\lambda V_{t+2} +\cdots$$

继续把下一项也展开后，会得到：

$$V_t^{target} = r_t + \gamma(1-\lambda)V_{t+1} + \gamma\lambda r_{t+1} + \gamma^2\lambda(1-\lambda)V_{t+2} + \gamma^2\lambda^2 r_{t+2} +\cdots$$

现在定义 n-step return：
$$G_t^{(1)} = r_t+\gamma V_{t+1} $$
$$G_t^{(2)} = r_t+\gamma r_{t+1}+\gamma^2V_{t+2}$$
$$G_t^{(3)} = r_t+\gamma r_{t+1} +\gamma^2r_{t+2} +\gamma^3V_{t+3}$$

那么考虑：

$$(1-\lambda)\left[ G_t^{(1)} +\lambda G_t^{(2)} +\lambda^2G_t^{(3)} +\cdots \right]$$

展开第一项 reward $r_t$ 的系数： ---- ***麦克劳林级数与无穷级数*** 收敛条件是 $|\lambda|<1$

$$(1-\lambda)(1+\lambda+\lambda^2+\cdots)=1$$

所以得到：

$r_t$

对于 $r_{t+1}$，它从 $G^{(2)}$ 开始出现：

$$\gamma(1-\lambda) (\lambda+\lambda^2+\cdots) = \gamma\lambda$$

因此得到：

$$\gamma\lambda r_{t+1}$$

对于 $r_{t+2}$：

$$\gamma^2(1-\lambda) (\lambda^2+\lambda^3+\cdots) = \gamma^2\lambda^2$$

因此：

$$\gamma^2\lambda^2r_{t+2}$$

而 bootstrap value 项则分别是：

$$\gamma(1-\lambda)V_{t+1}$$
$$\gamma^2\lambda(1-\lambda)V_{t+2} $$
$$\gamma^3\lambda^2(1-\lambda)V_{t+3}$$

这与前面从 GAE 展开得到的形式完全一致。

所以：

$$\boxed{ V_t+\hat A_t^{GAE} = (1-\lambda) \sum_{n=1}^{\infty} \lambda^{n-1}G_t^{(n)} }$$

这就是所谓的 $$\lambda-return$$

直觉上也很好理解：

- 1-step return 权重最大：
    

$$1-\lambda$$

- 2-step return 权重：
    

$$(1-\lambda)\lambda$$

- 3-step return 权重：
    

$$(1-\lambda)\lambda^2$$

依次递减。

而且这些权重之和：

$$(1-\lambda) \sum_{n=0}^{\infty}\lambda^n =1$$

所以它确实是多个 n-step return 的一个加权平均。

最核心的一句话是：

GAE 是 TD error 的指数加权和⟺GAE+Vt 是 n-step return 的指数加权平均$$\boxed{ \text{GAE 是 TD error 的指数加权和} \Longleftrightarrow \text{GAE}+V_t \text{ 是 n-step return 的指数加权平均} }$$

两者只是同一个东西的两种展开方式