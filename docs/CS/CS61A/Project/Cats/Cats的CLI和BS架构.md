## 1 CLI入口
---
- `@main run`
- 使用 main 装饰器
	- `inspect`  模块是 Python 标准库中的一个重要模块，它提供了许多函数来帮助获取有关活动对象的信息，如模块、类、方法、函数、回溯、帧对象和代码对象等。这对于实现调试器、解释器、文档生成工具等非常有用。
		- `inspect.stack()` 返回该函数调用位置的堆栈列表信息，比如调用顺序是 `bar()--foo()---func1()` 在func1中调用stack，则它返回的列表是 `[func1,foo,bar,<module>]`的堆栈信息 `insepct.FrameInfo`
		- `insepct.FrameInfo` 包含诸多信息
			- **frame**: 当前栈帧对象（frame object）
			- **filename**: 当前执行的文件名
			- **lineno**: 当前执行的行号
			- **function**: 当前执行的函数名
			- **code_context**: 当前行的代码上下文（通常是包含当前行的列表）
			- **index**: 在 `code_context` 中当前行的索引
	- `sys` 模块是 Python 标准库中的一个重要模块，它提供了一系列变量和函数，用于与 Python 解释器进行交互和控制 Python 运行时环境。
		- 最重要的作用之一就是**访问命令行参数** ---  `sys.argv`：获取传递给 Python 脚本的命令行参数列表，那么`sys.argv[1:]`是返回除了脚本名称的其它参数
	- **name** 是 Python 中的一个特殊内置变量，它存在于每个模块的全局命名空间中：
		- 当模块被直接运行时，**name** 的值为 `'__main__'`
		- 当模块被导入时，**name** 的值为模块的文件名(不包括.py扩展名)
		- `也就是说当一个函数被装饰器装饰时，相当于该函数成为了装饰器函数而自己成为参数`

- `argparse`模块是 Python 标准库中的一个模块，用于解析命令行参数和选项。它提供了一种声明式的方式来定义程序需要接受的命令行参数，自动生成帮助信息和使用说明。
	- 创建解析对象 --- `parse = argparse.ArgumentParser(description='****')`
	- 定义参数
		- 位置参数 --- `parse.addargument("***",help="***",nargs="*")`
		- 布尔参数 --- `parse.addargument("-*",help="***",action="store_true")`
		- 带值参数 --- `parse.addargument("-*",help="***",actioin="store")` 默认行为
		- `action`字段的含义
			1. **`store_true`**：存储 `True`（用于标志参数）
			2. **`store_false`**：存储 `False`（用于反向标志参数）
			3. **`store`**：存储参数值（默认行为）
			4. **`append`**：将值添加到列表中
			5. **`count`**：计算参数出现的次数
		- 位置参数需要在 `-*` 参数的前面或者后面
	- 使用参数
		- `args = parse.parse_args()`
		- `args.t` or `args.topic`

## 2 CLI主逻辑
---
- `run_typing_test`
- 读取数据库中的所有行到内存变量 `lines_from_file`
	- `这个需要注意一下python读文件的方法，有常用的open()+close()方法，但如果是整个文件可以用with`
- `random.shuffle(list)` 打乱列表
- 定义 `about + select` 函数用于选句子，其中的句子除杂已经在cats中说了
- 根据之前实现的函数返回 `wpm + accuracy`

## 3 BS架构
---
- `start`
- 修改全局变量 `global` 声明。这个在之前提到过
-  `os` 模块提供了与操作系统交互的功能，用于处理文件和目录、环境变量、进程管理等系统级操作。
	- 最常用的就是 环境变量
		- 获取 --- `os.environ.get("***","")` or `os.getenv("***","default")`
		- 设置 --- `os.environ['MY_TOPIC']="***"`
		- python的环境变量，类似于全局变量而有不同，与环境变量不同之处
			- 只能存储字符串
			- 可以被子进程继承
			- 存储在进程空间而非python内存
			- 环境变量更适合存储配置信息和跨进程共享的数据，而全局变量更适合在程序内部共享数据
	- 还可以系统操作
		- `os.system('ls -l')`
	- 管理文件和目录 `不同于读写文件，更偏向对文件目录的管理`


- `urllib + http + socketserver + webbrowser` 提供一系列python网页搭建的方法
- `json` 提供请求的数据结构转换

- 总体上的逻辑就是 handler处理请求等，并使用本地的各种资源，和之前的BS架构逻辑相近，但是想自己完整搭出来也是很麻烦的，这个等到CS61B再谈吧，这里还是只注重python本身的一些东西