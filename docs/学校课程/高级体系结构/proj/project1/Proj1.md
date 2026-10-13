## 作业描述

### 作业 1：数组求和-无原子的解决方案

截止日期：2024年11月26日，星期三 - 上午10:00

#### 提交方式

用开发者账号登录沐曦算子竞赛平台(https://operator.metax-tech.com/)，上传修改后的项目工程，参加竞赛排名。
#### 作业描述

《GPU Programming with MXMACA -- Part2》“第7章 原子操作”里数组求和例子(https://gitee.com/Inkstoneydz/mxmaca-courses-zju/tree/master/demo/07_atomic)。其实数组求和也可以不需要原子操作，先用GPU先把数据尺寸缩减到CPU可以接受的范围内，然后让CPU完成的思路。

具体方法如下：

1）首先，声明 sum 为比原数组小 1024 倍的数组。

2）然后在 GPU 上启动 n / 1024 个线程，每个负责原数组中 1024 个数的求和，然后写入到 sum 的对应元素中去。

（因为每个线程都写入了不同的地址，所以不存在任何冲突，也不需要原子操作了。）

3）最后求出的大小为 n / 1024 的数组，已经足够小了，可以直接在 CPU 上完成最终的求和。

main.cpp的文件里的“parallel_sum_V1”基于上述方法已经实现了一个简单的核函数。请根据parallel_sum_V2/V3/V4/V5/V6/V7的注释和提示，尝试进行更多更深入的优化。
#### 参考资料

《沐曦异构并行计算软件栈--MXMACA C/C++程序设计入门教程》的第8.3.4小节“适配GPU的硬件行为”/“图 8-17 三种常用的规约操作方法的实现思路（以处理 8 个元素为例）”



## 实践

### 项目搭建

- 首先就是 拆解一下这个 CMakeLists，已经好久没有写过 C 的项目了，makefile 和 cmake 的一些东西都还给老师了 🥹

- 我先简单 回顾了下之前学习 Cmake 的笔记，来试着一行行拆解一下
	- 第一行，不必多说，所有顶层 Cmake 必备的 cmake 最小版本说明
	- `set(CMAKE_CXX_COMPILER /opt/maca/mxgpu_llvm/bin/mxcc)` 
		- 指定 C++ 编译器为后者
	- `project(main LANGUAGES CXX)` 
		- 创建名为 `main` 的项目，只启用 C++
	- `set(CMAKE_CXX_STANDARD 17)` & `set(CMAKE_CXX_STANDARD_REQUIRED ON)`
		- 要求使用 C++17 & 不允许自动降级
		- 相当于添加 `-std==c++17`
	- `set(CMAKE_BUILD_TYPE Release)`
		- 指定 Release 模式，通常意味着启用优化、减少调试信息
	- `set(TARGET program)`
		- 定义一个变量 `TARGET = program` 方便复用
		- 类似于 shell，后续直接就是 `$(TARGET)`
	- `file(GLOB SRCS main.cpp kernel.cpp)`
		- 把源文件放入 `SRCS` ，相当于 `set(SRCS main.cpp kernel.cpp)`
		- `GLOB` 主要提供通配符能力
	- `add_executable(${TARGET} ${SRCS})`
		- 配置可执行文件 可执行文件的名字 和 需要编译链接的文件
	- `target_compile_options(${TARGET} PRIVATE -x maca --extended-lambda )`
		- 给 `program` 的源文件添加编译选项
		- 相当于 `mxcc -x maca --extended-lambda`
	- `install(TARGETS ${TARGET} DESTINATION bin)`
		- 定义安装规则
		- 最终会把：`program` 安装到：`<prefix>/bin/program`

- **MACA/GPU kernel 的编译过程**，最值得注意的是：这里并没有把 `kernel.cpp` 声明成一种独立的 CUDA/MACA language，而是把整个 target 当成 **C++ target**，然后通过 `mxcc -x maca` 让编译器自己识别并处理其中的 host/device 代码
	- 这有些类似于 `nvcc -x kernel.cpp` 不要根据后缀识别用，把这个文件按照CUDA文件来处理

- 规范的 MAMX/CUDA C++ 程序
	- 编译阶段我们只需要知道 声明 就好了不需要知道具体实现，因此 往往用 `.hpp` 搭桥
	- 链接阶段才需要具体实现，因此链接时再把目标文件并起来就好
	- 通常，我们暴露给 C++ 程序的接口需要符合 C++ 的规范，因此往往 `.cu` 实现的核函数需要封装一层为 C++的函数签名，然后再 通过 `.hpp` 声明这一层封装的函数签名，否则原签名 gcc 是不认识的

### 项目解析

- 根据上述分析，两个关键文件是 main.cpp + kernel.cpp

- main.cpp
	- 不用想就是定义主逻辑的地方
	- `std::vector<int, MacaAllocator<int>> arr(n)`
		- 存储 `int` 类型元素的动态数组(向量) `arr`
		- 需要申请/释放底层存储时，使用 `MacaAllocator<int>`，而不是默认的 `std::allocator<int>`
		- 我们看到它的内存分配用的是统一内存，具体见 [GPU 并行编程的基本知识](../../program/2%20CUDA%20C++%20Intro.md) 
	- `arr.data()`
		- 返回 `&arr[0]` 即 `int*`

	- 整体逻辑就是 cpu 先算一遍全部加起来的结果，然后 让 gpu 计算全部加起来的结果并启用计时器，记录gpu计算的时间，共测量 10 轮，同时需要比较cpu结果对比正确性

- kernel.cpp
	- 实现gpu计算数组求和的地方
	- [GPU 并行编程的基本知识](../../program/2%20CUDA%20C++%20Intro.md)
	- 在 GPU 核函数中，索引是一大问题，最好画个图分析 正面向你的是 xy 平面，你只需要知道肯定只有一个grid

	- v1
		- 每个线程串行处理 1024 个加法计算，多个线程并行处理所有计算
	- v2 
		- 引入了 local_memory 但是并不是实际上的并行，依然是单个线程串行执行所有加法操作
	- v3/4
		- v3有问题
		- v4
			- `__shared__ int local_sum[1024];`
				- 一个 block 内 1024 个线程，共享这一份 `local_sum[1024]`
				- 一个 block 处理 1024 个数据的相加
			- `__syncthreads();`
				- 需要等 block 中的所有线程同步后再往后运行，否则会出问题
			- 这里显然还有个问题就是，就是线程的工作量是不匹配的，有些线程需要做好多次，有些只用做1次
	- v5
		- 只是对 v4 的小修改，不要先拷贝了，直接就开加，同时可以缩小每个 block 中的 线程数到  就好
		- 提速还挺明显
	- v6
		- 引入 warp 的概念，以减少 block 级别的等待，warp 是GPU的最小调度单位
		- 我们实验机器的 warpSize=64，`warpSize` 是某个 GPU 架构规定的 **一个 warp 包含多少个线程**，它是硬件/ISA/编程模型共同约定下来的执行粒度
		- warp 中每个线程在这个 warp 内的位置，通常称为一个 **lane**
		- `__shfl_down_sync(mask, value, offset)`
			1. mask 中所有指定的 lane 到达这一条 shuffle 指令 
			2. 这些 lane 之间交换寄存器数据
		- 例子 `__shfl_down_sync(FULL_MASK, value, 32)`

		```text
							lane 0
							lane 1
							...
							lane 63
							   │
							   │ 全部到达
							   ▼
							shuffle(offset = 32)
							   │
							   ├─ lane 0 读取 lane 32
							   ├─ lane 1 读取 lane 33
							   └─ ...
		```

	- v7
		- 增添灵活性，但是最终的效率其实不如 v6

- 最终采用 v6 版本✅
```
parallelv1 --- avgTime: 0.001265; minTime: 0.000817
parallelv2 --- avgTime: 0.001156; minTime: 0.000749
parallelv3(v4的降级版可能会有错误)
parallelv4 --- avgTime: 0.002337; minTime: 0.000251
parallelv5 --- avgTime: 0.000433; minTime: 0.000125
parallelv6 --- avgTime: 0.000415; minTime: 0.000122
parallelv7 --- avgTime: avgTime: 0.000546; minTime: 0.000250
```


- 提交作业的时候犯了很愚蠢的错误
	- `zip program.zip program` 是单文件压缩
	- `zip -r program.zip program/` 才是文件夹压缩
	- 我还疑惑为啥总是执行失败 😂


