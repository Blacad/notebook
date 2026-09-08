---
comments: true
---


- [项目文档](https://www.learncs.site/docs/curriculum-resource/cs61a/cs61a_zh/project/scheme)
- 本项目是实现一个Scheme子集的解释器
	- 在Lab10的时候，我们就实现了一个基于Scheme的计算器+define+and方法，只包含Eval + Apply过程，不包含 Lexical 和 Syntactic analysis的过程，于是我想到之前似乎实现过一个 词法与语法分析器，直接无缝缝合，最终实现了完整的历程，详情见 example中的Calculator
	- 在实现这个更大的解释器的过程中，能更加深入理解语言的底层原理，同时也能够巩固编译原理的知识



## 第一阶段 求值器
---
- 完善Frame --- define 和 lookup
- 完善 scheme_apply
	- 实现 builtin函数 的部分
	- 
- 完善 `scheme_eval` 
	- 对 `BuiltinProcedure` 的评估
	- 特别注意 `scheme_eval`，评估`symbol`时会基于环境去`lookup`, 我们可以看 `scheme.py` 的主历程，它创建 `global_frame` 时，会调用 `add_buildins`，因此所有的 `builtins` 函数都会在  `global_frame` 的 `bingdings` 中，其中的解包可以学习一下，所以 `first` 经过  `scheme_eval` 后会返回 `BuildinProcedure` 实例
- 完善 `define` 过程
	- 正向来形成 `解释树` 的过程理解
		- 每个括号分为两部分依次递归评估，记得补充每个括号最后的nil
	- 二级结论
		- 括号里的括号自动是 `Pair`,每个元素自身有个`Pair`
- 完善 `quote` 过程

- 第一阶段的任务其实我在自己写的计算器中已经实现过了，

## 第二阶段 过程
---
- 完善 `begin` 过程

- 完善 `lambda` 的创建过程

- 完善 `Frame` 创建子Frame的过程

- 完成 `Lambda` 的执行过程 --- 需好好分析环境
	- LambdaProcedure 中的 env 是它的父Frame
	- 当执行一个函数的历程时，当前环境肯定是该函数的Frame，因此  `LambdaProcedure` 的 apply 中 `env` 仍是 `env` 而不是 `first.env`（它的父Frame）
	- `scheme_eval` 和 `scheme_apply` 的 `env` 参数 都是当前环境
	- `eval_all` 实则是具体执行 `LambdaProcedure` 的函数，由此可见 `LambdaProcedure`  可以有多个 `Pair` ，返回最后一个 `Pair`的结果

- 完善 `define` 的定义函数的过程

- 完成 `mu` ，一种利用 Dynamic Scope 环境的过程定义方法
	- `Scheme` 和 `Python` 中使用的都是 Lexical Scope，但是 `Lisp` 的某些方言会使用 `Dynamic Scope`, 在本解释器中，我们实现两种方法
	- `mu` 的具体执行过程还是很有趣的，调用它时，一样也是会创建新Frame，但是它会将当前环境作为自己的父 Frame，然后就是常规的一路向上的 `lookup` 了

- 本阶段主要是完成用户自定义函数的功能，而且还实现了两种 Scope的函数

## 第三阶段 特殊形式
---
- 实现 `and` 和 `or` 的特殊形式 --- python 和 scheme是一样的
	- `and` 和 `or` 都从左往右执行，遵循短路机制
		- `and` 执行到 `False` 时短路并返回`False`
		- `or` 执行到 非 `False` 时短路并返回 非 `False` 的值

	- `and` 如果全为 `True` ，则返回最后一个的值
	- `or` 如果全为 `False`，则返回`False`

- 实现 `cond` 
	- `Scheme` 的 `cond` 语法很奇特 --- 像 `switch-case`
		- 如果条件为 非 `False`,但是后续没有表达式则返回 条件非 `False` 的值
		- 如果条件为 非`False`，后续有表达式，则会依次执行，返回最后表达式的值 --- `eval_all`
		- 如果条件为 `else` 则为 `True`
		- 如果没有 `else` 且 没有满足条件的分支，则返回 None `undefined`

- 实现 `let`
	- `let` 会在当前环境下创建子Frame
	- `let` 的绑定环节中是严格的一对一
	- 正向来形成 `解释树` 的过程理解
		- 每个括号分为两部分依次递归评估，记得补充每个括号最后的nil
		- -----------![解释树形成.jpeg](../assets/解释树形成.jpeg)

- 完成测试套件
	- 在完成测试套件的过程中，遇到了最麻烦的一个问题，即过度评估一个过程已经评估到位了但是仍然继续评估
	- `(apply append '((1 2) (3 4)))`
		- 我原先的策略是，在 `apply` 中再调用`scheme_eval`处理函数参数，但是总是失败
		- 后来改为在 `eval` 中就完全将参数处理好，结果成功了
		- 通过调试可以看到 `complete_apply` 函数中，会调用 `scheme_apply` 过程，当使用`apply` 方法时，它会调用 `complete_apply` 函数，进而造成过度评估

 
- 至此，`Scheme` 的解释器历程就已经编写完成了，之后是写一些 `Scheme` 历程，我就不再赘述了
- 成功通过所有测试，完成 `Scheme` 项目 ✅
- 同时也完成了 CS61A的所有项目 ✅


## 注意
----
- 函数参数定义法
	- 关键字（+默认参数）
	-  `*args`接任意数量的位置参数，函数内接收为元组
	-  `**kwargs`接受任意数量的关键字参数，函数内接收为字典
	- 在函数定义中使用 `*`，表示后面的参数只能用关键字方式传入
	- 在函数定义中使用 `/`，表示前面的参数只能用位置传递
- 函数参数传递法
	- 位置参数与关键字传参混用时注意 --- ✅ **规则**：位置参数要在关键字参数前面
	- `*args` 函数参数表示法：`f(1, 2, 3)` 等效于 `f(*[1, 2, 3]) or f(*(1,2,3))` 对list或tuple解包
	- 使用 `**kwargs` 对**字典**进行解包传参
	- `*args` 与 `**kwargs` 同时使用前者在前，后者在后

- python的特殊字符串
	- `f‘It is {word}.’` 使用 `word`变量替换`{word}`
	- `'It is {0}'.format(word)` 使用`word`变量替换`{0}`

- 再次强调装饰器函数的过程
	- 定义
		- 装饰器函数的历程会在被修饰函数定义时执行
		- 一直执行装饰器函数的例程，直到执行完 ==直接调用被修饰函数的装饰器==
	- 调用
		- 执行 ==直接调用被修饰函数装饰器==的返回函数

- 调用自定义函数时才会创建 Frame，Builtins的方法是不创建Frame的

- Python的 `and` 和 `or`
	- `and` 和 `or` 都从左往右执行，遵循短路机制
		- `and` 执行到 `False` 时短路并返回`False`
		- `or` 执行到 非 `False` 时短路并返回 非 `False` 的值

	- `and` 如果全为 `True` ，则返回最后一个的值
	- `or` 如果全为 `False`，则返回`False`
