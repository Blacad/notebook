-  command line arguments
	- 例如，c 语言的编译命令 --- clang -o hello hello.c -lcs50
		- -o、hello、hello.c 和 -lcs50 都是命令行参数
			- -o 和 hello 指定文件名
			- hello.c 为输入文件
			- -lcs50 如果你使用了非标准库需要在编译参数中添加该库的名字，比如数学库 -lm，这里是cs50库

- compile --- 以C语言为例
	- preprocess
	- compile
	- assemble
	- link

- debugging --- 程序员的一生之敌🦷
	- printf 大法
	- 常用调试器 debugger ，在cs50中使用debug50，但是一般使用gdb
	- 