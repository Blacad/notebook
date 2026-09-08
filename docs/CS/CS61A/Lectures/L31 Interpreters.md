---
comments: true
---

- 本节课主要讲解 Scheme Evaluator 应当如何评估Scheme中特殊形式的Expression
	- `define`
		- 涉及到 frame 与 environment
	- Logical Forms 逻辑形式
		- 比如 `if` 、`and` 、`cond`等等
	- Lambda表达式
		- symbols --- 该表达式的变量名们
		- env、frames --- lambda的环境和框架
			- 框架需要有 `define` 和 `lookup`方法
	- `quote`
	- .....
- Scheme Evaluator的 Evaluate 和 Apply 过程中需要操作Frame来搭建整个程序的环境

- ==环境==
	- Lexical(Static) scope --- 词汇作用域：一个frame的父级是定义过程的环境
	- Dynamic scope --- 动态作用域：一个frame的父级是调用过程的环境
	- -----例子![Pasted image 20251014194757.png](../assets/Pasted image 20251014194757.png)
		- ==Python和Scheme均使用 Lexical Scope==

- ==Note 本讲的两个关键洞悉==
	- Scheme中的特殊形式往往由第一个操作符区分
	- Scheme的Evaluator需要操作Frames管理Environment
		- 特别是`define`、`lambda`
