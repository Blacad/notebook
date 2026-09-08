- 本节主题 --- model-based classification with Naive Bayes 用朴素贝叶斯网络进行分类的应用和训练


- 之前我们一直在讲如何利用 model 去实现最优决策，ML 是讲如何从数据或经验中得到一个model
	- 学习参数(e.g. probabilities)
	- 学习结构(e.g. BNs)
	- 学习隐藏状态 (e.g. clustering, neural nets)

- Classification
	- 数据集：每个数据点x都与某个标签（即类别）y相关联
	- 分类目标：给定输入x，编写一个算法来预测标签y
	- 过程工作流程
		- 从输入中提取特征：描述每个x的属性，并希望有助于分类
		- 在特征上运行某些机器学习算法：比如朴素贝叶斯
		- 输出一个预测的标签y

- Training
	- 机器学习算法从数据中学习特征和标签之间的模式
	- 你不必自己推理数据
	- 你被提供了训练数据：大量的示例数据点和它们的实际标签

- 基于模型的方案 Model-based approach
	- 建立一个模型（例如贝叶斯网络），其中输出标签和输入特征都是随机变量
	- 实例化任何观察到的特征
	- 查询在特征条件下的标签分布

- Naive Bayes Model
	- 它的结构就是 两层树结构
	- 我们需要学习 root 的概率分布以及各个 leave 在给定root下的条件概率分布 i.e. CPTs
	- 执行训练：
		- 使用训练数据集来估计概率表
		- 估计 P[Y] = 每个标签出现的频率是多少？
		- 估计 P(F|Y) = 标签如何影响特征？
	- 执行分类：
		- 实例化所有特征：你知道输入特征，所以它们是你的证据
		- 查询 P(Y|f1, f2 ..., fn)
		- 给定所有输入特征的标签概率，使用推理算法（例如变量消除）来计算这个概率 
	- Naive Bayes Inference 示例
		- ![NaiveBayesInfe.png](../assets/NaiveBayesInfe.png)

	- Bag-of-words Naive Bayes
		- 在区分垃圾邮件与否是，一般我们会提取邮件的特征作为 leave，而标签作为 root
		- 但是 Bag-of-words中，我们将完整的邮件内容全部考虑进去，即将每个位置的词都作为一个特征 leave，标签依旧是root
		- 两者的计算方式是一致的，只是特征上不同


- Parameter Estimation 参数估计
	- 利用 Maximum Likelihood，即选择一个参数设置 $\theta$ 使得 $P(observation|\theta)$ 最大化
	- 最大似然估计
		- $\theta_{ML}= argmax_\theta P(X|\theta)=argmax_\theta \prod_i P_\theta(x_i)$
		- $P_{ML}(x)=\frac{count(x)}{total \ samples}$
	- 另一种视角
		- $\theta_{MAP}=argmax_\theta P(\theta|X)=argmax_\theta P(X|\theta)P(\theta)/P(X)$


- Overfitting 过拟合
	- 过拟合会对没见过的sample表现非常差，也会因小噪声而导致不正确的结果
	- 如果只针对训练数据疯狂训练就容易产生过拟合

	- 为了更好的泛化性来防止过拟合，我们需要 smoothing
		- Laplace Smoothing
			- 将所有可能出现的情况都加 1 or k (这个可以指定)，当然也可以指定只加 $\alpha$ 这些情况
			- 如下图所示为Laplace(+1)的例子![LaplaceEstimate.png](../assets/LaplaceEstimate.png)
	- 同时我们的数据往往分成 三份
		- Training、Held-out Data、Test Data
		- 其中，Held-out Data 是验证集，不作为训练，而是作为 Tuning
			- 根据 smoothing，我们会有超参数 k(具体加多少)，$\alpha$(具体加哪些)，而Tuning，就是在 held-out Data上调整超参数