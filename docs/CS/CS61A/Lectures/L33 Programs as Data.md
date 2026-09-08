---
comments: true
---

- Scheme程序由 expression组成 --- 类似于Python
	- `(list '+ 1 2) ----> (+ 1 2)` --- Scheme程序片段
	- `(eval (list '+ 1 2)) ----> 3` ---- 评估Scheme程序片段得到结果

- Generating Code
	- 因为程序片段本质上是数据，所以可以编写生成程序的程序
	- ==Quasi-quotation 准引用==![准引用.png](../assets/准引用.png)

	- 我们可以编写一个返回 Scheme程序片段的函数，而函数的参数可以是 代码的片段，这样我们甚至可以做出 泛化的历程，也能更好理解为什么说 `Programs are data`。详情见example里的scmpd
		- `sum-while` 就是可生成可泛化程序的函数




