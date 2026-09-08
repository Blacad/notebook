- 打印时的特殊字符 ---- `\`
	- 它用于构成转义字符，类似于成帧中的特殊字符。如果我们想打印它本身则需要`\\`
	- `\n`  + `\r` + `\b` 等等，如图![Pasted image 20250321125437.png](../Pasted%20image%2020250321125437.png)
	- ❓\0到底有什么作用捏✅
		- '\0' 是字符串的结束符，任何字符串之后都会自动加上'\0'。如果字符串末尾少了‘\0’字符，则其在输出时可能会出现乱码问题

- C 语言中的库函数都有相应的文档，我们可以通过文档知道库函数有哪些以及它的行为。这里提供一个[cs50整理的C语言库函数文档](https://manual.cs50.io/)比较好用

- Linux 常用命令
	- .. 表示父目录，. 表示当前目录
	- cd \<path> 进入路径
	- mkdir \<dirname> 创建文件夹
	- rm \<filename> 删除文件
	- rmdir \<dirname> 删除空文件夹
	- rm -rf \<dirname> 递归删除文件夹
	- mv
		- mv \<filename1> \<filename2> 更改文件名字
		- mv \<filename> \<dirname> 将文件移入文件夹
	- cp \<filename1> \<filename2> 备份(拷贝)文件

- 使用docker 弄个 Linux 玩，具体操作见OSNote中的Docker说明

- 编程准则
	1. Be simple and short
	2. Be tidy
	3. Avoid repeating ---- 其实说明了函数化模块化编程

- prototype 函数原型
	- void meow(void);

- variable function scope 变量作用域 and variable life 变量生命周期 --- { }

- do while 结构比较适合用来处理用户的不合规输入，然后让用户一致输入直到满足 

- integer flow

- truncation due to division

- floating point imprecision 浮点不准确性 

