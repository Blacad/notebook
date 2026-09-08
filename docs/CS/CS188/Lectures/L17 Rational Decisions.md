
- Utilities 效用
	- 效用 是从世界状态（结果）到实数的函数，描述了代理的偏好
	- 在给定知识下最大化期望效用是理性agent的主要目标

- Utility magnitudes 效用量级
	- 在minimax中，效用的序数排序(大小关系)会决定agent的行动，量级(大多少)是不影响agent行动的
	- 但是在 expectimax 中，量级会对agent的行动产生影响

- Preference 偏好
	- 表示
		- $A>B$ 更偏好 A
		- $A \sim B$ 同样偏好 A 与 B
	- 理性偏好公理
		- 可排序、传递性、连续性、可替代性、单调性
		-  所有理性的偏好都应该满足上述公理，而满足上述公理的偏好可以用 效用函数 来表示

- Money 
	- 想象买彩票的场景 L = [p, $X; 1-p, $Y]
	- 期望货币价值 EMV(L)=pX+(1-p)Y
	- U(L)=pU(\$X)+(1-p)U(\$Y)
	- 通常 U(L) < U(EMV(L))，这被称为风险厌恶 risk-averse
	- 等价肯定certainty equivalent CE(L) ~ L
	- 保险费insurance premium EMV(L) - CE(L)
	- 如果 保险费=0，就是 risk-neutral风险中性


- Utilities of Sequences
	- stationary preferences 静态偏好
		- 静态偏好描述： $[a_1,a_2,...]>[b_1,b_2,...] \Leftrightarrow [c,a_1,a_2,...]>[c,b_1,b_2,...]$
		- 基于上述描述，有一种方式定义效用，加性折扣效用 additive discounted utility：
			- $U([r_0,r_1,...])=r_0+\gamma r_1 +\gamma^2 r_2+....$  其中，折扣系数discount factor $\gamma \in (0,1]$ 


- Decision Networks
	- 带有 utility 和 action 节点的 BNs
	- 节点形状
		- 椭圆是 chance node 就是原先BNs的节点
		- 矩形是 action node
		- 棱形是 utility node
	- 例子
		- 无证据![DNExample.png](../assets/DNExample.png)
		- 有证据![DNExample2.png](../assets/DNExample2.png)


- Value of Information
	- 计算获取证据的价值
	- 例子
		- ![VOFExample.png](../assets/VOFExample.png)
	- VPI 特点
		- 非负性
		- 次可加
			- $VPI(Ej,Ek|e) \le VPI(Ej|e)+VPI(Ek|e)$
		- 顺序无关
			- VPI(Ej,Ek|e)=VPI(Ej|e)+VPI(Ek|e,Ej)=VPI(Ek|e)+VPI(Ej|e,Ek)