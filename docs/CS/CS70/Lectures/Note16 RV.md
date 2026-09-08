- [笔记](../Materials/Notes/16%20RV-variance%20and%20covariance.pdf)

- Function of RV
	- LOTUS: $E[f(X)] = \sum_{x}f(x)P [X = x].$

- Variance and Covariance
	-  Variance定义：For a r.v. X with expectation $E[X] = µ$, the variance of X is defined to be $Var(X) = E[(X − µ)^2]$
		- The square root $σ(X) :=\sqrt{Var(X)}$ is called the standard deviation of X
	- 定理：For a r.v. X with expectation $E[X] = \mu$, we have $Var(X)=E[X^2]-\mu^2$

- Variance Computation


- Sum of Independent Random Variables
	- 定理 ：For independent random variables X,Y , we have E[XY] = E[X]E[Y].
	- 定理：For independent random variables X,Y , we have Var(X +Y) = Var(X) +Var(Y).


- Covariance and Correlation
	- 表达式E[XY]−E[X]E[Y]是X和Y之间关联的度量，被称为协方差
	- 定义Covariance：$Cov(X,Y) = E[(X - \mu_X )(Y - \mu_Y )] = E[XY]-E[X]E[Y]$
		- 如果X，Y相互独立，那么Cov(X,Y) = 0。然而，逆命题不成立。
		- Cov(X,X) = Var(X)
		- Covariance is bilinear; i.e., for any collection of random variables $\{X_1,...,X_n\},\{Y_1,...,Y_m\}$ and fixed constants $\{a_1,\dots,a_n\},\{b_1,\dots,b_m\}$
			- $Cov(\sum_{i=1}^n a_{i}X_{i},\sum_{j=1}^{m} b_jY_j) = \sum_{i=1}^n \sum_{j=1}^{m} a_ib_jCov(X_i,Y_j).$
		- For general random variables X and Y, Var(X +Y) = Var(X) +Var(Y) +2Cov(X,Y).

	- 定义Correlation：Suppose X and Y are random variables with σ(X) > 0 and σ(Y) > 0. Then, the correlation of X and Y is defined as $Corr(X,Y) =\frac{Cov(X,Y)}{σ(X)σ(Y)}$
	- 定理：For any pair of random variables X and Y with σ(X) > 0 and σ(Y) > 0, −1 ≤ Corr(X,Y) ≤ +1.
		- Corr比Cov更有用，因为它的值总是介于 -1 与 1之间
		- proof 表明，如果 $Corr(X,Y) = \pm1$,则 $Y=aX+b$, a,b 是常数，这表明二者线性相关，一者确认另一者同时确定


