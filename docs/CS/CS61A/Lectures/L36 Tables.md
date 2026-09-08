---
comments: true
---


- Joining Tables
	- Two tables A & B are joined by a comma to yield all combos of a row from A & a row from B --- 两个表A和B通过逗号连接，以产生A表中的一行与B表中的一行的所有组合
		- `select parent from parents, dogs where child = name and fur="curly"`
			- parents 表有 name、fur 列
			- dogs 表有 parent、child 列
			- `from parents, dogs` 产生A表中的一行与B表中的一行的所有组合
			- `child = name` 确保生成表中每行的 child 等于 name --- ==如果用列名作为 oprands 意味着对每行进行操作==


- Aliases and dot expressions
	- 希望能从特定的表中选择出一列，引入 别名 和 dot表达式，有助于消除歧名，比如 join 的两张表有同名的列 或 两张同样的表
	- `select [columns] from [table] where [condition] order by [order]`
	- `[table]` 是是一个以逗号分隔的表名列表，带有可选别名
	- eg. `select a.child as first, b.child as second from parents as a, parents as b where a.parent = b.parent and a.child < b.child`

- Joining Multiple Tables
	- Multiple tables can be joined to yield all combinations of rows from each --- 多个表可以连接起来，以产生每个表的行组合的所有可能


- Numerical Expressions
	- 表达式可以包含函数调用和算术运算符
	- `select [columns] from [table] where [expression] order by [expression]`
		- `[columns] <---> [expression] as [name], ...`
		- Combine values ---> +, -, \*, /, %, and, or
		- Transform values ---> abs, round, not, -
		- Compare values ---> <, <=, >, >=, (<>, !=), =


- String Expressions
	- `select "hello," || " world"; ---> hello, world` 连接字符串
	- 字符表
		- `create table phrase as select "hello, world" as s;`
		- `select substr(s,4,2) || substr(s, instr(s, " ")+1, 1) from phrase;`
			- `substr(string,position,length)`
			- `instr(s, " ")+1` 返回s中第一个空格后面的位置 --- 这里的位置从 1 记录