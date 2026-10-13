---
comments: "true"
---
- [来源](https://docs.nvidia.com/cuda/cuda-programming-guide/02-basics/intro-to-cuda-cpp.html) ✅基本记完一遍来源的基础知识

## Kernels

- Kernel --- 可以在主机上调用的在GPU上执行的函数被称为内核。内核被编写为可以由许多并行线程同时运行

- Specifying kernels 指定内核
	- 内核的代码使用 `__global__` 声明符指定。这表示编译器将此函数编译为适用于GPU，以便可以从内核启动中调用它。
	- 内核启动是一个启动内核运行的操作，通常从CPU开始。内核是具有void返回类型的函数
	
	```c++
		// Kernel definition
	__global__ void vecAdd(float* A, float* B, float* C){...}
	```


- Launching Kernels 启动内核
	- 并行执行内核的线程数在内核启动时指定，这称为执行配置。
	- 相同的内核的不同调用可能使用不同的执行配置，例如不同的线程数或线程块。
	- 有两种从CPU代码启动内核的方法，即三箭头符号和cudaLaunchKernelEx
	- 三箭头符号Triple Chevron Notation是启动内核最常见的方式
		- 它使用三个箭头字符来封装内核启动的执行配置，即 `<<< >>>`。执行配置参数以逗号分隔的列表形式在箭头内指定，类似于函数调用中的参数
		- 例如 ==`vecAdd<<<1, 256>>>(A, B, C);`==
			- 三元箭头符号的前两个参数分别是网格维度(Grid)和线程块维度(Block)。
			- 当使用一维线程块或网格时，可以使用整数来指定维度。
			- 上述代码启动一个包含256个线程的单个线程块。
			- 每个线程将执行完全相同的内核代码。
			- 在线程和网格索引内联函数中，我们将展示每个线程如何使用其在线程块和网格中的索引来改变其操作的数据。
		- 每个块中线程的数量有限，因为一个块中的所有线程都位于同一个流式多处理器（SM）上，并且必须共享SM的资源。
		- 在当前的GPU上，一个线程块可能包含多达1024个线程。如果资源允许，可以同时在SM上调度多个线程块。内核启动相对于主机线程是异步的。也就是说，内核将在GPU上设置执行，但主机代码在继续之前不会等待内核在GPU上完成（甚至开始）执行。必须使用某种形式的GPU和CPU之间的同步来确定内核是否完成。最基本的方法是完全同步整个GPU
		- 使用2或3维网格或线程块时，CUDA类型dim3用作网格和线程块维度参数。下面的代码片段展示了使用16x16线程块网格的MatAdd内核的内核启动，每个线程块是8x8
				```c++
				 dim3 grid(16,16); 
				 dim3 block(8,8);
				 MatAdd<<<grid, block>>>(A,B,C);
				```


- thread和grid索引内建函数
	- 在内核代码中，CUDA 提供了内建函数来访问执行配置的参数以及线程或块的索引。
		- `threadIdx`表示线程块中线程的索引。线程块中的每个线程将具有不同的索引。
		- `blockDim` 返回线程块的维度，该维度在内核启动的执行配置中指定。
		- `blockIdx` 表示线程块在网格中的索引。每个线程块将有一个不同的索引。
		- `gridDim` 提供了网格的维度，该维度是在内核启动时在执行配置中指定的。
	- 每个这些内建函数都是一个具有.x、.y和.z成员的3分量向量。未在启动配置中指定的维度将默认为1。`threadIdx`和`blockIdx`是从0开始计数的。也就是说，`threadIdx.x`将取从0到`blockDim.x-1`的值。.y和.z在其各自的维度中操作方式相同。
	- 这些允许一个线程识别它应该执行什么工作。回到vecAdd内核，内核有三个参数，每个都是一个浮点向量。内核执行A和B的逐元素相加，并将结果存储在C中。内核被并行化，使得每个线程执行一次加法。它计算哪个元素由其线程和网格索引决定。
			```c++
			__global__ void vecAdd(float* A, float* B, float* C)
				{
				   // calculate which element this thread is responsible for computing
				   int workIndex = threadIdx.x + blockDim.x * blockIdx.x;
				
				   // Perform computation
				   C[workIndex] = A[workIndex] + B[workIndex];
				}
				
				int main()
				{
				    ...
				    // A, B, and C are vectors of 1024 elements
				    vecAdd<<<4, 256>>>(A, B, C);
				    ...
				}	
			```

- Bounds Checking 
	- 以上示例假设向量的长度是线程块大小的倍数，本例中为256个线程。为了使内核处理任何向量长度，我们可以添加检查以确保内存访问不超过数组的边界，如下所示，然后启动一个线程块，其中将有一些空闲线程。
	```c++
			__global__ void vecAdd(float* A, float* B, float* C)
				{
				   // calculate which element this thread is responsible for computing
				   int workIndex = threadIdx.x + blockDim.x * blockIdx.x;
					
				   // Perform computation
				    if(workIndex < vectorLength)
					     {
					         // Perform computation
					         C[workIndex] = A[workIndex] + B[workIndex];
					     }
				}
				
	```

	- 使用上述内核代码，可以启动比所需更多的线程，而不会导致数组越界访问。当workIndex超过vectorLength时，线程退出且不执行任何工作。在块中启动不执行任何工作的额外线程不会产生很大的开销，然而应该避免启动没有任何线程执行工作的线程块。现在这个内核可以处理不是块大小的倍数的向量长度。
	- 所需线程块的数量可以通过将所需线程数（在这种情况下为向量长度）除以每个块中的线程数来计算，即所需线程数除以每个块中的线程数的整数除法，并向上取整。以下是一个将此表示为单个整数除法的常见方法。在整数除法之前添加线程 - 1，这就像一个向上取整函数，只有当向量长度不能被每个块中的线程数整除时，才会添加另一个线程块。
		- `int blocks = cuda::ceil_div(vectorLength, threads);`
		- `int blocks = (vectorLength + threads - 1) / threads;`
		- 上述二者等效


## Memory

- 为了使用上面显示的vecAdd内核，数组A、B和C必须存储在GPU可访问的内存中。有几种不同的方法可以实现这一点，这里将介绍其中两种。

- Unified Memory 统一内存
	- 统一内存是CUDA运行时的一项功能，它允许NVIDIA驱动程序管理数据在主机和设备之间的移动。
	- 内存通过使用cudaMallocManaged API或通过声明带有`__managed__`指定符的变量来分配。
	- NVIDIA驱动程序将确保当GPU或CPU尝试访问时，内存都是可访问的。

	```c++
			void unifiedMemExample(int vectorLength)
		{
		    // Pointers to memory vectors
		    float* A = nullptr;
		    float* B = nullptr;
		    float* C = nullptr;
		    float* comparisonResult = (float*)malloc(vectorLength*sizeof(float));
		
		    // Use unified memory to allocate buffers
		    cudaMallocManaged(&A, vectorLength*sizeof(float));
		    cudaMallocManaged(&B, vectorLength*sizeof(float));
		    cudaMallocManaged(&C, vectorLength*sizeof(float));
		
		    // Initialize vectors on the host
		    initArray(A, vectorLength);
		    initArray(B, vectorLength);
		
		    // Launch the kernel. Unified memory will make sure A, B, and C are
		    // accessible to the GPU
		    int threads = 256;
		    int blocks = cuda::ceil_div(vectorLength, threads);
		    vecAdd<<<blocks, threads>>>(A, B, C, vectorLength);
		    // Wait for the kernel to complete execution
		    cudaDeviceSynchronize();
		
		    // Perform computation serially on CPU for comparison
		    serialVecAdd(A, B, comparisonResult, vectorLength);
		
		    // Confirm that CPU and GPU got the same answer
		    if(vectorApproximatelyEqual(C, comparisonResult, vectorLength))
		    {
		        printf("Unified Memory: CPU and GPU answers match\n");
		    }
		    else
		    {
		        printf("Unified Memory: Error - CPU and GPU answers do not match\n");
		    }
		
		    // Clean Up
		    cudaFree(A);
		    cudaFree(B);
		    cudaFree(C);
		    free(comparisonResult);
		
		}
	```

- Explicit Memory Management 显式内存管理
	- 显式管理内存分配和数据迁移可以在一定程度上提高应用程序性能，但这会使代码更加冗长。下面的代码使用cudaMalloc在GPU上显式分配内存。GPU上的内存使用与之前示例中统一内存相同的cudaFree API释放。

	```c++
		void explicitMemExample(int vectorLength)
		{
		    // Pointers for host memory
		    float* A = nullptr;
		    float* B = nullptr;
		    float* C = nullptr;
		    float* comparisonResult = (float*)malloc(vectorLength*sizeof(float));
		    
		    // Pointers for device memory
		    float* devA = nullptr;
		    float* devB = nullptr;
		    float* devC = nullptr;
		
		    //Allocate Host Memory using cudaMallocHost API. This is best practice
		    // when buffers will be used for copies between CPU and GPU memory
		    cudaMallocHost(&A, vectorLength*sizeof(float));
		    cudaMallocHost(&B, vectorLength*sizeof(float));
		    cudaMallocHost(&C, vectorLength*sizeof(float));
		
		    // Initialize vectors on the host
		    initArray(A, vectorLength);
		    initArray(B, vectorLength);
		
		    // start-allocate-and-copy
		    // Allocate memory on the GPU
		    cudaMalloc(&devA, vectorLength*sizeof(float));
		    cudaMalloc(&devB, vectorLength*sizeof(float));
		    cudaMalloc(&devC, vectorLength*sizeof(float));
		
		    // Copy data to the GPU
		    cudaMemcpy(devA, A, vectorLength*sizeof(float), cudaMemcpyDefault);
		    cudaMemcpy(devB, B, vectorLength*sizeof(float), cudaMemcpyDefault);
		    cudaMemset(devC, 0, vectorLength*sizeof(float));
		    // end-allocate-and-copy
		
		    // Launch the kernel
		    int threads = 256;
		    int blocks = cuda::ceil_div(vectorLength, threads);
		    vecAdd<<<blocks, threads>>>(devA, devB, devC, vectorLength);
		    // wait for kernel execution to complete
		    cudaDeviceSynchronize();
		
		    // Copy results back to host
		    cudaMemcpy(C, devC, vectorLength*sizeof(float), cudaMemcpyDefault);
		
		    // Perform computation serially on CPU for comparison
		    serialVecAdd(A, B, comparisonResult, vectorLength);
		
		    // Confirm that CPU and GPU got the same answer
		    if(vectorApproximatelyEqual(C, comparisonResult, vectorLength))
		    {
		        printf("Explicit Memory: CPU and GPU answers match\n");
		    }
		    else
		    {
		        printf("Explicit Memory: Error - CPU and GPU answers to not match\n");
		    }
		
		    // clean up
		    cudaFree(devA);
		    cudaFree(devB);
		    cudaFree(devC);
		    cudaFreeHost(A);
		    cudaFreeHost(B);
		    cudaFreeHost(C);
		    free(comparisonResult);
		}
	```
	- CUDA API中的cudaMemcpy用于将位于CPU上的缓冲区中的数据复制到位于GPU上的缓冲区。除了目标指针、源指针和字节数大小之外，cudaMemcpy的最后一个参数是cudaMemcpyKind_t
		- `cudaMemcpyHostToDevice` 用于从CPU到GPU的复制
		- `cudaMemcpyDeviceToHost` 从GPU到CPU的复制
		- `cudaMemcpyDeviceToDevice` 用于GPU内部或GPU之间的复制
		- `cudaMemcpyDefault` 被作为 cudaMemcpy 的最后一个参数传递。这导致 CUDA 使用源和目标指针的值来确定要执行复制的类型。
	- ==cudaMemcpy API是同步的。也就是说，它不会在复制完成之前返回。异步复制在CUDA Streams中启动内存传输时引入==


	- 代码使用cudaMallocHost在CPU上分配内存。这为主机分配了页锁定内存(pinned page)，可以提高复制性能，并且对于异步内存传输是必要的。一般来说，对于将用于GPU之间数据传输的CPU缓冲区，使用页锁定内存是良好的实践。如果过多的主机内存被页锁定，某些系统上的性能可能会下降。最佳实践是仅对用于从GPU发送或接收数据的缓冲区进行页锁定。



## 同步CPU和GPU

- 内核启动相对于调用它们的CPU线程是异步的。这意味着CPU线程的控制流将在内核完成之前继续执行，甚至可能在内核启动之前。为了保证在主机代码中继续执行之前内核已经完成执行，需要某种同步机制。

- 同步GPU和主机线程最简单的方法是使用`cudaDeviceSynchronize`，它会阻塞主机线程，直到GPU上之前发出的所有工作完成。

- 最基础的块级同步机制是 `__syncthreads()` 内置函数，它作为一个栅栏，所有块中的线程必须等待，然后才允许任何线程继续执行。
- 为了高效协作，预期共享内存将是一个低延迟的内存，靠近每个处理器核心（就像一级缓存一样），而 `__syncthreads()` 预期将是轻量级的。`__syncthreads()` 只同步单个线程块内的线程。


## 运行时初始化

- CUDA运行时为系统中的每个设备创建一个CUDA上下文。此上下文是该设备的主要上下文，并在第一个需要在此设备上激活上下文的运行时函数中初始化。上下文在应用程序的所有主机线程之间共享。作为上下文创建的一部分，如果需要，设备代码将即时编译并加载到设备内存中。这一切都是透明的。CUDA运行时创建的主要上下文可以通过驱动程序API访问

- CUDA 12.0之后，`cudaInitDevice`和`cudaSetDevice`调用初始化与指定设备关联的运行时和主上下文。如果这些调用之前发生运行时API请求，运行时将隐式使用设备0并按需自我初始化以处理这些请求。这在计时运行时函数调用和解释第一次调用运行时的错误代码时非常重要

- `cudaDeviceReset`销毁当前设备的默认上下文。如果在默认上下文被销毁后调用CUDA运行时API，将为该设备创建一个新的默认上下文


## Error Checking

- 每个CUDA API都返回一个枚举类型cudaError_t的值。在生产应用程序中，始终检查并管理每个CUDA API调用的返回值是最佳实践。如果没有错误，返回的值是cudaSuccess。许多应用程序选择实现一个如以下所示的实用宏。

```c++
	#define CUDA_CHECK(expr_to_check) do {            \
	    cudaError_t result  = expr_to_check;          \
	    if(result != cudaSuccess)                     \
	    {                                             \
	        fprintf(stderr,                           \
	                "CUDA Runtime Error: %s:%i:%d = %s\n", \
	                __FILE__,                         \
	                __LINE__,                         \
	                result,\
	                cudaGetErrorString(result));      \
	    }                                             \
	} while(0)
```

- 此宏使用`cudaGetErrorString API`，该API返回一个描述特定`cudaError_t`值的可读字符串。使用上述宏，应用程序会在`CUDA_CHECK(expression)`宏内调用CUDA运行时API调用，如下所示：

```c++
    CUDA_CHECK(cudaMalloc(&devA, vectorLength*sizeof(float)));
    CUDA_CHECK(cudaMalloc(&devB, vectorLength*sizeof(float)));
    CUDA_CHECK(cudaMalloc(&devC, vectorLength*sizeof(float)));
```


### Error State

- CUDA运行时为每个主机线程维护一个`cudaError_t`状态。该值默认为cudaSuccess，每当发生错误时都会被覆盖。`cudaGetLastError`返回当前错误状态并将其重置为cudaSuccess。或者，`cudaPeekAtLastError`返回错误状态而不重置它。

- 内核使用三重箭头符号启动时不会返回cudaError_t。在内核启动后立即检查错误状态是一种良好的做法，以便检测内核启动或内核启动前的异步错误。在内核启动后立即检查错误状态时，如果返回值为cudaSuccess，并不意味着内核已成功执行或甚至已开始执行。这仅验证传递给运行时的内核启动参数和执行配置没有触发任何错误，并且不是内核启动前的错误状态或异步错误。


### CUDA_LOG_FILE

- 另一种识别CUDA错误的好方法是使用CUDA_LOG_FILE环境变量。当设置此环境变量时，CUDA驱动程序会将遇到的错误消息写入到环境变量中指定的路径的文件中

- `env CUDA_LOG_FILE=cudaLog.txt ./errlog`


## Device and Host Functions

- `__global__` 指定符用于表示内核的入口点。也就是说，一个将在GPU上并行执行的功能。通常，内核是从主机启动的，然而，也可以使用动态并行性在另一个内核内部启动内核。

- `__device__` 指示一个函数应该为 GPU 编译，并且可以从其他 `__device__` 或 `__global__` 函数中调用。一个函数，包括类成员函数、仿函数和 lambda 表达式，可以同时指定为 `__device__` 和 `__host__`


## Variable Specifiers

- 多种变量指定符
	- `__device__` specifies that a variable is stored in [Global Memory](https://docs.nvidia.com/cuda/cuda-programming-guide/02-basics/writing-cuda-kernels.html#writing-cuda-kernels-global-memory)
	- `__constant__` specifies that a variable is stored in [Constant Memory](https://docs.nvidia.com/cuda/cuda-programming-guide/02-basics/writing-cuda-kernels.html#writing-cuda-kernels-constant-memory)
	- `__managed__` specifies that a variable is stored as [Unified Memory](https://docs.nvidia.com/cuda/cuda-programming-guide/02-basics/understanding-memory.html#memory-unified-memory)
	- `__shared__` specifies that a variable is store in [Shared Memory](https://docs.nvidia.com/cuda/cuda-programming-guide/02-basics/writing-cuda-kernels.html#writing-cuda-kernels-shared-memory)
- 当一个变量在 `__device__` 或 `__global__` 函数内没有指定修饰符时，尽可能分配到寄存器中，必要时分配到局部内存中。在` __device__` 或 `__global__` 函数外没有指定修饰符声明的任何变量都将分配在系统内存中。

- 当一个函数使用 `__host__`  `__device__` 指定时，编译器被指示为该函数生成GPU和CPU代码。在这样的函数中，可能希望使用预处理器来指定仅针对GPU或CPU副本的代码


