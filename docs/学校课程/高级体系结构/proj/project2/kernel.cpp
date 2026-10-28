#include "kernel.h"

void call_parallel_sum_kernel(int *sum, int const *arr, int n) {
    // size_t threadsPerBlock = 128;
    // size_t threadsPerBlock = 256;
    size_t threadsPerBlock = 512;
    // size_t numberOfBlocks = n / 1024 / 128;
    size_t numberOfBlocks = n / 1024;
    // size_t numberOfBlocks = 128;
    parallel_sum<<< numberOfBlocks, threadsPerBlock >>>(sum, arr, n);

}

// parallel_sum_V1 performs parallel sum without atomic.
// This code does not use any advance optimization technique on GPU,
// But still acheives many fold performance gain.
// parallel_sum_V1: 无原子的解决方案（sum变量变成一维数组）。
// 这段代码没有使用任何高级的GPU优化技术，但仍然实现了多倍的性能提升。
// __global__ void parallel_sum(int *sum, int const *arr, int n) {
//   for (int i = blockDim.x * blockIdx.x + threadIdx.x; i < n / 1024;
//        i += blockDim.x * gridDim.x) {
//     int local_sum = 0;
//     for (int j = i * 1024; j < i * 1024 + 1024; j++) {
//       local_sum += arr[j];
//     }
//     sum[i] = local_sum;
//   }
// }


// parallel_sum_V2: First read into a thread-local array, then reduce step by step.
// By using a phased reduction approach, the operations within each for loop are independent,
// thus enabling parallelization.
// parallel_sum_V2: 先读取到线程局部数组，然后分步缩减.
// 采用分阶段缩简的方法，这样每个for循环内部的操作都是独立的，从而可以实现并行化.

// __global__ void parallel_sum(int *sum, int const *arr, int n) {

//     for (int i = blockDim.x * blockIdx.x + threadIdx.x;
//          i < n / 1024;
//          i += blockDim.x * gridDim.x) {

//         int local_sum[1024];

//         // 读入 1024 个元素
//         for (int j = 0; j < 1024; j++) {
//             local_sum[j] = arr[i * 1024 + j];
//         }

//         // 1024 -> 512
//         for (int j = 0; j < 512; j++) {
//             local_sum[j] += local_sum[j + 512];
//         }

//         // 512 -> 256
//         for (int j = 0; j < 256; j++) {
//             local_sum[j] += local_sum[j + 256];
//         }

//         // 256 -> 128
//         for (int j = 0; j < 128; j++) {
//             local_sum[j] += local_sum[j + 128];
//         }

//         // 128 -> 64
//         for (int j = 0; j < 64; j++) {
//             local_sum[j] += local_sum[j + 64];
//         }

//         // 64 -> 32
//         for (int j = 0; j < 32; j++) {
//             local_sum[j] += local_sum[j + 32];
//         }

//         // 32 -> 16
//         for (int j = 0; j < 16; j++) {
//             local_sum[j] += local_sum[j + 16];
//         }

//         // 16 -> 8
//         for (int j = 0; j < 8; j++) {
//             local_sum[j] += local_sum[j + 8];
//         }

//         // 8 -> 4
//         for (int j = 0; j < 4; j++) {
//             local_sum[j] += local_sum[j + 4];
//         }

//         // 4 -> 2
//         for (int j = 0; j < 2; j++) {
//             local_sum[j] += local_sum[j + 2];
//         }

//         // 2 -> 1
//         for (int j = 0; j < 1; j++) {
//             local_sum[j] += local_sum[j + 1];
//         }

//         sum[i] = local_sum[0];
//     }
// }



// parallel_sum_V3: Uses the shared memory of the thread block.
// - Upgrades the local variable local_sum to an array shared by the thread block,
// - Replaces the loop variable j with the thread index and i with the block index(to achieve parallel computation).
// (Note: The result obtained by parallel_sum_V3 is not correct. Why is that?)
// parallel_sum_V3: 使用线程块的共享内存（shared memory）。
// - 把局部变量local_sum升级为线程块共享的数组，
// - 将循环变量j替换为线程的编号，将i替换为线程块的编号，以此来实现真正的并行计算。
// （注意：parallel_sum_V3得到的计算结果并不正确，这是为什么呢？）
// __global__ void parallel_sum(int *sum, int const *arr, int n) {

