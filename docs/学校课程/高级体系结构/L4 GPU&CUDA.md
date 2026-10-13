
>[!notice] 本节重点
>- 本节主要讲 CUDA 编程 和 GPU 的硬件架构，重点在于理解 GPU 的底层体系结构，并串联理解 软件抽象 和 硬件逻辑
>- 最重要的应该是 GPU体系结构 小节
## GPU 微架构演变

GPU 简史

- CPU + 加速卡 1970-1999
- GPU 固定功能渲染pipeline 1999-2006
	- 开发者只能通过开关和参数（如 `glLight*`、`glTexEnv*`、`glFog*` 等）调整状态，不能改变算法本身
- GPU 可编程pipeline 2006-now
	- 顶点着色器、几何着色器、片元/像素着色器等由开发者用 GLSL、HLSL、MSL 等编写，运行在 GPU 上
	- 2002 --- GPGPU General Program GPU
		- 能够适配科学计算，但是需要研究人员很熟悉GPU
	- 2004 --- 流式处理器思维
		- CPU (程序programs) + GPU(设备核函数kernels)
	- 2006 --- CUDA诞生
		- 为了流式处理器方便写kernels，而研制出 CUDA 语言进行编程(C/C++通用API)
		- MXMACA 是沐曦类似CUDA的编程语言


## GPU 体系结构

执行模型

- ILP 指令并行 vs SIMT线程并行
	- CPU 是 ILP 指令并行，乱序执行与分支预测的指令级并行
	- GPU 是 SIMT 单指令多线程，单指令多线程并行
	- ![](images/L4/5.png)

GPU体系结构(硬件视角)

- warp/wave 调度
	- ==warp 是 GPU并行调度的最小粒度==，通常32/64线程，以 束 为单位同步发射和执行
- SIMT执行模型
	- 同一warp/wave 内的所有线程在同一时钟周期执行同一条指令，但是可以处理不同的数据
	- warp 分支分化，由于 warp 中的所有线程执行同一条指令，在处理 `if-else` 时，就可能产生分支，变成先执行 `if`分支(不执行的线程等待)，再执行 `else` 分支(不执行的线程等待) 的执行模式
- SM/AP架构
	- 由大量精简的 Streaming Processor(CUDA core)组成，舍弃了复杂控制逻辑与缓存层级
	- 以 NVIDIA Fermi 架构为例
	- ![](images/L4/2.png)
	- 例如
		- 1个SM中有 4 个 warp 调度器，64 个CUDA core，分成2组，每组 32 个 CUDA core(warp 有 32 个线程)，还有寄存器文件和共享内存等等
		- 该 SM 同时可执行 2 个 warp，假设1个Block有256个线程(8个warp)装载进该SM，那么它会调度这8个warp流水线执行，形成 warp内并行，warp间(可能)并发
		


GPU的线程层次(软件视角)

- Grid -> Device(整个GPU)
- Thread Block -> Streaming Multi-Processor(SM)
	- 调度层面由 Warp 实现
	- 1个SM可以装载多个Block，1个Block只能装载到1个SM(不能分装)
- Thread -> Scalar Processor(CUDA core)

=== "软件抽象"
	![](images/L4/1.png)

=== "CPU&GPU"
	![](images/L4/6.png)
	- 现代 CPU 和 GPU 间也有更高速的连通方式不仅仅是PCIe

=== "存储结构"
	![](images/L4/3.png)

=== "索引"
	![](images/L4/4.png)