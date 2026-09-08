---
comments: true
---

- 实现Linked List
	- 见example的LinkedList文件

- 实现Tree
	- 见example的Tree文件
	- 在编写Tree的程序时发现了一个非常有趣的bug，我将创建树函数的第二个参数使用默认参数形式，同时创建了多个Tree实例且没有指定第二个参数，在python中这些实例的children都指向同一个空list即默认参数，因而产生了bug，这是由于list是一个可变类型采用浅拷贝。启发我们 不要用可变类型做默认参数，而应该使用None并在函数体里赋值