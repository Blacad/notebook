---
comments: true
---

- Lists
	- `[1,2,3,4,5]`
	- `normal=[1,2,3,4,5]`
	- `normal[0] == 1` or `getitem(normal,0)`
	- `len(normal) == 5`
	- `[3,4]+normal*2 == [3,4,1,2,3,4,5,1,2,3,4,5]` or `add([3,4],mul(normal,2))`(乘2相当于复制一份加到后面)
	- `nested_list=[[0,1,2],[3,4,5]]`
	- `nested_list2=[0,1,2,[3,4],5]`
	- `nested_list3=[0,1,2,[3,4],'a',4.5,True,[None,lambda:5,'a']]`
	- 负索引从列表末尾开始计数，最右边元素的索引是`-1` --- `nested_list3[-1]==[None,lambda:5,'a']`

- Contain --- `in` or `not in` 表达式
	- 寻找单独元素是否存在于Lists，存在返回True，不存在返回False

- For 语句
	- `for element in Lists:...`
	- `for <name> in <expression>:<suite>` 其中expression是能迭代的比如Lists
	- for语句对Lists解包 --- 一般用作序列长度相等的情况
		- `pairs = [[0,1],[1,2],[3,4]]`
		- `for a,b in pairs:...`
		- `Note:第一轮(a==0,b==1)`

- Range
	- 它是连续整数的序列但不是List
	- `range(-2,2)`
		- 左闭右开
		- `Length == ending value - starting value`
	- 转换为List --- `list(range(-2,2))`
	- `range(4)` 缺省起始值则默认为0
	- `range(5,1,-1)` 最后一个参数调整步长默认为1

- `Note:可以使用下划线接受值，这样表示你不打算在其他地方用它`

- List Comprehensions
	- `[i*2 for i in [1,2,3,4]]`
	- `[i for i in [1,2,3,4] if i%2==0]`
	- `def divisors(n):return [1]+[x for x in range(2,n) if n%x==0]`