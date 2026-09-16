- 实现 Logic and Classical Planning

- 本项目绝对能够深化 你对 Propositional Logic 的认识

- 三个 prerequisite
	- The Expr Class --- 表达式类用于构建 命题逻辑的句子
		- 注意使用 `conjoin` 和 `disjoin`
		-  其子类 `PropSymbolExpr` 更适合创建带下标的变量创建
	-  Prop Symbol Names --- 概率变量命名方式注意
	- SAT Solver --- 使用 `pycosat` 库去当SAT Solver(DPLL) 输出 可能世界

## 总结
------ basic logic

- Q1 --- 理解 命题逻辑学基本工具的实现和用法
	- sentence 1-3 帮助了解 `Expr` 和 `PropSymbolExpr` 如何使用
	- `findModelUnderstandingCheck()` 帮助了解 `SAT` 解析器如何使用或者说 `Expr` 如何能被 `SAT` 处理
	- `entails` 帮助理解 `findModel` 使用
	- `pltrueinverse` 帮助理解 `pl_true` 使用 即 `model_check`

- Q2 --- 命题逻辑学的基本练习
	- least one \ most one \ exact one
------------ find path\eat food (problem全知，知道全局map+自己的location完成task)
- Q3 --- 命题逻辑实际应用的基础，详细讲是实现各种公理逻辑并完成位置check
	- `pacmanSuccessorAxiomSingle` 实现继承公理
	- `pacphysicsAxioms` 将各种公理(包括继承公理)与已知事实相结合 得到最核心的 Base-KB(其中每个clause都是公理或已知事实都是绝对正确的)
	- `checkLocationSatisfiability` 实现位置check --- `KB^action^position`
		- 这样能得到合理的 `action`
	- 本题比较麻烦的地方在于去除冗余或填充不足，一开始没太好理解每个函数的分工，导致经常出现变量冗余或不足


- Q4 --- 用 命题逻辑 来解决寻路问题
	- 逐步构成KB的整体逻辑上与 Q3 类似
	- 整体还是 plan-then-solve 的逻辑，这里是完全 plan 完即寻找到 model 后然后提取出每个time_step为True的动作完全执行
	- 在处理问题的逻辑上也类似，有一个 success check(goal assertion) 环节(将最终目标与现存KB结合看是否有解)(由于KB都是T，而最终是T，因此最终目标为T，该model是解)
	- 可以看到 该方法解决问题是非常缓慢的

- Q5 --- 用命题逻辑 来解决吃豆问题
	- 其实就是更改为food 的 `GoalAssert` 以及 添加 food 的`SuccessorAxiom` 就好了，其它与Q4一致

------------ localization\mapping\slam

- 以下是知道全局大小的

- Q6 --- 实现 `localization` 
	- 问题描述：Pacman从一个已知的地图开始，但起始位置未知。它有一个4位传感器，可以返回其N、S、E、W方向是否存在墙壁。例如，1001表示贪吃蛇的北和西方向有墙壁，这4位使用一个包含4个布尔值的列表来表示。通过跟踪这些传感器读数和在每个时间步采取的动作，Pacman能够确定自己的位置。你需要编写帮助贪吃蛇确定在每个时间步可能位置的代码句子
	- 这题的坑挺多的
		- 第一个坑，要把所有已知事实，墙在和墙不在都要记录进去KB
		- 第二个坑，能加入 KB 的position并不是可能的position，能加入KB的position证明其==在已知事实的所有可能世界中==都成立，而possible position 只要有解就行
	- percept 是基于事实的出来的已知事实，因此加入KB
	- Axioms 都是完全正确的公理，用来辅助推理的


- Q7 --- 实现 `mapping`
	- 问题描述：Pacman现在知道自己的起始位置，但不知道墙壁的位置（除了知道外坐标边界的边界是墙壁）。类似于定位，它有一个4位传感器，返回其N、S、E、W方向是否存在墙壁。你需要编写帮助Pacman确定墙壁位置的代码句子。
	- 其余和Q6基本一致，就是 walls_grid 相当于就是 knownmap


- Q8 --- 实现 `slam`
	- 问题描述：在SLAM（同时定位与建图）中，Pacman知道自己的初始坐标，但不知道墙壁的位置。在SLAM中，Pacman可能会无意中采取非法行动（例如，当有墙壁阻挡时向北走），这会增加Pacman位置的不确定性。此外，在我们的SLAM设置中，Pacman不再有一个==4位传感器来告诉我们四个方向是否有墙壁，而只有一个3位传感器，可以揭示他相邻的墙壁数量==。这有点像wifi信号强度条；000 = 没有相邻墙壁；100 = 相邻1面墙壁；110 = 相邻2面墙壁；111 = 相邻3面墙壁。这3位由一个包含3个布尔值的列表表示。因此，您将使用SLAMSensorAxioms和numAdjWallsPerceptRules而不是sensorAxioms和fourBitPerceptRules，通过编写帮助Pacman确定（1）每个时间步的可能的地点，以及（2）墙壁位置
	- 其历程就是 Q6 和 Q7的结合，整体没什么变化

- ==核心 idea==
	- KBA的核心是KB整体一定保证是True，才能往后推，它的核心历程是逐步推并扩充KB直到achieve goal