//     // 每个 block 共享 1024 个 int
//     __shared__ int local_sum[1024];

//     int j = threadIdx.x;  // thread index
//     int i = blockIdx.x;   // block index

//     // each thread reads one element
//     local_sum[j] = arr[i * 1024 + j];

//     // 1024 -> 512
//     if (j < 512) {
//         local_sum[j] += local_sum[j + 512];
//     }

//     // 512 -> 256
//     if (j < 256) {
//         local_sum[j] += local_sum[j + 256];
//     }

//     // 256 -> 128
//     if (j < 128) {
//         local_sum[j] += local_sum[j + 128];
//     }

//     // 128 -> 64
//     if (j < 64) {
//         local_sum[j] += local_sum[j + 64];
//     }

//     // 64 -> 32
//     if (j < 32) {
//         local_sum[j] += local_sum[j + 32];
//     }

//     // 32 -> 16
//     if (j < 16) {
//         local_sum[j] += local_sum[j + 16];
//     }

//     // 16 -> 8
//     if (j < 8) {
//         local_sum[j] += local_sum[j + 8];
//     }

//     // 8 -> 4
//     if (j < 4) {
//         local_sum[j] += local_sum[j + 4];
//     }

//     // 4 -> 2
//     if (j < 2) {
//         local_sum[j] += local_sum[j + 2];
//     }

//     // 2 -> 1
//     if (j == 0) {
//         sum[i] = local_sum[0] + local_sum[1];
//     }
// }


/*
// parallel_sum_V4: 
    // Add __syncthreads() in parallel_sum_V3 to make sure all threads sync up
before moving on.
    // - Ensuring that all threads reach the point where __syncthreads() is
called before proceeding further,
    // - Debug program to output correct results
    // 在parallel_sum_V3版本上，找到所有合适的位置添加上__syncthreads()，目的是：
    // - 让所有线程都运行到 __syncthreads() 所在位置以后，才能继续执行下去;
    // - 调试程序输出正确的结果
__global__ void parallel_sum(int *sum, int const *arr, int n) {

}
*/

// __global__ void parallel_sum(int *sum, const int *arr, int n)
// {
//     __shared__ int local_sum[1024];

//     int j = threadIdx.x;
//     int i = blockIdx.x;

//     local_sum[j] = arr[i * 1024 + j];

//     __syncthreads();

//     for (int stride = 512; stride > 0; stride /= 2) {
//         if (j < stride) {
//             local_sum[j] += local_sum[j + stride];
//         }

//         __syncthreads();
//     }

//     if (j == 0) {
//         sum[i] = local_sum[0];
//     }
// }


/*
    // parallel_sum_V5/V6/V7
    // Try more possible optimization points, for example:
    // - No need for __syncthreads() within a warp
    // - Avoid warp divergence
    // - Use a grid stride loop to read multiple arr elements at once
    // 尝试更多可能的优化点，例如：
    // - 线程束内不需要__syncthreads()
    // - 避免线程束产生分化
    // - 使用网格跨步循环一次读取多个 arr 元素
__global__ void parallel_sum(int *sum, int const *arr, int n) {

}
*/

// v5
// __global__ void parallel_sum(int *sum, const int *arr, int n)
// {
//     __shared__ int local_sum[512];

//     int tid = threadIdx.x;
//     int base = blockIdx.x * 1024;

//     // 一个线程直接负责两个输入
//     local_sum[tid] =
//         arr[base + tid] +
//         arr[base + tid + 512];

//     __syncthreads();

//     for (int stride = 256; stride > 0; stride >>= 1) {
//         if (tid < stride) {
//             local_sum[tid] += local_sum[tid + stride];
//         }

//         __syncthreads();
//     }

//     if (tid == 0) {
//         sum[blockIdx.x] = local_sum[0];
//     }
// }


