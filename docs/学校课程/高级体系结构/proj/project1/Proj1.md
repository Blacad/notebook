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

#### kernel.cpp：无原子分段求和与逐步优化

参考源码：[[高级体系结构/proj/project2/kernel.cpp|kernel.cpp]]、[[高级体系结构/proj/project2/kernel1.cpp|kernel1.cpp]]；基础概念见 [GPU 并行编程的基本知识](../../program/2%20CUDA%20C++%20Intro.md)。

##### 1. 先明确输出与索引

- GPU 并不是直接写出一个总和，而是把输入按 **1024 个元素一组**，输出 `groups = n / 1024` 个分段和：

  ```cpp
  sum[i] = arr[i * 1024] + ... + arr[i * 1024 + 1023];
  ```

- CPU 再把 `sum` 中的分段和相加。不同分组写入不同的 `sum[i]`，所以不需要原子操作；一个分组内部则通过线程协作完成归约。
- 这些实现都使用整数除法 `n / 1024`，**不会处理不足 1024 个元素的输入尾段**，因此“完整数组求和”以 `n` 是 1024 的倍数为前提。`kernel1.cpp` 对尾块中多出来的无效分组做了保护，但不是在处理输入尾段。
- 本项目使用一维 grid/block：`blockIdx.x` 是 block 编号，`threadIdx.x` 是 block 内线程编号，`blockDim.x` 是每块线程数，`gridDim.x` 是块数。一次 kernel launch 对应一个 grid，线程与数据的映射随版本变化：

| 版本       | 一组 1024 个元素由谁处理             | 每线程读取元素数 | 启动配置 / 归约方式                               |
| -------- | --------------------------- | -------- | ----------------------------------------- |
| V1 / V2  | 一个线程                        | 1024     | 线程级 grid-stride loop；V2 是线程私有数组           |
| V3 / V4  | 一个 block，1024 个线程           | 1        | V3 缺同步；V4 共享内存树形归约                        |
| V5 / V6  | 一个 block，512 个线程            | 2        | V5 全程共享内存；V6 最后用 64-lane shuffle          |
| V7       | 一个 block，256 个线程；可循环处理多组    | 4        | block 级 grid-stride loop + 共享内存 + shuffle |
| `sol6.1` | 一个 32-lane 子组；每 block 有 8 组 | 32       | 256 线程/block，寄存器累加 + 子组 shuffle           |

> **源码当前状态：** `kernel.cpp` 中真正启用的是 **V6**，入口使用 `512 threads/block` 和 `n / 1024 blocks`；V1～V5、V7 都是注释掉的备选实现。不能只取消某段 kernel 的注释，而不修改对应的启动参数。`sol6.1` 是另一份完整实现，也定义了同名的核函数与启动入口，应作为替代源码使用，不能把两份同时链接进同一个目标。

##### 2. V1：一个线程串行加 1024 个元素

```cpp
for (int i = blockDim.x * blockIdx.x + threadIdx.x;
     i < n / 1024;
     i += blockDim.x * gridDim.x) {
    int local_sum = 0;
    for (int j = i * 1024; j < i * 1024 + 1024; ++j)
        local_sum += arr[j];
    sum[i] = local_sum;
}
```

- `i` 是分组编号，不是原数组元素编号。全局线程编号决定第一组，每次跨过整个 grid 的线程总数，继续处理下一组。
- 组与组之间并行，但每组内部仍是一条连续的累加依赖链。
- 同一轮读取中，相邻线程访问的输入地址相差 1024 个 `int`，不如后续版本相邻线程读取相邻元素的布局。
- 与 V4～V6 不同，V1/V2 的 block 数应按“需要处理的分组数 / 每块线程数”向上取整；如果仍沿用当前入口的 `n / 1024 blocks`，虽然可以覆盖分组，却会启动大量不执行求和的线程。

##### 3. V2：线程私有数组与分阶段归约

