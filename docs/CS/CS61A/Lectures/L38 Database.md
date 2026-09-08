---
comments: true
---

- 某种SQL中，它的文档会详细记录它的语法树，具体的各种操作很多很复杂而且不同种SQL可能的操作也不同，所以还是看文档，本节的一些常见用法还是得掌握


- 本课程使用的 `SQLite` 是比较轻量级的DBMS，你可以创建一个 `.db` 的数据库文件，使用 `sqlite3 db1.db` 打开文件并进入到 `sqlite`的命令行界面，然后就可以持久化存储数据库信息了，当然也可以用 python 对于 `SQLite` 支持的接口在程序中使用数据库 


- Create Table and Drop Table
	- 创建表的语法相对复杂，可以看相关SQL的文档
		- 一个简单例子 `create table if not exists table-name (column-def,...)` 或者是我们之前的 `create table if not exists table-name as select-stmt`
	- 删除表相对简单，`drop table if exists table-name`

- Modifying Table
	- `insert`
		- `insert into t(column) values (value),...;`
		- `insert into t values (value),...;`
		- `insert into t [select-stmt];`
	- `update`
		- `update t set column = [expression] where [expression];`
	- `delete`
		- `delete from t where [expression];`

- Python 与 SQL
	- python 原生支持 sqlite3 `import sqlite3`
	- `db = sqlite3.Connection('db1.db')`
	- `db.execute("create table primes (n,prime);")`

	- 占位符
		- `db.execute("insert into primes values (?), (?), (?);",range(4,7))`

	- `db.commit()` 对修改持久化到文件中