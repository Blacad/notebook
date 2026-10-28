- KV Cache：用空间换时间
	- [原理：Slides 29–34](../../Slides/Unit2/inference.pdf#page=29)
	- 自回归模型使用 **Causal Attention**：旧 token 看不到未来，因此新增 token 时，各层旧 token 的 K、V 不会改变，可以直接复用。
	- **Prefill**：并行处理整个 prompt，建立各层的 KV Cache；**Decode**：每步只处理新 token，计算它的 Q、K、V，将新 K、V 追加到缓存。
	- 当前 query 仍需与全部可见的 K、V 做 attention；只是不用重新计算旧 token。旧 Q 后续不会再用到，所以不缓存。
	- 第 $t$ 个位置的 attention：$o_t=\operatorname{softmax}(q_tK_{1:t}^{\top}/\sqrt{d_k})V_{1:t}$。
	- ![新 query 读取历史 KV 与当前 KV（PDF 第31页）](lecture6-kvcache-decode.png)

- 代价：缓存占显存，读取缓存也有开销
	- [显存与数据搬运：Slides 34–35](../../Slides/Unit2/inference.pdf#page=34)
	- 缓存通常放在 HBM，每步 attention 仍需搬运历史 KV；上下文越长、并发越多，显存占用和带宽压力越大。
	- 普通 KV Cache 的大小约为 $2BLTH_{kv}ds$ bytes：$B$ 为 batch size，$L$ 为层数，$T$ 为缓存长度，$H_{kv}$ 为 KV heads 数，$d$ 为每个 head 的维度，$s$ 为每元素字节数（FP16 为 2）；前面的 2 表示 K 和 V。
	- **要用 KV heads 数，而不是 query heads 数**；可用显存还要扣除模型权重等，不能全用来放缓存。

- 减少 KV heads：MHA → MQA / GQA
	- [共享 KV：Slides 36–37](../../Slides/Unit2/inference.pdf#page=36)
	- **MHA（Multi-Head Attention）**：每个 query head 各有一套 K、V。
	- **MQA（Multi-Query Attention）**：所有 query heads 共用一套 K、V，缓存最省，但可能影响模型能力。
	- **GQA（Grouped-Query Attention）**：query heads 分组，每组共享一套 K、V，在缓存大小与模型能力之间折中；缓存量约为 MHA 的 $H_{kv}/H_q$。
	- 这属于模型结构改变，需要配套训练，不是推理时直接打开缓存就能得到的效果。
	- ![MQA 与 GQA 的 KV 共享方式（PDF 第37页）](lecture6-kvcache-mqa-gqa.png)

- 压缩 KV：MLA（Multi-head Latent Attention）
	- [原理与推导：Slides 38–43](../../Slides/Unit2/inference.pdf#page=38)
	- DeepSeek 使用的思路：每个 token 缓存低维 latent $c$，而不是完整的各头 K、V；简化表示为 $k=W_kc$、$v=W_vc$。
	- 相对于K：不必每步先恢复全部历史 KV：$q^Tk=(W_k^Tq)^Tc$，可把变换移到 query 上，即把 q 压缩就好了
	- 相对于V：$\sum_i\alpha_iW_vc_i=W_v\sum_i\alpha_ic_i$，可先加权汇总 latent 再投影。
	- 核心是**减少缓存和搬运量**；同样需要改变模型结构并训练（以上省略位置编码等实现细节）。

- 减少保留的 token
	- [Slides 44–48](../../Slides/Unit2/inference.pdf#page=44)
	- **Sliding Window Attention**：只看最近一个窗口，窗口外的 KV 可丢弃；多层叠加可间接传递更早的信息。若模型混用局部层和全局层，全局层仍需保留完整历史。
	- **StreamingLLM**：保留开头少量 attention sink tokens + 最近窗口，比单纯丢掉所有早期 token 更稳定；不等于仍能读取全部远处历史。
	- **Pruning KV Cache**：如 Scissorhands、H2O，利用“只有少量 token 反复获得较大 attention”的现象，保留重要 KV，丢弃不重要 KV；通常不需重训，但可能损失效果。

- 跨对话复用：Prefix Cache / Prompt Cache
	- [Slides 49–54](../../Slides/Unit2/inference.pdf#page=49)
	- 在模型和计算设置一致时，不同请求可复用**完全相同前缀**的 KV，减少重复 prefill；不是缓存最终回答。
	- 一旦前缀出现差异，后面即使又出现相同 token，也不能直接复用：它们依赖的上下文已经不同。
	- Prompt 编排：**稳定内容放前面，变化内容放后面**，如固定 system prompt、工具说明在前，用户问题和变量在后，可提高命中率。
	- ![共享前缀可以复用，分叉后的相同字也不能直接复用（PDF 第49页）](lecture6-kvcache-prefix.png)

- 和 [[Lecture5 FlashAttention]] 的区别
	- **FlashAttention**：减少 attention 计算过程中的 HBM ↔ SRAM 搬运；**KV Cache**：避免生成时反复计算历史 token。两者可以配合使用。
	- [总结：Slide 55](../../Slides/Unit2/inference.pdf#page=55)：普通 KV Cache 不改变原有 attention、不需重训，代价是显存；共享/压缩 KV 改结构，窗口/剪枝则减少可见历史，需要权衡模型效果。
