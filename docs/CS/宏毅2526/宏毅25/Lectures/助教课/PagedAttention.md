

- PagedAttention 最初是 vLLM 提出的完整 attention serving 方法，其中既包含 KV cache 分页管理，也包含能够访问这些 block 的 attention kernel


- 而现在 vLLM 的架构已经更加模块化，当前的 vLLM 源码架构：
	```text
	KV Cache 管理
	    ↓
	Paged KV Cache / block table
	    ↓
	Attention Backend
	    ├── FlashAttention
	    ├── FlashInfer
	    ├── Triton Attention
	    └── ...
	```

- vLLM 可以理解为把 FlashAttention 等注意力计算后端的高效计算方式扩展/适配到 Paged KV Cache 上，使 FlashAttention kernel 等能直接根据 block table 访问非连续的 KV cache。
- 尤其从源码角度看，甚至可以写成：

$$ \boxed{ \text{PagedAttention today} \approx \text{Paged KV management} + \text{paged-aware attention kernel} } $$

- 而这个 paged-aware kernel 可以是 FlashAttention backend，不再必须是 vLLM 最早那套独立的 PagedAttention CUDA kernel