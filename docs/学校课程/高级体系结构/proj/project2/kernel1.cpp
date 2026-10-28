#include "kernel.h"

#include <cstdint>

namespace {
constexpr int kElementsPerSum = 1024;
constexpr int kLanesPerGroup = 32;
constexpr int kGroupsPerBlock = 8;
constexpr int kThreadsPerBlock = kLanesPerGroup * kGroupsPerBlock;

__device__ __forceinline__ unsigned int group_sum(unsigned int value) {
    // 显式 width=32：64-lane warp 分成两个独立的归约组。
    // 所有线程（包括尾块无效分组）都参加，每组只使用 lane 0 的结果。
#pragma unroll
    for (int offset = kLanesPerGroup / 2; offset > 0; offset >>= 1) {
        value += __shfl_down_sync(~0ULL, value, offset, kLanesPerGroup);
    }
    return value;
}
}  // namespace

// 固定以 256 threads/block 启动，适用于 32/64-lane warp。
// arr 和 sum 必须不重叠，main.cpp 中的独立分配满足此要求。
__global__ void parallel_sum(int *__restrict__ sum,
                             const int *__restrict__ arr, int n) {
    const unsigned int lane = threadIdx.x % kLanesPerGroup;
    const unsigned int group =
        blockIdx.x * kGroupsPerBlock + threadIdx.x / kLanesPerGroup;
    const unsigned int groups = n > 0 ? n / kElementsPerSum : 0;
    unsigned int s0 = 0, s1 = 0, s2 = 0, s3 = 0;

    if (group < groups) {
        const int *input = arr + group * kElementsPerSum;
        if ((reinterpret_cast<std::uintptr_t>(arr) & 15U) == 0) {
            // 每线程读取 8 个 int4，共 32 个元素。
            // 相邻 lane 读取相邻 int4；1024 元素段保持 16 字节对齐。
            const int4 *vectors = reinterpret_cast<const int4 *>(input);
#pragma unroll
            for (int round = 0; round < 8; ++round) {
                const int4 v = vectors[lane + round * kLanesPerGroup];
                // 四条独立累加链，避免串行依赖；采用模 2^32 累加。
                s0 += static_cast<unsigned int>(v.x);
                s1 += static_cast<unsigned int>(v.y);
                s2 += static_cast<unsigned int>(v.z);
                s3 += static_cast<unsigned int>(v.w);
            }
        } else {
            // 普通 int* 未对齐时仍可调用；每轮保持合并访存。
#pragma unroll
            for (int round = 0; round < 8; ++round) {
                const unsigned int i = lane + round * 4 * kLanesPerGroup;
                s0 += static_cast<unsigned int>(input[i]);
                s1 += static_cast<unsigned int>(input[i + kLanesPerGroup]);
                s2 += static_cast<unsigned int>(input[i + 2 * kLanesPerGroup]);
                s3 += static_cast<unsigned int>(input[i + 3 * kLanesPerGroup]);
            }
        }
    }

    // 不提前退出无效分组，保证完整 warp 的全部线程执行 shuffle。
    const unsigned int total = group_sum((s0 + s1) + (s2 + s3));
    if (lane == 0 && group < groups) {
        sum[group] = static_cast<int>(total);
    }
}

void call_parallel_sum_kernel(int *sum, const int *arr, int n) {
    const int groups = n / kElementsPerSum;
    if (groups <= 0) {
        return;
    }

    const int blocks = (groups + kGroupsPerBlock - 1) / kGroupsPerBlock;
    // main.cpp 的 n=2^24 对应 2048 blocks，每块输出 8 个分段和。
    // 无共享内存、块级同步、临时分配、预清零或原子操作。
    parallel_sum<<<blocks, kThreadsPerBlock>>>(sum, arr, n);
}
