---
comments: "true"
---
- 来源：[Introduction](https://docs.nvidia.com/cuda/cuda-programming-guide/01-introduction/introduction.html) / [Programming Model](https://docs.nvidia.com/cuda/cuda-programming-guide/01-introduction/programming-model.html) / [The CUDA platform](https://docs.nvidia.com/cuda/cuda-programming-guide/01-introduction/cuda-platform.html) ✅ 整理自 CUDA C++ Programming Guide 第一章「Introduction」下的三个小节（1.1 / 1.2 / 1.3）

## 1.1 Introduction（引言）

### 使用 GPU 的好处（The Benefits of Using GPUs）

- GPU 专门用于==计算密集、高度并行==的计算（最初即图形渲染），因此设计上把更多晶体管用于**数据处理**，而非**数据缓存和流程控制**
- CPU 与 GPU 的设计取向对比
	- CPU：少量复杂核心，投入大量晶体管到复杂的控制逻辑与缓存，==优化延迟（latency）==
	- GPU：大量简单核心，==优化吞吐（throughput）==
- GPU 尤其适合具有==高算数强度（high arithmetic intensity）==的问题，即**算术运算次数 / 内存访问次数**比值很大的问题
	- 数据并行（data-parallel）：同一段程序在大量数据元素上并行执行，流程控制需求低
	- 高算数强度意味着访存延迟可以被大量计算所隐藏，因此不必像 CPU 那样依赖大缓存

### CUDA：通用并行计算平台与编程模型（A General-Purpose Parallel Computing Platform and Programming Model）

- CUDA（Compute Unified Device Architecture）= 通用并行计算平台 + 编程模型
	- 利用 NVIDIA GPU 的并行计算引擎，比纯 CPU 更高效地解决复杂计算问题
- CUDA 通过**对编程语言做最小扩展** + 提供 API 来实现
	- 主要面向 C/C++ 程序员，也支持 Fortran、Python 等
	- 配套提供 C++ 语言扩展、Runtime API、Driver API
- 核心是==三个关键抽象==，作为对编程语言的最小扩展暴露给程序员：
	1. **线程组层次结构（hierarchy of thread groups）**
	2. **共享内存（shared memories）**
	3. **栅栏同步（barrier synchronization）**
- 这三个抽象提供：
	- ==细粒度（fine-grained）数据并行 + 线程并行==
	- 嵌套在==粗粒度（coarse-grained）数据并行 + 任务并行==之中
- 它们引导程序员把问题划分为：
	- 可被**线程块（block）独立并行**解决的粗粒度子问题
	- 每个子问题再细分为由**块内线程协作并行**解决的细粒度部分
- 每个线程块可以被调度到任意可用的多处理器（multiprocessor / SM）上执行
- ==可扩展性==：编译好的程序可以运行在**任意数量**的多处理器上，只有运行时系统（runtime system）需要知道物理多处理器的数量

### 可扩展的编程模型（A Scalable Programming Model）

- 多线程 CUDA 程序被划分为多个**彼此独立执行**的线程块
- 线程块必须能够==以任意顺序、并行或串行==执行，互不依赖
- 这种独立性使硬件可以在**任意数量的核心（SM）上以任意顺序**调度线程块
- 由此，GPU 架构只需通过**扩展多处理器与内存分区的数量**即可覆盖从移动端到超算的广阔市场
- ==一次编译，即可运行在任意数量的 SM 上==

### 文档结构（Document Structure）

- CUDA C++ Programming Guide 按以下几个部分组织：
	1. **Introduction and Programming Model Abstract** —— 语言无关的编程模型总览 + CUDA 平台简介
	2. **Programming GPUs with CUDA**（C++、Python）
	3. **Advanced CUDA** —— 高级特性
	4. **CUDA Features** —— 具体 CUDA 特性（如 Cooperative Groups、动态并行、统一内存等）
	5. **Technical Appendices** —— 技术附录

## 1.2 Programming Model（编程模型）

> 本节是语言无关的「编程模型摘要（Programming Model Abstract）」，从抽象层面介绍 kernel 如何被组织为线程网格来执行。

### Kernels（内核）

- CUDA C++ 通过允许程序员定义称为**内核（kernel）**的函数来扩展 C++
	- kernel 被调用时，由 N 个不同的 CUDA 线程并行执行 N 次
	- 用 `__global__` 声明，用 `<<<...>>>` 执行
- 每个线程都有一个唯一的线程 ID，可通过内建变量访问

### 线程层次（Thread Hierarchy）

- `threadIdx` 是一个 **3 分量向量**（`.x` / `.y` / `.z`）
	- 线程可以用 1D / 2D / 3D 的索引来标识，自然地对应**向量 / 矩阵 / 体（volume）**的计算
- **线程块（thread block）**：线程被组织成块
	- 目前一块最多 **1024** 个线程
	- 块维度乘积 ≤ 1024，例如 `(1024, 1, 1)`、`(32, 32, 1)` 等，z 维最大 64
- `blockIdx` 也是一个 3 分量向量
- **网格（grid）**：线程块被组织成 1D / 2D / 3D 的网格
	- 线程块数量通常由数据规模决定，往往远超处理器数量
- ==线程块独立执行==：块之间可任意顺序调度（并行或串行），因此不能依赖块间的执行顺序
- 块内线程通过 ==共享内存 + `__syncthreads()`== 协作
- **warp**：线程被组织成以 **32 个线程**为单位的 warp，按 ==SIMT（Single Instruction, Multiple Thread，单指令多线程）==方式执行
	- 分支发散（divergence）时，不同路径的线程被屏蔽（mask）后分别执行
- **线程块簇（Thread Block Clusters）**：计算能力 ≥ 9.0 时，可选地把多个块组成簇，支持跨块同步与分布式共享内存

### SIMT（单指令多线程）

- SIMT = Single Instruction, Multiple Thread（单指令多线程）
- SM 以 **32 个并行线程**为一组，称为==warp==，来创建、管理、调度和执行线程
- warp 中的线程从同一程序地址一起开始，但每个线程有==自己的指令地址计数器（instruction address counter）和寄存器状态==，因此可以自由分支、独立执行
- 一个 warp ==一次只执行一条公共指令==（lockstep，锁步执行）
	- 只有当 warp 的 32 个线程执行路径一致时，才能达到满效率
- ==分支发散（divergence）==：当 warp 内线程因数据相关的条件分支而发散时
	- warp 会==串行执行每一条被采用的分支路径==，同时屏蔽（mask 掉）不在该路径上的线程
	- 所有路径执行完后，线程==重新汇聚（reconverge）==到同一条执行路径
	- 发散只发生在==同一个 warp 内部==；不同 warp 之间独立执行
- ==SIMT 与 SIMD 的对比==
	- SIMD：把 SIMD 宽度直接暴露给软件（程序员显式操作向量）
	- SIMT：程序员编写线程级并行的标量线程代码，无需显式考虑向量宽度
	- 对正确性而言，程序员基本可以忽略 SIMT 行为；但==减少发散能显著提升性能==
- ==Volta 起的独立线程调度（Independent Thread Scheduling）==
	- Volta 之前：一个 warp 的 32 个线程共享一个程序计数器（PC）+ 一个活动掩码（active mask）
	- Volta 及之后：为每个线程维护独立的执行状态（含 PC 和调用栈），使线程间完全并发，不再依赖 warp 的统一性
- warp 调度
	- 每个 warp 的执行上下文（PC、寄存器等）在整个 warp 生命周期内==保存在片上==，因此上下文切换零开销
	- 每个指令发射时刻，warp 调度器选择一个有就绪线程的 warp，把下一条指令发射给这些活动线程

### 内存层次（Memory Hierarchy）

- 每个线程：私有的==局部内存（local memory）==
- 每个线程块：==共享内存（shared memory）==，块内所有线程可见，生命周期与块相同（声明为 `__shared__`）
- 所有线程：==全局内存（global memory）==，位于 GPU DRAM，可被所有线程和主机访问，跨 kernel 启动持久，由 L2（可选 L1）缓存
- 另外两类只读内存空间：
	- ==常量内存（constant memory）==
	- ==纹理内存（texture memory）==
- 全局 / 常量 / 纹理内存针对不同用途做了优化，且==在同一个应用程序的多次内核启动之间保持持久==

### 异构编程（Heterogeneous Programming）

- CUDA 编程模型假设 CUDA 线程运行在==物理上独立的设备（device）==上，作为主机（host）CPU 的协处理器（coprocessor）
	- kernel 在 GPU 上执行，C++ 程序其余部分在 CPU 上执行
	- ==程序总是从主机（CPU）开始执行==：主机分配/拷贝数据、启动 kernel、再等待结果
- 主机与设备维护**各自独立的内存空间**
	- 串行代码跑在 host 上，并行代码跑在 device 上
	- 通过 Runtime 管理设备内存：`cudaMalloc` / `cudaFree` / `cudaMemcpy` / `cudaMemset` 等
	- CPU 与 GPU 可==并发执行==以充分利用两者
- 在 64 位系统上，CUDA 提供==统一虚拟地址空间（unified virtual address space）==，host 与 device 指针共享同一地址范围
- ==统一内存（Unified Memory / Managed Memory）==：
	- 由 `cudaMallocManaged` 或 `__managed__` 分配
	- 把 host 与 device 内存桥接为==单一一致的内存映像==，由驱动自动管理数据移动

### Tile 编程（Tile Programming in CUDA）

- Tile 编程允许程序员在**整个线程块（block）**的层面上编写代码，对被称为==tile（瓦片）==的多维数据集合描述操作，由编译器把操作映射到块内的各个线程
- Tile kernel 在块的网格上启动，程序员**只指定网格维度**；每块的线程数由编译器根据 kernel 中的 tile 操作决定
- Tile kernel 内部，整个块执行==单一控制流==，因此**没有 warp 发散（divergence）**的概念
	- 标量操作由单个线程执行
	- tile 操作由所有线程协作并行执行
- 关键区分：==block 是执行的单元，tile 是数据的单元==
	- 一个块可以创建并操作多个不同形状、不同数据类型的 tile

#### 数组与 tile（Arrays and Tiles）

- **数组（array）**：存储在设备内存中的多维容器（全局数组），可变，可通过 store 操作修改
- **tile**：只存在于 tile 代码中的多维值集合，局部于单个块，且==不可变（immutable）==——每个操作都产生一个新 tile
- tile 不一定有内存表示：编译器可能用寄存器、共享内存或其他 SM 资源来存放
- 每个 tile 维度必须是==2 的幂==，且在编译期已知；tile 不能作为 kernel 参数传递

#### Tile 空间与数据移动（Tile Space and Data Movement）

- 数据通过==load / store 操作==在数组与 tile 之间移动
- ==tile 空间（tile space）==：把数组概念性地划分为等大、互不重叠的 tile 的结果
	- 形状 (M, N) 的数组、tile 形状 (tm, tn) → 划分为 ⌈M/tm⌉ × ⌈N/tn⌉ 个 tile
- store 操作把 tile 写回数组的对应区域；超出数组边界的写入被**静默丢弃**
- ==gather / scatter== 操作支持从/向任意位置加载/存储

#### Tile 上的操作（Operations on Tiles）

- 内建 tile 操作包括：逐元素算术、矩阵乘法、沿轴归约（求和、求最大值）、形状变换（reshape、transpose）、类型转换
- 两个不同形状的 tile 组合时，较小的 tile 会自动==扩展（broadcast）==以匹配较大的 tile

#### 与 SIMT 编程的关系（Relationship to SIMT Programming）

- Tile 编程与 SIMT 编程**共存**：一个应用程序可以同时包含两种 kernel，操作同一块设备内存
- 选择哪种编程模型是**每个 kernel 独立决定**的
- Tile 编程**不取代** SIMT——某些算法仍需要 SIMT 提供的细粒度线程控制
- 二者构建在相同的底层硬件（SM、线程块、网格）之上，使用相同的设备内存空间


## 1.3 The CUDA platform（CUDA 平台）

### 计算能力（Compute Capability）

- 每块 NVIDIA GPU 都有一个==计算能力（Compute Capability）==版本号，格式为 `X.Y`（如 `12.0`）
	- `X`：主版本，表示架构代际（major architecture generation）
	- `Y`：次版本，表示同代内的增量改进
- 计算能力表示该 GPU 支持哪些特性等参数规格
- 计算能力与==流式多处理器版本（SM version）==一一对应，记作 `sm_XY`（如计算能力 `12.0` → `sm_120`）
- `sm_XY` 用于标记 GPU 二进制文件；计算能力可在程序中通过 API 查询

| 计算能力 | 架构           | 代表 GPU         |
| ---- | ------------ | -------------- |
| 7.0  | Volta        | V100           |
| 7.5  | Turing       | T4 / RTX 2080  |
| 8.0  | Ampere       | A100           |
| 8.6  | Ampere       | RTX 3090 / A10 |
| 8.9  | Ada Lovelace | RTX 4090 / L40 |
| 9.0  | Hopper       | H100           |
| 10.0 | Blackwell    | B100 / B200    |
| 12.0 | Blackwell    | RTX 50 系列      |

### NVIDIA 驱动（Driver）

- 被称为「GPU 的操作系统」
- 所有 GPU 使用都需要它，包括显示/图形，以及其他 API（Vulkan、Direct3D 等）

### CUDA Toolkit

- 独立的软件产品，包含用于 GPU 计算的库、头文件与工具

### CUDA Runtime

- Toolkit 中的一个库，提供 API 与语言扩展，用于==内存分配、数据拷贝、内核启动==等

### Runtime API vs Driver API

- ==CUDA Runtime API 构建在更底层的 CUDA Driver API 之上==
	- Driver API 由 NVIDIA 驱动直接暴露
	- 应用程序可以使用其中一种，或两者混用
	- 某些特性仅能通过 Driver API 访问

### PTX（Parallel Thread Execution）

- PTX 是一种==虚拟指令集架构（virtual ISA）==，也是面向 NVIDIA GPU 的高级汇编语言
- 它是真实 GPU 硬件 ISA 之上的一层**抽象**
- 编译器或领域特定语言（DSL）可以把 PTX 作为中间表示（IR）生成，之后可==离线编译或 JIT 编译==成可执行的 GPU 二进制
- PTX 有版本号，与计算能力对应（如支持计算能力 8.0 的 PTX 称为 `compute_80`）

### nvcc 编译器

- `nvcc` 是 NVIDIA CUDA 编译器，一个**编译驱动（compiler driver）**，简化 CUDA C++ / PTX 的编译流程
	- 分离 host 代码与 device 代码
	- 把 device 代码编译为 PTX 和/或 `cubin` 形式
	- 把 `<<<...>>>` 内核启动语法替换为 CUDA Runtime 调用
- 相关编译选项：
	- `-arch`、`-code`、`-gencode` 控制嵌入应用程序的 PTX / 二进制版本

### cubin 与 fatbin

- CUDA C++ 代码通常先编译成 PTX，再编译成特定架构的二进制，称为==cubin==
- ==fatbin== 是一个容器，可同时容纳多个 cubin 与 PTX 版本，以获得更广的兼容性
- cubin 是**架构特定**的
	- 二进制兼容性：==同一主版本内==，只要目标次版本 >= 源次版本即可；**跨主版本不兼容**

### Runtime、Driver 与 JIT 编译

- 应用程序在运行时加载的 PTX 会被设备驱动==JIT 编译==为二进制代码
- 编译结果缓存在**计算缓存（compute cache）**中，驱动升级时会自动失效
- 除 `nvcc` 外，==NVRTC== 可在运行时把 CUDA C++ device 代码编译为 PTX
- CUDA 驱动对已编译应用程序保持向后二进制兼容；每个 CUDA Toolkit 版本要求一个最低驱动版本
