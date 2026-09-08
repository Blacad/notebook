---
comments: true
---

- generator 是一种特殊的迭代器
	-  生成器函数是一种函数，它产生值而不是返回它们。一个普通函数只返回一次；生成器函数可以多次产生值，例如：
	- `def plus_minus(x):`
		- `yield x`
		- `yield -x` 
	- `t = plus_minus(3)`
	- `next(t) == 3`
	- `next(t) == -3`
	- 当调用生成器函数本身时什么都不会发生，只有向它请求下一个值时它才会运行到下一个断点并立刻返回当处值
	- 当调用next时，它会执行到下一个`yield`处并输出yield处的值，这是一种惰性计算


- `yield from` 语句
	- `yield from list` 等同于
		- `for x in list:`
			- `yield x`
	- `yield from`语句常用来构造递归，在这种递归中将Base情况进行了形式隐去，即不符的情况就是Base，由于它惰性计算的特性我们可以查看前几个递归结果而不用全部计算。

- ==分析总结==
	- 生成器函数执行就两种常见形式 `next` 和 `for x in generator`
		- `next`分析简单，`next`会锁定frame，每次调用next时，它会执行到下一个`yield`处并输出yield处的值
		- `for x in generator`的情况相对复杂，`for`会锁定frame，每次执行到`for`就相当于调用一次`next`，直到`next`没有输出时循环结束
	- `for x in generator`和递归相结合使用时分析很复杂，把握两点
		- 运行到yield处时就会立刻返回值并让上一层往后执行
		- 同时`for`会锁定这个frame(不会退栈)，每运行到一次`for`就相当于调用一次`next`，直至没有返回循环结束
	- 一个分析例子如图所示
		- ![yield.jpg](../assets/yield.jpg)