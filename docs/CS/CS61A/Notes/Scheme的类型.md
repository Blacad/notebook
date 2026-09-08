
| 类型类别   | 判断函数             | 示例                           |
| ------ | ---------------- | ---------------------------- |
| 数字     | `(number? x)`    | `(number? 42)` → `#t`        |
| 整数     | `(integer? x)`   | `(integer? 3.14)` → `#f`     |
| 实数     | `(real? x)`      | `(real? 3)` → `#t`           |
| 有理数    | `(rational? x)`  | `(rational? 3/4)` → `#t`     |
| 复数     | `(complex? x)`   | `(complex? 2+3i)` → `#t`     |
| 字符串    | `(string? x)`    | `(string? "hello")` → `#t`   |
| 符号     | `(symbol? x)`    | `(symbol? 'abc)` → `#t`      |
| 布尔     | `(boolean? x)`   | `(boolean? #f)` → `#t`       |
| 列表     | `(list? x)`      | `(list? '(1 2 3))` → `#t`    |
| 向量     | `(vector? x)`    | `(vector? '#(1 2 3))` → `#t` |
| 程序（过程） | `(procedure? x)` | `(procedure? car)` → `#t`    |
| 空表     | `(null? x)`      | `(null? '())` → `#t`         |
| 字符     | `(char? x)`      | `(char? #\a)` → `#t` ✅       |
