
- Model Merging 基本概念
	- 以 向量 的视角 看待 model，将具备某种能力的 Model 视作有某种能力的向量
	- 然后我们可以把向量的加减运算，视作能力的赋予或剥夺
	- 这真的有效，当然需要有一些别的技巧，但是根本上是上述所说的那样


- 三种 Model Merging 的基本方法
	- [相加](../../Slides/Lecture11/merging.pdf#page=7)
		- 具备功能一 的 model 1
		- 具备功能二的 model 2
		- foundation model
		- $\Delta_1=model_1 - fm$
		- $\Delta_2 = model_2 - fm$
		- 同时具备两种能力的模型
			- $newModel = fm + \Delta_1 + \Delta_2$
	- [相减](../../Slides/Lecture11/merging.pdf#page=13)
		- 具备能力一的 model 1
		- 负面能力的 model 2
		- foundation model
		- $\Delta_2 = model_2 - fm$
		- 负面能力减弱的模型
			- $newModel=model_1 - \Delta_2$
	- [类比](../../Slides/Lecture11/merging.pdf#page=15)
		- Task A ：Task B = Task C ： Task D
			- A任务之于B任务 等同于 C任务之于D任务
			- 比如 A 任务是 中文翻译英文，B任务是 
		- $model_A$ 表示具备 Task A 能力的 model，依次类推
		- $\Delta_1 = model_B - model_A$
		- $model_D=model_C + \Delta_1$

- Merging 的方法显然有一些问题，比如 需要merging 的向量往往并不是完全无关的，因此单纯的相加最终的效果往往并非是直接叠加
	- Maybe 正交处理可能有效果
	- 有一些[解决方案](../../Slides/Lecture11/merging.pdf#page=23)