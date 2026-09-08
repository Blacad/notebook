- 本节主题 --- 线性和逻辑回归的应用和训练


- Linear Regression 线性回归
	- 输入是 feature values
	- 每个feature 有 weight
	- 它们的和是 prediction
	- $h_w = \sum_i w_i \cdot f_i(x)=w \cdot f(x)$
	- 输出就是 $h_w$
	- 在实践中往往使用$h_w =w \cdot f(x) + w_0$ ，防止有时计算出 $h_w$ 为 0


- Weight Update
	- 图示(默认列向量)
		- ![weightupdate.png](../assets/weightupdate.png)
		- $Loss = \sum_i (f(x^i)^{T}w - y_i)^2/2$
		- 目标 $argmin_w |Xw - y|^2/2$ 
		- $\nabla_w(y-Xw)^T(y-Xw)/2=0$ --> $-X^Ty+X^TXw=0$
		- 因此 $\widehat w = (X^TX)^{-1}X^Ty$


- 回到 线性分类器的部分
	- 线性分类器最大的局限就是如果数据不可分那么权限更新会变得混乱
	- 线性分类器中，我们采用的是决定性决策
		- $H(z)$ 表示 z 分数的情况是class +1 的概率
		- 那么 $H(z)=1 \ when \ z >0 \ else \ 0$ 
	- 决定性决策导致不可分的数据无法使用线性分类器进行分类，为此我们引入 概率决策
	- Non-Separable Case: Probabilistic Decision 不可分情况：概率决策
		- $H(z)=sigmoid(z)$
		- 这样推理的策略就变成 likelihood 的了
			- 这就是逻辑回归 Logical Regression  ![LogisticRegression.png](../assets/LogisticRegression.png)


	- Multi-class Logistic Regression
		- 与线性分类器的操作一样，每个类都有一个分数 z
		- 那么各个类的概率分别是 $\frac{e^{z_i}}{e^{z_1}+...}$
		- 推理策略为
			- 多类逻辑回归![MultiClassLR.png](../assets/MultiClassLR.png)

	- 二分类是多分类的特例
		- softmax $\frac{e^{c1}}{e^{c1}+e^{c2}}$ 如果 c1 类的总是0，那么成为 sigmoid $\frac{1}{1+e^{c2}}$


	- Optimization 优化
		- 更新逻辑回归的权重![OptimizationLR.png](../assets/OptimizationLR.png)
			- 梯度上升 Gradient Ascent 更新权重的方式 
			- $\alpha$ 是学习率