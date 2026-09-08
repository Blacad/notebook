---
comments: true
---

- 容器能提供一个迭代器，迭代器提供访问元素的顺序
	- `iter(iterable)`创建迭代器，指向第一个元素
	- `next(iter)`推进迭代器，输出指向元素并后移，如果到达末尾继续后移会抛出`StopIteration`错误
	- `list(iter)` 列出迭代器中后续的所有元素并将迭代器置于末尾，返回迭代器后所有元素的list

- Dictionary Iteration
	- python字典3.5之前是无序的，后序有序
	- `iter(d.keys()) == iter(d)` 迭代key
	- `iter(d.values())` 迭代value
	- `iter(d.items())` 迭代key-value
	- ==如果在使用字典迭代器时增删字典，则迭代器失效，仅改不影响迭代器==

- For 语句
	- `for i in iter:` 可以遍历迭代器的所有元素，但是会将迭代器推至末尾以至于无法再使用

- Built-in Function for iteration
	- 惰性计算，只有迭代到时才调用函数
	- `map(func,iterable)` 遍历iterable中的func(x) --- 返回iter
	- `filter(func,iterable)` 遍历可迭代对象中的x，满足func(x) 为真(一直向后应用直到找到第一个达成的) --- 返回iter
	- `zip(first_iter,second_iter)` 遍历共索引的（x，y）对 --- 返回iter
		- `list(zip([1,2],[3,4])) == [(1,3),(2,4)]`
		- `list(zip|[1, 2], [3, 4, 5])) == [(1,3),(2,4)]`
		- `list(zip([1, 2], [3, 4, 5], [6, 7])) == [(1, 3, 6), (2, 4, 7)]`
	- `reversed(sequence)` 遍历序列中x的逆序 --- 返回iter
	- `list(iterable)` 创建一个包含迭代器中所有x的list
	- `tuple(iterable)` 创建一个包含iterable中所有x的tuple
	- `sorted(iterable)` 创建一个包含 x 的可迭代对象的排序列表