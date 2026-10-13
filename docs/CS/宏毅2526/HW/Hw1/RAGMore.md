
- 本次作业中我们只有一篇文档 --- 《红楼梦》，但是实际生产应用过程中，我们可能会接触到成千上万个文档文件，如何提高搜索效率成为了一个问题，这有点类似于之前的搜广推，我们需要先进行初筛，然后精筛，在本次作业中其实只涉及 精筛 的部分知识


- 完整的流程比如是 先找Top-20 Documents，再进行细分，而找 Top-20 Documents 往往不会将完整的Document 变成一个embedding，而是同样做 chunking，后续再聚合chunks的分数到对应的Document，然后得到 Top-20 Documents，再然后进行精细的划分


- 具体来讲，就以 Elastic Search 为例，它会先将所有文档进行 chunking 并且 embedding，构成完整的 Embedding DB，同时需要保留原信息

- Elastic Search 会 对这些 Embedding 建立图结构，这里以 HNSW(Hierarchical Navigable Small World) 为例，这会是分层设计的图结构
	- 有点类似于 Skip List 的多层结构，不过它每层都是图结构
	- 上层的节点会很少，负责大范围跨越
	- 下层节点比较密集，负责局部精细搜索
	- 比如 新的 embedding 进来了，会用一个概率分布将其随机到某个 level，比如被随机到了 level 1，它就会存在于 level 1 和 level 0(上层节点一定会在下层出现)
	- 接着会将 embedding 与 最上层从上往下开始比较，由图进行导航，具体而言
		- 先从最顶层的入口节点开始比较，比较当前节点以及当前节点的所有邻居节点，去往更近的节点，直到没有邻居节点比当前节点更近为止，往下层走
		- 下层直接从上一层定位到的节点开始比较(上层的节点在下层一定存在)循环往复直到抵达自己被分配到的 level
		- 到了 需要插入的level时，会进行更加宽泛的比较，比如被分配到 level 1，那么从 最高层 到 level 2 就是一路greedy下来，而在 level 1时会以上层定位节点为始，维持一个候选节点集，而不只是找最近邻的节点，然后从这些候选节点中选择一些建立边，然后往下层走
		- 再下层时会在上一层的候选区域里搜索 + 建边
			- 这里所谓的在候选区域选，就是每次取当前最接近 x 的节点 -> 展开它的 graph neighbors -> 把好的邻居插回候选池 并 淘汰太差的候选
		- 如此直到 level 0 完结
	- 参数 $M$  控制连接邻居数量(连接大小)
		- 连接时并不是机器的选择距离最近的M个，而是多样性选择M个(考虑图的导航能力)
	- 参数 $efConstruction$ 控制插入时在当前层搜索多宽(候选集大小)

- 总而言之，基于上述 Elastic Search 我们就能找到非常多的有关chunks，后续你可以选择直接使用这些Chunks，也可以选择先聚合回Document，再对这些 Documents 进行更加精细的处理(之所有回到Document，是因为Document往往能够提供更加完整的上下文信息，而Chunk可能会丢失这部分信息)

- Elastic Search 能够支持多种 retrieval 模式，比如 BM25 等，因此 它在存储时往往保留 原文 + 倒排索引(内部分词) + Embedding 图结构 等等，方便后续的 Retrieval，是个挺好用的工具
	- 可以利用 Docker 本地部署一个 Elastic Search 的服务
	- 然后就可以用 python 的库 `from elasticsearch import Elasticsearch` 来使用它的诸多功能，非常好用
	- 但是真实的大系统设计往往是 `PostgreSQL(outBox Event) -> Debezium CDC -> Kafka -> Elasticsearch`
		- PostgreSQL：保存权威业务数据
		- Outbox Event：在同一事务里记录“应该对外发布的业务事件”(这是个表的设计理念，相当于把数据更改与业务说明放在同一个事务里，使得数据与业务对齐)
		- Debezium CDC：从 PostgreSQL WAL(Write Ahead Log) 中捕获 Outbox 表的新事件
		- Kafka：持久化、缓冲并分发这些事件(消费者与生产者之间的buffer)
		- Elasticsearch Consumer：消费 Kafka 事件并更新搜索索引
		- Elasticsearch：保存面向搜索优化的数据副本，用于 BM25、向量检索等


- 对于大规模的搜索场景，不论是 搜广推，还是现在的 RAG 整个大的思路是
	- Recall -> pre-ranking -> re-ranking

| 阶段   | 搜索               | 推荐              | RAG                 |
| ---- | ---------------- | --------------- | ------------------- |
| 海量对象 | 网页/商品            | Item            | Chunks              |
| 快速召回 | 倒排/BM25/ANN      | ANN/CF/多路召回     | BM25/HNSW           |
| 粗排   | 轻量模型             | 轻量 CTR 模型       | similarity/reranker |
| 精排   | Learning-to-Rank | DNN/Transformer | CrossEncoder        |
| 最终输出 | Top-N documents  | Top-N items     | Top-K chunks        |

- 粗排层的意义在于把多个 Recall 渠道得到的值再进行一次更精细的筛选，在本次作业中 RFF 扮演粗排的角色(当然本次作业没有精排了因为整个系统并没有很大)

- 所谓的CrossEncoder，就是把粗排后的 chunks 或者 候选答案 和 Query 拼接在一起，给一个 双向的神经网络模型，比如 T5，输出一个分数来直接评估相关性，这样往往更准
	- 最初的Recall环节我们只是把它们分别变成 Embedding，然后根据向量相似性匹配的，可能有误差


