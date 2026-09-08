---
comments: true
---

- 控制表达式
	- Logical Operator
		- `<left> or <right>` 
		- `<left> and <right>`
		- 上面两个都是先计算`<left>`并评估，如果通过再计算`<right>`并评估
		- `not <exp>`
		- Lab2的[真值判断](https://www.learncs.site/docs/curriculum-resource/cs61a/cs61a_zh/lab/lab02#q1wwpd%E7%9C%9F%E5%80%BC%E5%88%A4%E6%96%AD)
			- and 如果是对的则返回后一个量；or 如果对了就返回对的位置的值。否则返回 False
			- None、''(空字符串)、0是False
			- and or 都是从左向右看

- 高阶函数Higher-Order Function --- 可以提供泛化功能，返回函数或以其他函数为参数的函数
	- 泛化函数 --- 调用函数(这个严格上讲不算高阶函数)
		- 比如计算正多边形或圆形面积都有 `r^2`，我们可以定义一个函数`area(r,shape)`计算 `r^2`并检查`r`的正确性，同时利用`shape`区分不同图形，然后再定义其他函数分别计算诸如正方形、圆形、正六边形等的面积。这样每个泛化后的函数都会调用`area`比如`area_square(r): return area(r,1)`。这就实现了泛化函数
	- 泛化计算过程 --- 以其他函数为参数
		- 不同公式具有相同的计算过程，我们可以将计算过程进行泛化。比如计算自然数之和与计算它们的立方和
		- 代码示例[L4-1](../示例代码/L4-1.md)
	- 返回函数的函数 --- 它告诉我们在python中函数与其他变量没什么区别
		- 我们可以通过嵌套定义函数的方法在一个函数体内再定义一个函数，并将内部的函数作为返回值
		- 代码示例[L4-2](../示例代码/L4-2.md)
