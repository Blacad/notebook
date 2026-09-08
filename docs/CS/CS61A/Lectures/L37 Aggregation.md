---
comments: true
---

- Aggregate Functions
	- `select [columns] from [table]`
	- 在`[columns]`子句中，一个聚合函数从一组行计算出一个值
	- `select max(legs) from animals;`
		- legs 是表中的特定列
		- `max(legs)` 是对多行值进行计算，是聚合函数
	- 常用聚合函数 `avg、min、max、count、sum`
		- 其中`count(*) \ count(legs)`是计算总行数，但是 `count(distinct legs)` 是计算 legs 的取值不同情况数，比如我的表中有10行，但是 legs的取值只有 2、4，那么 `count(legs) ---> 10 count(distinct legs) ---> 2`
		- `distinct` 关键字能用于 count、sum，用于 sum 就是将不同的值加和，==略去重复值==


- Mixing Aggregate Functions and Single Values
	- 聚合函数也会在表中选择一行，这可能是有意义的
	- `select max(weight), kind from animals;`
		- 如果最大的weight是唯一的话
			- 它会返回 weight 最大的一行的 weight 和 kind
		- 如果最大的weight是不唯一的有几行都是最大
			- 它会返回任意一行最大weight 的 weight 和 kind

- Groups
	- 表中的行可以被分组在一起，聚合实际上在每个组单独执行
	- `select [columns] from [table] group by [expression] having [expression];`
	- 一些例子
		- `select legs, max(weight) from animals group by legs;`
		- `select legs, weight from animals group by legs, weight;` --- 返回legs 和 weight 的唯一组合，过滤掉重复的 < legs, weight > 组合
		- `select max(kind), weight/legs from animals group by weight/legs;`
	- ==`having`子句过滤聚合的组集合,`where` 子句过滤行,`having` 子句可以跟聚合函数连用==
		- `select weight/legs,count(*) from animals group by weight/legs having count(*)>1;`