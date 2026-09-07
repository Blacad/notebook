
- ROC AUC（**Receiver Operating Characteristic - Area Under the Curve**）是二分类任务中最常用的评价指标之一，它衡量的是**模型区分正负样本的能力（ranking ability）**，而不是某一个固定阈值下的分类准确率

- ROC 曲线
	- 假设一个二分类模型输出的是一个分数（例如 sigmoid 输出的概率），我们可以设置不同的分类阈值，不同阈值会得到不同的分类结果，因此也会得到不同的：
		- True Positive Rate（TPR）
		- False Positive Rate（FPR）
	- ROC 曲线 横轴是 FPR，纵轴是 TPR，随着阈值变化，TPR和FPR也会变化，将点连接起来就是 ROC Curve

- AUC
	- AUC 是 ROC 曲线下的面积，面积越大越好，面积范围是 0-1
	- 1 是完美分类，0.9+是优秀，0.8+较好，0.7+一般，0.5随机猜测
	- 概率意义
		- 随机选择一个正样本和负样本，模型将正样本排在负样本前面的概率
	- 统计学意义
		- Mann–Whitney U 检验统计量的归一化形式
		- 如果只是计算 AUC，往往会使用 Mann–Whitney U 检验统计量的方式，它的算法速度更快