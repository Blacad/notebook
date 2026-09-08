---
comments: true
---

- class attributes
	- 类属性会被所有类实例共享，它们是类属性而不是实例的属性
- Attribute Lookup
	- `instance\class.name` 使用Name查属性时，先找自己的实例属性再找自己的类属性
	- `getattr(instance\class,'name')` 使用getattr查属性，先找自己的实例属性再找自己的类属性
	- `hasattr(instance\class,'name')` 使用hasattr查有无属性，先找自己的实例属性再找自己的类属性，有返回True
	- `setattr() delattr()` 只针对自己可write的属性

- ==属性赋值 Assignment to Attributes== --- 创建或修改属性
	- 如果赋值调用者是 instance，那么赋值的是 instance的属性
	- 如果赋值调用者是 class，那么赋值的是 class 的属性
	- `最需要注意的应该是instance企图修改class属性时，是会创建一个同名的instance属性由该instance自己保留而非修改class属性。可参考examples里的代码`
	- class对象
		- 查访 --- class attributes
		- 增改删 --- class attributes
	- instance对象
		- 查访 --- 自己的instance attributes + class attributes
		- 增改删 --- 自己的instance attributes
	- 所以用instance对class attribute企图做write操作时，本质上是创建了一个同名的instance属性


- Method Calls
	- 点调用

- Bound Methods = Object + Function 一般是 instance + Function
	- `self`为第一个参数的方法 
	- `account.deposit(5)`等价于 `Account.deposit(account,5)`