- 每个线程先把本组元素读入 `int local_sum[1024]`，再按 `1024 → 512 → 256 → ... → 1` 逐层两两相加。
- 同一层的不同 `j` 之间没有数据依赖，给编译器提供了安排独立操作的空间；但代码仍由**一个线程**执行，并没有让 1024 个线程共同求和。
- `local_sum` 是线程私有数组，**不是 `__shared__` 共享内存**。源码不能保证这个大数组全部放在寄存器里，实际存储与开销取决于编译结果；不能把 V2 简单理解成“用了更快的 local memory”。
- 相比 V1，它改变的是单线程内的归约组织方式，不是线程与分组的映射，也不保证一定更快。

##### 4. V3 / V4：一个 block 协作，关键是同步

```cpp
__shared__ int local_sum[1024];
int j = threadIdx.x;
int i = blockIdx.x;
local_sum[j] = arr[i * 1024 + j];
```

- 这里把 V2 的线程私有数组变成 **block 内共享的数组**，把循环变量 `j` 换成线程编号，一组由 1024 个线程共同处理。
- 相邻线程读取相邻输入元素，随后每层只有 `j < stride` 的线程负责相加。
- **V3 的问题不是求和公式错，而是缺少同步：** 有的线程可能还没把输入写入共享内存，别的线程就开始读取；上一层还没写完，下一层也可能已经开始。跨 warp 的线程不能假设自动同步，因此存在数据竞争。
- V4 在加载之后、每轮归约之后加入 block 级屏障：

  ```cpp
  __syncthreads();
  for (int stride = 512; stride > 0; stride /= 2) {
      if (j < stride)
          local_sum[j] += local_sum[j + stride];
      __syncthreads();
  }
  if (j == 0)
      sum[i] = local_sum[0];
  ```

- `__syncthreads()` 必须放在这里的条件分支**外面**，让 block 内所有线程到达屏障，而不是只让参与相加的线程调用。
- V4 修复了同步，但活跃线程数每轮减半；很多线程后面只等待、不计算。该写法有一次加载后的屏障和十轮归约后的屏障。
- **启动必须是 1024 threads/block。** 如果直接沿用当前入口的 512 个线程，`local_sum[512..1023]` 没有初始化，即使加了同步，结果也不正确。

##### 5. V5：加载时先加一对，缩小 block

```cpp
__shared__ int local_sum[512];
int base = blockIdx.x * 1024;
local_sum[tid] = arr[base + tid] + arr[base + tid + 512];
```

- 每个线程读取两个元素并先相加，把第一层 `1024 → 512` 融合进加载阶段，不再先向共享内存写入全部 1024 个原始值。
- 每块只需 **512 个线程、512 个共享 `int`**；后续从 `stride = 256` 开始归约。
- 与 V4 相比，共享内存从 4096 字节减到 2048 字节，块内线程数减半，归约阶段也少了一轮屏障；但后续仍然有逐层等待和活跃线程递减的问题。

##### 6. V6：共享内存归约到 64，再用 warp shuffle

当前启用的 V6 仍让 512 个线程各读两个元素，过程是：

```text
1024 个输入
    ↓ 每线程读两个并相加
512 个共享部分和
    ↓ 共享内存归约 + block 屏障
256 → 128
    ↓ tid < 64 的线程各读一对共享值，放入寄存器
64 个寄存器部分和
    ↓ shuffle，offset = 32, 16, 8, 4, 2, 1
lane 0 写入 sum[blockIdx.x]
```

- 这份实现按实验机器 **`warpSize = 64`** 编写：一个 warp 中的线程位置称为 lane，`warpReduce` 通过 `__shfl_down_sync(0xffffffffffffffffULL, value, offset)` 交换 lane 的寄存器值。
- 例如 `offset = 32` 时，lane 0 读取 lane 32 的值并加到自身，lane 1 读取 lane 33；随后逐步减小 offset，最后 **lane 0** 得到 64 个值的总和。不是每个 lane 都得到完整总和。
- mask 指定参与的 lane；这些 lane 必须按要求执行对应的 shuffle。shuffle 不是“让整个 block 同步”，也不能替代跨 warp 共享内存访问所需的屏障。

