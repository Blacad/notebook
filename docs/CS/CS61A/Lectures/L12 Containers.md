---
comments: true
---

- Box-and-Pointer Notation
	- 嵌套列表的内层列表元素本质上是指向内层列表的指针

- Slicing
	- `odds=[1,3,5,7,9]`
	- `[odds[i] i in range(1,3)] == odds[1:3]`
	- 左闭右开
	- 缺省左侧 ---- `odds[:3] == [1,3,5]`
	- 缺省右侧 --- `odds[1:] == [3,5,7,9]`
	- 全部缺省 --- `odds[:] === odds`
	- ==`Note:切片会产生新的List。也就是说改变切片的值不会对原list造成影响`==

- 处理容器的值
	- sum
		- `sum(iterable[,start])--->value` 
			- 表示sum接受两个参数，第一个参数需要能迭代，第二个参数可选。
		- `sum([3,4,5]) == 9`
		- `sum([3,4,5],6) == (6+3+4+5)`
		- `sum([[2,3],[4,5]]) ---> Error` 但是 `sum([[2,3],[4,5]],[]) == [2,3,4,5]`
		- sum不支持字符串相加
	- max(对应min)
		- `max(iterable[,key=func])->value` or `max(a,b,c,...[,key=func])`
			- 表示max接受两个参数，第一个参数需要能迭代，第二个参数可选且是具名参数(需要通过`key=value`来赋值调用)。
			- 表示max接受多个参数，最后key参数可以省略。
			- 它的逻辑是对前面每个参数或可迭代的每个item作用key函数然后根据返回值选择最大的。
		- `max(range(-20,10),lambda x:abs(x)) == -20` --- 返回绝对值最大的数


	- all(对应any)
		- `all(iterable)->bool`
			- 表示all接受一个可迭代参数
			- 它的作用是对可迭代对象的每个item作用bool函数都为True则返回True，否则返回False
		- 例子`all([x<5 for x in less_than5_list])` --- 前面是bool测试后面是遍历

- Strings
	- 三种形式
		- `'abc'` or `"a'b'c"` or `"""abc"""`(跨多行)
	- String 是序列但与其他序列又有所不同
	- 对于String的contain语句(`in` or `not in`)可以检测整个字符串
		- `'where' in 'where is it' == True`

- Dictionaries
	- `dict = {'a':1,'b':2,'c':'c','d':lambda x:x}` --- key-value
	- `dict['a']==1` 用key作索引
	- 字典也是序列是key的序列 --- `list(dict)==['a','b','c','d']`
	- 字典的value序列 --- `dict.values()`(是一种可迭代序列但不是list)但是可以转换`list(dict.values())`
	- key不能是 list 或 字典，而且不能有两个同样的键(采取后一个key-value)。字典的 key 不可以是可变数据，也不能包含可变数据
	- Dictionary Comprehension
		- `{key:value for name in iter if filter}`