
- 简单来说 GNN = Graph + NN
	- 一个节点可以从其邻居那里学习结构
	- [GNN的基本介绍](../../Slides/Lecture4/GNNS3+4.pdf#page=20)
		- Spatial-based GNN --- GAT
		- Spectral-based GNN --- GCN

- Spatial-based GNN
	- [基本思路](../../Slides/Lecture4/GNNS3+4.pdf#page=33)
	- [NN4G模型](../../Slides/Lecture4/GNNS3+4.pdf#page=34-35)
	- [DCNN模型](../../Slides/Lecture4/GNNS3+4.pdf#page=36-37)
	- [DGC模型](../../Slides/Lecture4/GNNS3+4.pdf#page=38)
	- [MoNET模型](../../Slides/Lecture4/GNNS3+4.pdf#page=39)
		- 重点是将图中的边赋予权重
	- [GraphSAGE](../../Slides/Lecture4/GNNS3+4.pdf#page=41)
		- 重点研究如何将邻居计算的结果聚合
	- [GAT](../../Slides/Lecture4/GNNS3+4.pdf#page=44)
		- 让模型通过attention去学图中边的权重
	- [GIN](../../Slides/Lecture4/GNNS3+4.pdf#page=48)
		- 对该类GNN的理论分析，提出有效的GNN的更新策略并且提出求和更好
 
- Spectral-based GNN --- 这个理论太复杂了
	- [理论基础](../../Slides/Lecture4/GNNS3+4.pdf#page=60-81)
	- [ChebNet](../../Slides/Lecture4/GNNS3+4.pdf#page=81-87)
	- [GCN](../../Slides/Lecture4/GNNS3+4.pdf#page=88)