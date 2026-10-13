---
comments: "true"
---
- [来源](https://docs.nvidia.com/cuda/cuda-programming-guide/02-basics/writing-cuda-kernels.html) ✅ 整理自 CUDA C++ Programming Guide「Writing CUDA SIMT Kernels」

## Writing CUDA SIMT Kernels（编写 CUDA SIMT 内核）

- CUDA C++ kernel 的写法与普通 CPU 代码类似，但要考虑 GPU 特有的几点以榨取性能：
	- 线程调度（thread scheduling）
	- 内存访问模式（memory access patterns）
	- 执行模型（execution model）

### SIMT 基础（Basics of SIMT）

- **CUDA 线程**是并行性的基本单元，每个线程维护自己的状态（PC、寄存器），可有自己的控制流
- **warp** 是 GPU 的基本执行单元，通常由 **32 个线程**组成
- SIMT = Single Instruction, Multiple Threads（单指令多线程）
- warp 内的线程应避免==发散（divergent）==代码路径：发散会使各分支被串行化执行，降低并行效率
- ==SIMT 与 CPU SIMD 的区别==
	- SIMT：用多个独立线程执行同一指令，采用标量式（scalar）编程
	- SIMD：一条指令操作多个数据元素，采用显式向量（vector）编程

### 内核规范（Kernel Specification）

- 函数限定符（function qualifiers）：
	- `__global__`：在 device 上执行，从 host（或 device）调用；kernel 返回类型为 ==void==
	- `__device__`：在 device 上执行，只能从 device 调用；通常被内联
	- `__host__`：在 host 上执行，从 host 调用；可与 `__device__` 组合（同时生成 host/device 两个版本）

	```c++
	__global__ void vecAdd(float* A, float* B, float* C) { ... }
	```

### 线程层次（Thread Hierarchy）

- 层次结构：==grid（网格）→ thread block（线程块）→ thread（线程）==
- grid 与 block 都可以是 1D / 2D / 3D
- 内建变量：
	- `gridDim.x/y/z`：网格各维大小（内核启动时设定）
	- `blockDim.x/y/z`：块各维大小（内核启动时设定）
	- `blockIdx.x/y/z`：当前块的索引（执行过程中变化）
	- `threadIdx.x/y/z`：当前线程在块内的索引（执行过程中变化）
- 多维 grid / block ==只是编程上的便利，不影响性能==：只要总线程数与内存访问模式相同，性能就相同
- 线程线性化顺序（C 行优先）：==x 变化最快==，然后 y（步长 `blockDim.x`），再 z（步长 `blockDim.x * blockDim.y`）
- 相邻的 `threadIdx.x` 值属于同一个 warp（warp 内线程执行同一指令）

### 设备内存空间（GPU Device Memory Spaces）

#### 全局内存（Global Memory）

- 所有线程都可访问的主内存，类比 CPU 的系统 RAM
- 通过 `cudaMalloc` / `cudaMallocManaged` 分配；通过 `cudaMemcpy` 在 CPU 与 GPU 之间传输
- kernel **不创建内存**：全局内存在 kernel 执行前就已分配并初始化
- 数据在 kernel 结束后仍持久存在；`cudaDeviceReset` 会释放分配
- 性能取决于==对齐与合并（coalesced）访问==：warp 内线程应访问连续内存地址才能达到峰值带宽
- 典型示例：矩阵转置（out-of-place），用 2D grid + 32×32 的 block

#### 共享内存（Shared Memory）

- 用 `__shared__` 声明
- ==每个线程块一份==，生命周期与块相同，仅块内线程可访问
- 片上内存，低延迟高带宽（远快于全局内存）
- 典型使用模式：
	1. 声明 per-block 的共享缓冲区
	2. 把数据载入共享内存
	3. `__syncthreads()`
	4. 在共享数据上计算
	5. 再次 `__syncthreads()`
	6. 把结果写回全局内存
- `__syncthreads()` 阻塞直到块内==所有线程到达同一调用点==；在条件语句中使用时，需保证块内所有线程都能到达同一同步点
- 分配方式：
	- 静态：`__shared__ float tile[TILE_DIM][TILE_DIM];`
	- 动态：`extern __shared__`，在内核启动时指定大小
- 与 L1 cache 共享物理内存（如 64KB，可配置为 16/48、32/32、48/16 等）
- ==bank conflict（bank 冲突）==应避免：常用技巧是加 padding，如 `tile[TILE_DIM][TILE_DIM+1]`

#### 常量内存（Constant Memory）

- 用 `__constant__` 声明
- 只读，适合所有线程读取相同数据的场景
- 数据物理上在全局内存，但通过专门的==常量缓存（constant cache）==访问
- 当 warp 内所有线程==请求同一地址==时最有效；否则访问会被串行化
- host 用 `cudaMemcpyToSymbol()` 初始化
- 不能动态分配；通常为 64KB

#### 纹理内存（Texture Memory）

- 只读，通过纹理获取（texture fetch）访问
- 数据在全局内存，但通过专门的==纹理缓存（texture cache）==访问
- 纹理硬件提供高效的地址解析与特殊支持：
	- 越界地址处理（out-of-bounds address handling）
	- 浮点插值（interpolation）
	- 类型转换与位操作
- 典型步骤：
	1. `cudaMalloc()` 分配全局内存
	2. 创建纹理引用，如 `texture<float, 1> texRef;`
	3. 用 `cudaCreateChannelDesc<datatype>()` 创建通道描述符
	4. 用 `cudaBindTexture()` 绑定
	5. kernel 中用 `tex1Dfetch()` / `tex1D()` / `tex2D()` / `tex3D()` 访问
	6. 用 `cudaUnbindTexture()` 解绑
- Kepler 及之后，可用 `__ldg()` 或 `const __restrict__` 指针使用只读缓存，无需显式纹理绑定

### 原子操作（Atomics）

- 原子函数对全局/共享内存位置执行==读-改-写（read-modify-write）==操作，不受其他线程干扰
- 算术原子：`atomicAdd`、`atomicSub`、`atomicExch`、`atomicMin`、`atomicMax`、`atomicInc`、`atomicDec`、`atomicCAS`
- 位运算原子：`atomicAnd`、`atomicOr`、`atomicXor`
- 出于效率考虑应谨慎使用、能避免则避免；但对某些共享数据的更新与同步场景是必需的
