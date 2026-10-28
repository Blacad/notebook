
- 原论文 [FlashAttention](https://arxiv.org/abs/2205.14135)

- 最 Naive 计算 Attention 的过程
	- [Naive Attention10-17](../../Slides/Unit2/inference.pdf#page=10)
	- 计算softmax时，需要先找 最大 attention score，再算 和，再算最终的 softmax，这样每位置 3 次读写
	- 需要从 HBM 向 SRAM 搬运非常多次数据，这成为瓶颈

- Improve-1 同时找出最大的 attention score 和 计算用于 softmax 的 attention score(需要减attention score max以使得避免结果溢出)
	- [直接算出softmax分母18-21](../../Slides/Unit2/inference.pdf#page=18)
	- 其实很简单，当找到的最大值需要更改时，原先是 $e^{a-d_1}$， $d_2$ 是更大值时只需要，$e^{a-d_1} \cdot e^{d_1-d_2}$ 就好了，同时前面的值已经加和为单个量 $s_p$，因此直接更新之前的加和结果 $s_p \cdot e^{d_1-d_2}$ 就更新好之前的值了，所以 SRAM 只需要常驻 当前的最大值 $d$ 和 目前的加和结果 $s$ 就好了
	- 再算最终的 softmax 时，由于有最终 最大值 $d$  和 加和结果 $s$ ，所以直接所有 attention score 可直接计算出最终值
	- 经过上述操作，将每位置 3次读写 压缩为 每位置 2次读写

- FlashAttention 一步到位不需要 softmax后的 attention score
	- [FlashAttention25-27](../../Slides/Unit2/inference.pdf#page=25)
	- 更近一步，找到最大值 $d$ 时，直接计算 $s=\sum e^{a-d}$，并计算 $\frac{e^{a-d}}{s}$，这样直接得到 $o=\frac{e^{a-d}}{s} * v$，找到新的最大值$d_2$时，$d'=d_2$，$s'=s \cdot e^{d-d_2}$ , $o'=o*\frac{s}{s'}*e^{d-d_2}$

- 在实作的代码中，如果没有特别设置，应该是默认使用 flashattention，如果不想使用 flashattention 就需要设置 `"atten_implementation": "eager"`




