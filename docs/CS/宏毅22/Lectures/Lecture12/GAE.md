# PPO / Advantage / Value 简要总结

## 1. PPO 的损失说明

PPO 核心是策略损失：

$$  
L_{\text{clip}} = \mathbb E_t \left[ \min \left( r_t(\theta)\hat A_t, \operatorname{clip}(r_t(\theta),1-\epsilon,1+\epsilon)\hat A_t \right) \right]  
$$

  

其中：

$$  
r_t(\theta) = \frac{\pi_\theta(a_t|s_t)} {\pi_{\text{old}}(a_t|s_t)}  
$$

  

含义：

- \hat A_t>0：提高该动作概率
    
- \hat A_t<0：降低该动作概率
    
- clip：当策略沿正确方向变化过大时停止继续推动
    

完整 PPO Loss 通常为：

$$  
\mathcal L = -L_{\text{clip}} + c_vL_V - c_eH(\pi)  
$$

  

其中：

- L_{\text{clip}}：Actor 损失
    
- L_V：Critic 的 Value Loss
    
- H(\pi)：Entropy，鼓励探索
    

---

## 2. 各种类型的优势说明
理论依据，标准马尔可夫决策过程

真实 Advantage 定义：

$$  
A^\pi(s_t,a_t) = Q^\pi(s_t,a_t)-V^\pi(s_t)  
$$

  

实际训练中需要估计 Q

### TD Advantage

$$  
\hat A_t^{TD} = r_t+\gamma V(s_{t+1})-V(s_t)  
$$

  

即：

$$  
\hat Q_t = r_t+\gamma V(s_{t+1})  
$$

  

特点：

- 1-step
    
- 强 bootstrap
    
- 低 variance
    
- 较高 bias
    

---

### Monte Carlo Advantage

$$  
\hat A_t^{MC} = G_t-V(s_t)  
$$

  

其中：

$$  
G_t = r_t+\gamma r_{t+1}+\gamma^2r_{t+2}+\cdots  
$$

  

特点：

- 使用完整 trajectory
    
- 弱 bootstrap
    
- 高 variance
    
- 较低 bias
    

---

### GAE Advantage

$$  
\hat A_t^{GAE} = \delta_t + \gamma\lambda\delta_{t+1} + (\gamma\lambda)^2\delta_{t+2} +\cdots  
$$

  

其中：

$$  
\delta_t = r_t+\gamma V(s_{t+1})-V(s_t)  
$$

  

也可理解为：

$$  
\hat A_t^{GAE} = G_t^\lambda-V(s_t)  
$$

  

因此：

$$  
\boxed{ TD \leftrightarrow GAE \leftrightarrow MC }  
$$

  

本质区别是对：

$$  
Q^\pi(s_t,a_t)  
$$

  

的估计时间尺度不同

---

## 3. 各种情形下的 Value 以及 Critic 学习目标

理论依据 Bellman 方程

真实 Value：

$$  
V^\pi(s) = \mathbb E_{a\sim\pi}[Q^\pi(s,a)] = \mathbb E[G_t|s_t=s]  
$$

  

即：

> 当前状态下，按照策略继续执行时的平均未来 Return。

### TD(0)

Value Target：

$$  
V_t^{target} = r_t+\gamma V_{\text{old}}(s_{t+1})  
$$

  

Critic 更新：

$$  
V_{\text{new}}(s_t) \rightarrow r_t+\gamma V_{\text{old}}(s_{t+1})  
$$

  

它是对：

$$  
Q(s_t,a_t)  
$$

  

的单步估计

---

### Monte Carlo

Value Target：

$$  
V_t^{target} = G_t  
$$

  

Critic 更新：

$$  
V_{\text{new}}(s_t) \rightarrow G_t  
$$

  

其中 G_t 是该状态下一条实际 trajectory 的 Return 样本

---

### GAE

Value Target：

$$  
V_t^{target} = V_{\text{old}}(s_t) + \hat A_t^{GAE}  
$$

  

即：

$$  
V_t^{target} = G_t^\lambda  
$$

  

Critic 更新：

$$  
V_{\text{new}}(s_t) \rightarrow G_t^\lambda  
$$

  

其中：

$$  
G_t^\lambda  
$$

  

是多个 n-step Return 的加权组合。

因此：

$$  
\lambda=0 \Rightarrow V_t^{target} = r_t+\gamma V_{\text{old}}(s_{t+1})  
$$

  

$$  
\lambda=1 \Rightarrow V_t^{target} = G_t  
$$

  

所以：

$$  
\boxed{ \lambda \text{ 控制 Critic Target 在 bootstrap Value 与真实 trajectory Return 之间的平衡} }  
$$

  

最终 Critic 通过大量样本学习