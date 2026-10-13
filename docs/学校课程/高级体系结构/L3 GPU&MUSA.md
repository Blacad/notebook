>[!notice] 本节重点
>- 摩尔线程及其MUSA编程原理，重点说明 国产自研GPU生态和NVIDIA生态的兼容与差异

## 编程层兼容

- MUSA 与 CUDA 在编程接口上是近乎一致的，当然 沐曦 MACA 自研的一套也是和 CUDA 在编程接口上是近乎一致的

| NVIDIA CUDA   | 摩尔线程 MUSA                 | 沐曦 MXMACA                 |
| ------------- | ------------------------- | ------------------------- |
| CUDA C/C++    | MUSA C/C++                | MXMACA Native / CUDA 兼容接口 |
| `nvcc`        | `mcc` / `mtcc`            | `mxcc`                    |
| CUDA Runtime  | MUSA Runtime              | MXMACA Runtime            |
| cuBLAS        | muBLAS                    | MACA 对应计算库                |
| cuDNN         | muDNN                     | MACA DNN/AI 库             |
| NCCL          | MCCL                      | MACA 通信栈                  |
| CUDA → GPU 指令 | MUSA → MTGPU ISA          | MACA → 沐曦 GPU 指令          |
| CUDA 兼容本身     | `MUSA Mapping` / `musify` | `cu-bridge` / `cucc`      |


- MUSA兼容
	```text
	CUDA source
	    │
	    ▼
	MUSA Mapping / musify
	    │
	cudaMalloc → musaMalloc
	cuBLAS     → muBLAS
	...
	    │
	    ▼
	   mcc
	    │
	    ▼
	 MT GPU
	```


- cu-bridge 介绍与MACA兼容

	```text
	                    CUDA 应用
	                       │
	         ┌─────────────┼─────────────┐
	         ▼             ▼             ▼
	     CUDA API       CUDA语法       构建系统
	         │             │             │
	     wrappers      compiler适配    nvcc/CUDA_PATH
	         │             │             │
	         └─────────────┼─────────────┘
	                       ▼
	                    cu-bridge
	                       │
	                       ▼
	                  MXMACA SDK
	                       │
	             ┌─────────┴─────────┐
	             ▼                   ▼
	        MACA Runtime        MACA Compiler
	             │                   │
	             └─────────┬─────────┘
	                       ▼
	                    沐曦 GPU
	```



## 差异与硬件优化


```text
                    用户程序
                       │
              CUDA-style API
                       │
       ┌───────────────┼───────────────┐
       ▼               ▼               ▼
     CUDA             MUSA           MXMACA
       │               │               │
      nvcc            mcc             mxcc
       │               │               │
       ▼               ▼               ▼
 Compiler Backend  Compiler Backend Compiler Backend
       │               │               │
       ▼               ▼               ▼
   NVIDIA ISA       MTGPU ISA       MetaX ISA
       │               │               │
 ┌─────┴─────┐    ┌────┴─────┐    ┌────┴─────┐
 │Matrix Eng.│    │   TCE    │    │Matrix Eng.│
 │TMA/LSU    │    │   TME    │    │Memory Eng.│
 │SMEM/cache │    │SMEM/cache│    │SMEM/cache │
 └─────┬─────┘    └────┬─────┘    └────┬─────┘
       ▼               ▼               ▼
   NVIDIA GPU       MT GPU          MetaX GPU
```


- 大致可以理解为从编译器开始分叉，真实的差距体现在 关键的算子库(是否充分利用硬件特性将性能吃满)、GPU 微架构的设计优化 以及 多卡系统的通信优化

| 层次           | 真正困难/有价值的地方                          |
| ------------ | ------------------------------------ |
| 编译器          | 如何把同一个 kernel 映射成自家 GPU 上高效的机器码      |
| Kernel / 算子库 | GEMM、Attention、MoE 等是否能把硬件吃满         |
| GPU 微架构      | 矩阵计算单元、访存单元、cache/shared memory、调度机制 |
| 多卡系统         | GPU-GPU 互联、AllReduce、All2All、通信计算重叠  |

- 比如像 FlashAttention 就是 关键算子库的一部分，它必须要充分利用硬件特性进行优化