**`warpReduce` 的代码与作用。**

下面保留源码的计算逻辑，用具名常量说明 mask：

```cpp
__device__ __forceinline__ int warpReduce(int value)
{
    constexpr unsigned long long FULL_MASK = 0xffffffffffffffffULL;

    value += __shfl_down_sync(FULL_MASK, value, 32);
    value += __shfl_down_sync(FULL_MASK, value, 16);
    value += __shfl_down_sync(FULL_MASK, value, 8);
    value += __shfl_down_sync(FULL_MASK, value, 4);
    value += __shfl_down_sync(FULL_MASK, value, 2);
    value += __shfl_down_sync(FULL_MASK, value, 1);

    return value;
}
```

- `__device__`：这个辅助函数在 GPU 上运行，由设备代码调用，不是 CPU 上执行的函数。
- `__forceinline__`：要求编译器尽可能内联，把辅助函数展开到调用处，避免额外的函数调用开销；它不负责线程同步。
- 参数 `value` 是**当前线程自己的寄存器值**。64 个 lane 同时执行同一段函数代码，但各自持有不同的 `value`，不是共同修改一个共享变量。
- 每一行分成两步：先通过 shuffle 取得另一个 lane 在该轮的值，再执行当前 lane 的加法。**shuffle 只传递数据，不做求和**；求和来自外面的 `value +=`。
- offset 每次减半，六轮完成树形归约。假设调用前 lane `k` 的值是 `x[k]`，lane 0 的有效归约结果依次为：

  ```text
  offset = 32：x[0] + x[32]                                  （2 个值）
  offset = 16：x[0] + x[16] + x[32] + x[48]                  （4 个值）
  offset =  8：x[0] + x[8] + x[16] + ... + x[56]             （8 个值）
  offset =  4：x[0] + x[4] + x[8]  + ... + x[60]             （16 个值）
  offset =  2：x[0] + x[2] + x[4]  + ... + x[62]             （32 个值）
  offset =  1：x[0] + x[1] + x[2]  + ... + x[63]             （64 个值）
  ```

**为什么 `offset = 16` 时，lane 0 不只是加上 `x[16]`？**

关键是：**shuffle 读取的是对方 lane 在这一轮传入的当前 `value`，而不是归约开始前的原始值 `x[k]`。** 所有参与 lane 都在逐轮更新自己的部分和，不是只有 lane 0 在计算。

```text
开始时：
    lane 0  的 value = x[0]
    lane 16 的 value = x[16]

第一轮 offset = 32 后：
    lane 0  的 value = x[0]  + x[32]
    lane 16 的 value = x[16] + x[48]

第二轮 offset = 16：
    lane 0 只从 lane 16 读取一次，
    但读到的已经是 lane 16 的部分和 x[16] + x[48]。

    lane 0 的新 value
        = lane 0 的当前 value + lane 16 的当前 value
        = (x[0] + x[32]) + (x[16] + x[48])
```

因此，lane 0 不是在第二轮额外读取了 lane 32 和 lane 48，而是**把两个已经归约过的部分和合并**。每轮有效部分和覆盖的原始值数量翻倍，依次为 `2 → 4 → 8 → 16 → 32 → 64`。

**“lane 全部到位后才往后执行”具体指什么？**

