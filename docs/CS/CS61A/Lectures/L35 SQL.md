---
comments: true
---

- databases 和 DBMS

- 语言类型
	- declarative  language --- SQL & Prolog....
	- imperative language --- python & scheme...

- SQL (Structured Query Language)
	- SQL是一种有 ANSI 和 ISO标准，但是不同的DBMS有自己方言的语言
	- A `select` statement creates a new table, either from scratch or by projecting a table
	- A `create table` statement gives a global name to a table
	- Lots of other statements exist: `analyze, delete, explain, insert, replace, update`, etc.
	- Most of the important action is in the `select` statement

- `select` 
	- 一个选择语句始终包括一个以逗号分隔的列描述列表。列描述是一个表达式，可选地后跟as和一个列名
		- `select [expression] as [name], [expression] as [name]`
	- `select` 创建一个单行表，两个选择语句的 `union` 是一个包含它们各自结果行数的表

- `create table`
	- 选择语句的结果会显示给用户，但不会存储。创建表语句给结果赋予一个名称，并存储起来
	- `create table [name] as [select statement]`

- Projecting Tables 投影表
	- `SELECT`语句可以使用`FROM`子句指定一个输入表，从已存在的表中投影
	- `select [columns] from [table] where [condition] order by [order]`

- Arithmetic in Select Expressions
	- select的表达式中可以执行算数计算，列名计算为行值，算术表达式可以组合行值和常数
	- ==当用列名参与运算时，意味着对每行进行处理==
	- `select col1, col2 + 2 as plus2 from lift`