// v6
__device__ __forceinline__ int warpReduce(int value)
{
    value += __shfl_down_sync(0xffffffffffffffffULL, value, 32);
    value += __shfl_down_sync(0xffffffffffffffffULL, value, 16);
    value += __shfl_down_sync(0xffffffffffffffffULL, value, 8);
    value += __shfl_down_sync(0xffffffffffffffffULL, value, 4);
    value += __shfl_down_sync(0xffffffffffffffffULL, value, 2);
    value += __shfl_down_sync(0xffffffffffffffffULL, value, 1);

    return value;
}

__global__ void parallel_sum(int *sum, const int *arr, int n)
{
    __shared__ int local_sum[512];

    int tid  = threadIdx.x;
    int base = blockIdx.x * 1024;

    int value =
        arr[base + tid] +
        arr[base + tid + 512];

    local_sum[tid] = value;

    __syncthreads();

    // 512 -> 256
    if (tid < 256)
        local_sum[tid] += local_sum[tid + 256];
    __syncthreads();

    // 256 -> 128
    if (tid < 128)
        local_sum[tid] += local_sum[tid + 128];
    __syncthreads();

    // 128 -> 64
    if (tid < warpSize) {
        int value = local_sum[tid] + local_sum[tid + 64];

        // 后面全部在一个 warp 内完成
        value = warpReduce(value);

        if (tid == 0)
            sum[blockIdx.x] = value;
    }
}



// v7

// __device__ __forceinline__ int warpReduce(int value)
// {
//     constexpr unsigned long long FULL_MASK = 0xffffffffffffffffULL;

//     value += __shfl_down_sync(FULL_MASK, value, 32);
//     value += __shfl_down_sync(FULL_MASK, value, 16);
//     value += __shfl_down_sync(FULL_MASK, value, 8);
//     value += __shfl_down_sync(FULL_MASK, value, 4);
//     value += __shfl_down_sync(FULL_MASK, value, 2);
//     value += __shfl_down_sync(FULL_MASK, value, 1);

//     return value;
// }


// __global__ void parallel_sum(
//     int *sum,
//     const int *arr,
//     int n)
// {
//     // V7 使用 256 threads/block
//     __shared__ int local_sum[256];

//     int tid = threadIdx.x;

//     // 一共有多少个 1024-element group
//     int groups = n / 1024;

//     /*
//      * block-level grid-stride loop
//      *
//      * block 0:
//      * group 0, group gridDim.x, ...
//      *
//      * block 1:
//      * group 1, group 1 + gridDim.x, ...
//      */
//     for (int i = blockIdx.x;
//          i < groups;
//          i += gridDim.x)
//     {
//         int base = i * 1024;

//         // ===========================
//         // 每线程处理 4 个元素
//         // 全部先在寄存器里累加
//         // ===========================

//         int value = 0;

//         value += arr[base + tid];
//         value += arr[base + tid + 256];
//         value += arr[base + tid + 512];
//         value += arr[base + tid + 768];

//         // 1024 -> 256
//         local_sum[tid] = value;

//         __syncthreads();


//         // ===========================
//         // shared memory reduction
//         // 256 -> 128 -> 64
//         // ===========================

//         if (tid < 128) {
//             local_sum[tid] += local_sum[tid + 128];
//         }

//         __syncthreads();

//         if (tid < 64) {
//             local_sum[tid] += local_sum[tid + 64];
//         }

//         __syncthreads();


//         // ===========================
//         // 最后的 64 个值
//         // 一个 C500 warp 完成
//         // ===========================

//         if (tid < 64) {

//             int warp_sum = local_sum[tid];

//             warp_sum = warpReduce(warp_sum);

//             if (tid == 0) {
//                 sum[i] = warp_sum;
//             }
//         }

//         /*
//          * 非常重要：
//          *
//          * 当前 group 的 warp reduction
//          * 完成以后，才能让其他线程开始
//          * 覆盖 local_sum，处理下一个 group。
//          */
//         __syncthreads();
//     }
// }