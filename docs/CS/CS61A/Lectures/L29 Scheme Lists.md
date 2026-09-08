---
comments: true
---

- Scheme Lists
	- `cons` Two-argument procedure
		- 创建一个 linked list
		- 该`cons`会将第一个参数视为一个元素，第二个参数为 list
	- `car`
		- 返回list的第一个元素
	- `cdr`
		- 返回list的除第一个元素的剩余list
	- `nil` --- 空链表

- Scheme Lists相关的更多过程
	- `(draw (cons s (cons s nil)))`
		- Scheme中的赋值和python中相同，针对可变对象是浅拷贝，对于非可变对象是深拷贝
	- `(list? s)`
	- `(null? nil) == t`
	- `(list 1 2 3 4)` 
		- 可变参数过程 创建一个 linked list
		- 每个参数都被视为一个元素

- Symbolic Programming 符号式编程 --- 有点像字符串，但是返回值是有讲究的。scheme自己也是有字符串和字符类型的`"hello"` 和 `#\a`。 [Scheme的类型](../Notes/Scheme的类型.md)
	- `(quote a) === 'a`
	- `'a` 是指 符号a的引用，它本身就是值
		- `(define a 1)`
	- `'(a b c)` 是指 列表(a b c)的引用
	- quote的返回值是指符号本身 --- 可以理解为评估quote后面的字符串为相应类型
		- `'a` 返回a ---> symbol
		- `'(a b c)` 返回列表 `(a b c)` ---> list
		- `'1` 返回 1 ---> number


- List Processing
	- `append`、`map`、`filter` 、`apply`
		- `apply` 是将list的所有参数都作为 func的参数，调用一次，返回函数结果
		- `map` 是将list的每个参数作为func的参数，调用多次，返回结果list
	- 寻找 包含s所有子序列和为偶数的列表，详见example中的even_set，其中最值得关注的是它的位运算很巧妙的穷举了所有情况
