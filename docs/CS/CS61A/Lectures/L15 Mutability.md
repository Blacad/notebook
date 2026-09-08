---
comments: true
---

- ASCII标准是专门英语设计的，但Unicode标准是拥有一个字符集，可以用于所有不同的语言

- mutation operations
	- `List`的类操作 `insert`等等
	- `list.pop()` --- 返回列表最尾元素并从列表移除
	- `list.remove('***')` --- 从列表移除***
	- `list.append('***')` --- 在列表后面添加***
	- `list.extend(['a','b'])` -- 在列表后面添加 a b
	- `list[2] = 'spade'` --- 将列表索引2改为spade
	- `list[0:2] = ['heart','diamond']` --- 将列表前两个元素改为后者
	- ==`Note: python的list使用赋值绑定时，是一个对象的两个名称，即 list2 = list时，对list的改变会反应到list2上。浅拷贝。深拷贝可以用切片或者列表推理`==
		- 所有指向同一对象的名称都受到突变(mutation)的影响
		- 只有可变类型的对象可以改变：列表和字典
		- [python中的可变类型与不可变类型辨析](../Notes/可变不可变.md)