- 对 `__shfl_down_sync` 而言，准确说法是：**mask 指定的所有尚未退出的 lane，都必须到达同一轮对应的 shuffle 调用，并使用相同的 mask；参与线程会在此汇合，完成寄存器交换后，才能继续执行依赖该返回值的加法。** 这里的“到位”是到达该次 shuffle 指令，不是到达函数入口或 kernel 结尾。参见 [CUDA：Warp `__sync` Intrinsic Constraints](https://docs.nvidia.com/cuda/cuda-programming-guide/05-appendices/cpp-language-extensions.html#warp-sync-intrinsic-constraints)。
- V6 使用 64 位全 1 的 `FULL_MASK`，且第一个 warp 的 64 个 lane 全部参与，所以在本实现中可以理解为“**这 64 个 lane 全部到位，才能完成这一轮 shuffle 并往后计算**”。不能让 mask 中某个仍在执行的 lane 跳过对应调用。
- 到达第二轮 `offset = 16` 的 shuffle 时，各参与 lane 已经计算出自己第一轮更新后的 `value`，因此 lane 0 读取的是 lane 16 的 `x[16] + x[48]`，而不是旧的 `x[16]`。
- 这不代表 warp 中的**每一条普通指令**都自带这样的同步，也不代表整个 block 一起等待。一般情况下应以 mask 指定的参与 lane 为准；shuffle 的汇合也不是共享内存屏障，跨 warp 的共享内存依赖仍需要前面的 `__syncthreads()`。

- `return value` 返回每个线程各自的最终值；调用方只使用 lane 0 的总和。V6 中的实际调用位置是：

  ```cpp
  // 此前的 block 屏障保证 local_sum[0..127] 已经准备好。
  if (tid < warpSize) {  // 本实现 warpSize=64，第一整个 warp 都参与。
      int value = local_sum[tid] + local_sum[tid + 64];
      value = warpReduce(value);
      if (tid == 0)
          sum[blockIdx.x] = value;
  }
  ```

- 进入函数前，这 64 个寄存器值已经覆盖本组 1024 个输入；因此 lane 0 的结果就是该组的分段和。它的意义是用寄存器间交换替代最后六轮共享内存归约，而不是重新读取原数组。

**`__shfl_down_sync` 的四个参数。**

调用形式可以写成下面的示意式（不是用来替代平台头文件的函数声明）：

```cpp
__shfl_down_sync(mask, value, delta, width /* 默认 warpSize */)
```

| 参数      | 含义                                                                            | V6 中的取值与作用                                                                                                   |
| ------- | ----------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------ |
| `mask`  | 参与 lane 的位掩码，第 `k` 位对应 lane `k`。被指定且尚未退出的线程必须执行相应的 shuffle，并使用匹配的 mask。       | `0xffffffffffffffffULL` 的 64 位全是 1，表示整个 64-lane warp；`ULL` 表示无符号 `long long` 字面量。它不是输入数据掩码，也不是指定“只保留哪些加法结果”。 |
| `value` | 当前线程提供给其他 lane 读取的值。函数返回另一个 lane 的值，不是返回整个 warp 的数组。                          | 每一轮传入当前线程已经更新过的局部部分和。虽然变量都叫 `value`，每个 lane 的数值仍彼此独立。                                                        |
| `delta` | 源 lane 相对当前 lane 的正向偏移。当前 lane `k` 读取 `k + delta` 的值。                         | 依次为 `32、16、8、4、2、1`。例如 `delta=32` 时，lane 0 读 lane 32，而不是把 lane 0 的值写到 lane 32。                               |
| `width` | 交换范围的子组宽度，省略时为 `warpSize`；取值应为不超过 `warpSize` 的 2 的幂。小于 warp 宽度时，每个连续子组独立交换数据。 | V6 省略此参数，在本机即 64；`kernel1.cpp` 显式写 32，使 lane 0～31 和 lane 32～63 各自归约，不能跨子组读取。                                 |

这里要区分 **mask 管“谁参加”，width 管“在哪个子组里取值”，delta 管“从哪个相对位置取值”**；mask 不会把稀疏的参与 lane 自动重新编号成连续子组。CUDA 的通用参数语义可参见 [Warp Shuffle Functions](https://docs.nvidia.com/cuda/archive/12.5.0/cuda-c-programming-guide/index.html#warp-shuffle-functions)，MACA 的 64-lane 与 mask 位宽适配说明可参见 [沐曦：shuffle 相关接口的使用](https://developer.metax-tech.com/api/client/document/preview/AI4S%20%E5%BA%94%E7%94%A8%E5%BC%80%E5%8F%91%E5%8F%82%E8%80%83/%E7%A7%91%E5%AD%A6%E8%AE%A1%E7%AE%97%E7%94%A8%E6%88%B7%E6%89%8B%E5%86%8C/%E6%9B%A6%E4%BA%91C500%E7%B3%BB%E5%88%97/3.8.0.x/split_files/%E5%B9%B6%E8%A1%8C%E7%BC%96%E7%A8%8B%E5%BA%93%E9%80%82%E9%85%8D.html)。不能把 CUDA 的 32-bit mask 原型直接当作这里 MACA 的 64-bit mask 原型。

**边界与同步注意事项。**

- `down` 不会跨 `width` 边界循环取值：按上述 shuffle 语义，若 `k + delta` 越过子组边界，返回当前 lane 自己的输入值，**不是自动返回 0**。这样外面的 `+=` 会把该 lane 的值翻倍；但树形归约通向 lane 0 的有效路径不会依赖这些越界结果，所以最终只取 lane 0。
- “源 lane 越过子组边界”和“源 lane 在范围内但没有参与执行”是两回事；后者的读取结果未定义。因此不能改成只有 lane 0 调用 `warpReduce`，其余 lane 完全不执行。
- `_sync` 协调 mask 指定的参与线程执行该次寄存器交换，不是 block 屏障，也不为共享内存提供读写屏障。V6 能省掉最后几轮 `__syncthreads()`，是因为此时用完整 warp 的 shuffle 交换寄存器值，不是因为“同一 warp 中的任意共享内存访问都无需同步”。这些边界和同步规则见 [CUDA Warp Shuffle Functions](https://docs.nvidia.com/cuda/archive/12.5.0/cuda-c-programming-guide/index.html#warp-shuffle-functions)。

- V6 保留三次 block 屏障：加载后、`512 → 256` 后、`256 → 128` 后。最后一个 warp 先把 `128 → 64` 的结果读入寄存器，后续用 shuffle，不再逐层通过共享内存交换数据。
- **不能仅把启动线程数改成 256，或直接移植到 32-lane warp。** 此处的 `+512`、`+64`、shuffle 的起始 offset 和 64-bit mask 都是与当前布局配套的；跨架构时需要重新核对实现与 API 行为。

##### 7. V7：256 线程与 block 级 grid-stride loop

- 每线程先在寄存器累加四个位置：`base + tid`、`+256`、`+512`、`+768`，再写入 `__shared__ int local_sum[256]`。
- 共享内存完成 `256 → 128 → 64`，最后由一个 64-lane warp 用同样的 `warpReduce` 完成归约。
- 外层循环改为按 **block** 跨步：

  ```cpp
  for (int i = blockIdx.x; i < n / 1024; i += gridDim.x) {
      // 归约第 i 组，写入 sum[i]
      // ...
      __syncthreads();
  }
  ```

- 一个 block 可以依次处理 `blockIdx.x`、`blockIdx.x + gridDim.x` 等组，因此 block 数可以小于分组数。若仍设置 `gridDim.x = n / 1024`，每个 block 只循环一次，尚未利用“复用 block 处理多组”的能力。
- 循环末尾的屏障不能随意删除：其他 warp 必须等待当前组的共享值被读完，才能覆盖 `local_sum` 并处理下一组。按源码写法，每次循环共四次 block 屏障。
- V7 必须配套 **256 threads/block**；每组共享内存进一步减到 1024 字节。它调整的是每线程工作量和 block 调度方式，并不是消除了 block 同步。

##### 8. sol：一个 32-lane 子组处理一组，完全去掉共享内存

这是与 V7 不同的进一步优化方案，核心常量如下：

```cpp
kElementsPerSum = 1024;
kLanesPerGroup = 32;
kGroupsPerBlock = 8;
kThreadsPerBlock = 32 * 8;  // 256

lane  = threadIdx.x % 32;
group = blockIdx.x * 8 + threadIdx.x / 32;
```

- 每个 block 固定 256 个线程，划分成八个 32-lane 子组；**一个子组输出一个分段和，一个 block 输出八个分段和**。不再像 V7 那样让整个 block 合作输出一个结果。
- 在 64-lane warp 上，一个 warp 包含两个独立子组；在 32-lane warp 上，一个 warp 对应一个子组。这里的子组划分不等于改变硬件 `warpSize`。
- 每线程读取 32 个元素，在四条独立的无符号累加链 `s0/s1/s2/s3` 上累加，最后合并。这样不必把所有加法都串在一条依赖链上，但实际提速仍需测量。

**加载路径：对齐时用 `int4`，否则回退到标量读取。**

- 通过 `reinterpret_cast<std::uintptr_t>(arr) & 15U` 检查首地址是否 16 字节对齐。每组偏移为 `1024 * sizeof(int) = 4096` 字节，所以首地址对齐时，每组起始地址也对齐。
- 对齐路径每线程读取八个 `int4`：第 `round` 轮读取 `vectors[lane + round * 32]`，共 `8 * 4 = 32` 个 `int`。相邻 lane 读取相邻的 `int4`。
- 非 16 字节对齐时，不强制转换成 `int4*` 读取，而是按 `lane + round * 128` 以及 `+32/+64/+96` 的标量索引加载；每条加载语句中，相邻 lane 仍访问相邻元素。
- `int4` 表达了向量加载方式，不能仅据此断言一定生成某种机器指令，或带来四倍速度。

**子组归约：显式设置 `width = 32`。**

```cpp
#pragma unroll
for (int offset = 16; offset > 0; offset >>= 1)
    value += __shfl_down_sync(~0ULL, value, offset, 32);
```

- offset 依次为 `16、8、4、2、1`。显式的 `width=32` 把 64-lane warp 分成两个独立的归约范围，避免相邻两个分组混在一起；每组只有 `lane == 0` 写回结果。
- 尾块中 `group >= groups` 的线程不读取输入，累加器保持零，但**不会提前 return**：它们仍执行 shuffle，以保证完整 warp 的线程参与。只有有效分组的 lane 0 才写 `sum[group]`。
- 所有交换都在子组的寄存器之间进行，因此不需要共享内存、`__syncthreads()`、原子操作、临时分配或输出预清零。

**`#pragma unroll`：在编译时展开循环。**

- 它是一条给编译器的循环优化指令，放在要展开的循环前，只作用于紧随其后的那个循环，不是 GPU 运行时执行的函数。
- “展开”就是把多次迭代改写成多段顺序代码，减少循环计数、条件判断与跳转。在迭代次数编译时可知的情况下，不带数字的 `#pragma unroll` 请求完全展开；编译器本身也可能自动展开短循环。CUDA 对此语法的说明见 [官方文档：`#pragma unroll`](https://docs.nvidia.com/cuda/cuda-programming-guide/05-appendices/cpp-language-extensions.html#pragma-unroll)；本项目使用 MACA 的 `mxcc`，实际是否展开、生成什么指令仍以编译结果为准。

例如，源码中的子组归约循环：

```cpp
#pragma unroll
for (int offset = 16; offset > 0; offset >>= 1) {
	// ~0ULL 等价于 全位置 1(即64 bits置为1)
    value += __shfl_down_sync(~0ULL, value, offset, 32);
}
```

展开后的计算逻辑等价于：

```cpp
value += __shfl_down_sync(~0ULL, value, 16, 32);
value += __shfl_down_sync(~0ULL, value, 8, 32);
value += __shfl_down_sync(~0ULL, value, 4, 32);
value += __shfl_down_sync(~0ULL, value, 2, 32);
value += __shfl_down_sync(~0ULL, value, 1, 32);
```

- 这和 V6 手写多个 shuffle 的思路相似：源码保留易维护的循环写法，编译时有机会变成固定的指令序列。这里有五轮，因为子组宽度是 32；V6 的 64-lane 归约有六轮。
- 源码还在**对齐加载路径与标量回退路径**的 `for (int round = 0; round < 8; ++round)` 前使用了 `#pragma unroll`，请求展开八轮读取和累加。展开后 `round` 对应的地址偏移成为常量，也给编译器更多安排加载与四条独立累加链的空间。
- **循环展开不等于线程并行。** 展开后仍是每个线程自己执行这些代码，不会增加线程、改变 lane 映射或替代同步；shuffle 轮与轮之间的 `value` 依赖仍然存在，不能把这五轮看成互相独立的加法。
- 它也不保证必然提速：展开会增大代码体积，并可能增加寄存器压力、降低驻留线程数量。这里的五轮归约与八轮加载都是短的固定次数循环，适合尝试展开，但性能收益仍需测量。
- 常见控制写法是 `#pragma unroll 4`（请求以因子 4 展开，不是只执行四次循环）和 `#pragma unroll 1`（禁止展开，按 CUDA 的语法约定）。循环原本应执行的迭代数与计算结果不会因为展开因子改变。

**启动参数与约束。**

- 入口使用 `blocks = (groups + 7) / 8`、`threadsPerBlock = 256`；若 `groups <= 0`，直接返回，不启动 kernel。
- 按源码注释中的 `n = 2^24`，共有 16384 个分段和，对应 **2048 blocks**。这与 V7 的 block 级跨步复用不同：这里通过每块同时处理八组来减少块数，没有外层 grid-stride loop。
- `arr` 和 `sum` 带有 `__restrict__`，调用方必须保证两片内存不重叠。项目目录没有提供 `main.cpp`，独立分配的说明来自这份源码注释。
- 内部用 `unsigned int` 累加，采用 **模 `2^32`** 的运算，避免有符号中间加法溢出。最终通过 `static_cast<int>` 写回；若题目要求超出 `int` 范围的精确数学总和，则要同时调整设备累加、输出类型和 CPU 最终累加，不能把模运算结果当成无溢出的精确值。

##### 9. 优化主线与实验记录

```text
V1：线程独自串行求和
 → V2：线程私有数组，分阶段归约
 → V3/V4：block 协作 + 共享内存，补齐同步
 → V5：加载时先加两个元素，减少线程与共享内存
 → V6：最后一个 64-lane warp 用 shuffle
 → V7：每线程加四个元素，block 可跨步处理多组
 → sol：32-lane 子组独立求和，每块八组，无共享内存/块屏障
```

- 优化要同时检查 **输入覆盖、线程与数据映射、启动参数、同步、warp/子组宽度以及输出写入是否唯一**。不能只看加法公式是否正确。
- 以下是原笔记保留的实验记录，并非本次重新运行结果；V3 的错误结果不能用于有效性能比较。记录没有注明计时单位和完整测试条件，也没有 `kernel1.cpp` 的实测数据，因此不能直接据此断言新实现一定更快。

- 当时实验最终采用 V7 版本（当前参考 `kernel.cpp` 启用的是 V6，注意区分历史记录与源码状态）：
```
parallelv1 --- avgTime: 0.001265; minTime: 0.000817
parallelv2 --- avgTime: 0.001156; minTime: 0.000749
parallelv3(v4的降级版可能会有错误)
parallelv4 --- avgTime: 0.002337; minTime: 0.000251
parallelv5 --- avgTime: 0.000433; minTime: 0.000125
parallelv6 --- avgTime: 0.000415; minTime: 0.000122
parallelv7 --- avgTime: 0.000365; minTime: 0.000074
```


- 提交作业的时候犯了很愚蠢的错误
	- `zip program.zip program` 是单文件压缩
	- `zip -r program.zip program/` 才是文件夹压缩
	- 我还疑惑为啥总是执行失败 😂


