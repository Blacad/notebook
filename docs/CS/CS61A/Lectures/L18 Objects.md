---
comments: true
---

- Object-Oriented Programming
	- `__init__` 是用于构建 Account 实例的函数的特殊方法名
	- `self` 是调用 方法 的 Account 类的实例
- 定义账户类
	- `class Account:`
		- `def __init__(self,account_holder):`
			- `self.balance=0`
			- `self.holder=account_holder`
		- `def deposit(self,amount):`
			- `self.balance = self.balance+amount`
			- `return self.balance`
- Note
	- python的类实例可以引入没有命名过的新属性，比如上面我只有属性`balance`和`holder`，但是`Account('A').address = 'ZJU'`添加地址属性并赋值
	- 对类实例级别赋值是浅拷贝 --- 可变类型的赋值都是浅拷贝，地址是一样的。同时，通过方法的增删改是不会影响可变类型的id的