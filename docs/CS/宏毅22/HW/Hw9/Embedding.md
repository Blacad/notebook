这段代码的核心目的，是**把 BERT 每一层中每个 token 的 768 维 hidden state 用 PCA 降到二维，然后画出来**，观察随着 BERT 层数加深，question、context、answer 对应 token 的表示如何变化

整体流程可以概括为：

> 原始文本 → Tokenizer → BERT 各层 hidden state → PCA 768→2 → 不同颜色画出 question/context/answer token

---

## 1. 先对 question 和 context 编码

```python
inputs = Tokenizer(
    questions[QUESTION-1],
    contexts[QUESTION-1],
    return_tensors='pt'
)
```

这里假设 `Tokenizer` 是 BERT tokenizer，比如：

```python
BertTokenizerFast.from_pretrained(...)
```

输入两个字符串：

```text
question
context
```

BERT tokenizer 通常会组织成：

```text
[CLS] question [SEP] context [SEP]
```

例如：

```text
Question: Where was Tom born?
Context: Tom was born in London.
```

可能变成：

```text
[CLS]
where
was
tom
born
?
[SEP]
tom
was
born
in
london
.
[SEP]
```

因此：

```python
inputs['input_ids']
```

的 shape 一般是：

```text
[batch_size, sequence_length]
```

这里只有一个样本，所以：

```text
[1, sequence_length]
```

---

# 2. 找 question 和 context 所在的位置

```python
question_start, question_end = \
    1, inputs['input_ids'][0].tolist().index(102) - 1
```

这里的 `102` 是标准 BERT vocab 中：

```text
[SEP] = 102
```

而：

```text
[CLS] = 101
```

假设输入是：

```text
位置：
0       1   2   3   4      5      6   7   8   9   10      11
[CLS]   q1  q2  q3  q4   [SEP]   c1  c2  c3  c4  ...    [SEP]
```

那么：

```python
question_start = 1
```

因为位置 0 是 `[CLS]`。

而：

```python
inputs['input_ids'][0].tolist().index(102)
```

找到**第一个 `[SEP]` 的位置**。

假设 `[SEP]` 位于 5，那么：

```python
question_end = 5 - 1 = 4
```

所以 question token 范围是：

```text
[1, 4]
```

---

接下来：

```python
context_start, context_end = \
    question_end + 2, len(inputs['input_ids'][0]) - 2
```

因为：

```text
question_end + 1
```

是 `[SEP]`，

因此：

```text
question_end + 2
```

才是 context 的第一个 token。

最后：

```python
len(...) - 1
```

是最后一个 `[SEP]`，

所以：

```python
len(...) - 2
```

是 context 的最后一个普通 token。

最终：

```text
[CLS] question [SEP] context [SEP]
       ↑       ↑     ↑      ↑
       q_start q_end c_start c_end
```

---

# 3. 加载之前保存好的 hidden states

```python
outputs_hidden_states = torch.load(
    f"hw9_bert/output/model_q{QUESTION}"
)
```

这里加载的应该是之前通过类似：

```python
outputs = model(
    ...,
    output_hidden_states=True
)

outputs.hidden_states
```

保存下来的结果。

对于标准的 **BERT-base 12层模型**：

```python
outputs.hidden_states
```

通常是一个长度为 **13** 的 tuple：

```text
hidden_states[0]   embedding layer 输出
hidden_states[1]   Transformer layer 1 输出
hidden_states[2]   Transformer layer 2 输出
...
hidden_states[12]  Transformer layer 12 输出
```

每一个元素的 shape 都类似：

```text
[batch_size, sequence_length, hidden_size]
```

BERT-base 中：

```text
hidden_size = 768
```

所以这里就是：

```text
[1, sequence_length, 768]
```

---

# 4. 遍历 BERT 的 12 个 Transformer layer

```python
for layer_index, embeddings in enumerate(
    outputs_hidden_states[1:]
):
```

注意这里：

```python
outputs_hidden_states[1:]
```

把第 0 个 embedding layer 跳过了。

因此实际上遍历：

```text
Layer 1
Layer 2
...
Layer 12
```

此时：

```python
embeddings
```

的 shape：

```text
[1, sequence_length, 768]
```

其中：

```python
embeddings[0]
```

就是：

```text
[sequence_length, 768]
```

可以理解成：

```text
token 1 → 768维向量
token 2 → 768维向量
token 3 → 768维向量
...
```

---

# 5. PCA 把 768 维降到 2 维

```python
reduced_embeddings = PCA(
    n_components=2,
    random_state=0
).fit_transform(embeddings[0])
```

原来的：

```python
embeddings[0]
```

shape 是：

```text
[sequence_length, 768]
```

PCA 把每个 token 的 768 维 hidden state 压缩成 2 维：

```text
[sequence_length, 2]
```

于是原来：

```text
token_i → [h1, h2, ..., h768]
```

现在变成：

```text
token_i → [x_i, y_i]
```

这样就可以在二维坐标系中画出来。

这里 PCA 的目标是找两个方差最大的方向。

如果原始 token hidden state 是：

hi∈R768\mathbf h_i \in \mathbb R^{768}

PCA 相当于寻找一个二维子空间：

zi=W⊤(hi−μ)\mathbf z_i = W^\top(\mathbf h_i-\boldsymbol\mu)

其中：

zi∈R2\mathbf z_i\in\mathbb R^2

所以每个 token 最终都对应二维图中的一个点。

---

# 6. 遍历所有 token

```python
for i, token_id in enumerate(inputs['input_ids'][0]):
```

这里：

```python
i
```

表示 token 在 sequence 中的位置。

例如：

```text
i = 0 → [CLS]
i = 1 → where
i = 2 → was
...
```

而：

```python
token_id
```

是对应的 vocabulary ID。

---

# 7. 获取这个 token PCA 后的位置

```python
x, y = reduced_embeddings[i]
```

因为：

```python
reduced_embeddings.shape
```

是：

```text
[sequence_length, 2]
```

所以：

```python
reduced_embeddings[i]
```

就是：

```text
[x, y]
```

这个 token 就可以画在坐标：

```text
(x, y)
```

上。

---

# 8. 把 token ID 解码回文本

```python
word = Tokenizer.decode(token_id)
```

例如：

```python
token_id = 1996
```

可能得到：

```text
"the"
```

因此后面画图时可以直接把 token 的文字写在点旁边。

---

# 9. 根据 token 类型设置颜色

代码的重点在这里：

```python
if word in answers[QUESTION-1].split():
    plt.scatter(x, y, color='blue', marker='d')
```

如果这个 token 出现在答案文本中：

```text
answer
```

就画成：

- 蓝色
    
- 菱形 `d`
    

---

否则：

```python
elif question_start <= i <= question_end:
    plt.scatter(x, y, color='red')
```

如果 token 位于 question 区域：

```text
red
```

---

否则：

```python
elif context_start <= i <= context_end:
    plt.scatter(x, y, color='green')
```

如果 token 位于 context：

```text
green
```

---

否则：

```python
else:
    continue
```

一般就是：

```text
[CLS]
[SEP]
```

这些特殊 token 不画。

最终颜色含义：

|token 类型|颜色|marker|
|---|---|---|
|answer|蓝色|菱形|
|question|红色|圆点|
|context|绿色|圆点|
|`[CLS]/[SEP]`|不画|—|

要特别注意，因为 `answer` 判断写在最前面，所以：

> 如果一个 token 既属于 context，又恰好是 answer 中的词，它最终会被画成蓝色，而不是绿色。

这正是作者想突出答案 token 的做法。

---

# 10. 在点旁边写 token

```python
plt.text(
    x + 0.1,
    y + 0.2,
    word,
    fontsize=12
)
```

例如某个 `"London"` token 的点在：

```text
(1.5, 2.3)
```

就在稍微偏右上角的位置：

```text
(1.6, 2.5)
```

写：

```text
London
```

这样就能知道每个点具体对应哪个 token。

---

# 11. 为什么要画“空点”

```python
plt.plot([], label='answer', color='blue', marker='d')
plt.plot([], label='question', color='red', marker='o')
plt.plot([], label='context', color='green', marker='o')
```

这里：

```python
[]
```

意味着实际上**没有画任何数据点**。

这样做只是为了人为构造 legend。

如果没有这三行，前面的：

```python
plt.scatter(...)
```

又没有设置 `label`，那么：

```python
plt.legend()
```

不知道：

```text
蓝色是什么
红色是什么
绿色是什么
```

因此作者人为创建三个不可见对象，让 legend 显示：

```text
◇ answer
● question
● context
```

---

# 12. 显示图例

```python
plt.legend(loc='best')
```

Matplotlib 自动找一个比较合适的位置放 legend。

---

# 13. 标记当前是第几层

```python
plt.title('Layer ' + str(layer_index + 1))
```

因为：

```python
layer_index
```

由 `enumerate` 从 0 开始：

```text
0, 1, ..., 11
```

所以加 1 得到：

```text
Layer 1
Layer 2
...
Layer 12
```

---

# 14. 每一层画一幅图

```python
plt.show()
```

因此最后会产生 **12 张图**：

```text
Layer 1
Layer 2
...
Layer 12
```

每张图表示：

> 该 Transformer layer 中，question/context/answer 的 token hidden state 在 PCA 二维空间中的分布。

---

# 这段代码真正想观察什么？

假设某一层得到：

```text
question token：
where      ●
born       ●
location   ●

context：
Tom        ●
London     ●
England    ●

answer：
London     ◆
```

在浅层 BERT 中，token embedding 更多反映**词汇、位置、局部语义**。

随着层数加深，self-attention 不断让 token 与其他 token 交互：

```text
Layer 1
token 本身信息很多

↓

Layer 4
开始融入上下文

↓

Layer 8
question 和 context 强烈交互

↓

Layer 12
与答案判断有关的语义逐渐形成
```

于是可能观察到一些 token 在二维空间里逐渐靠近。

例如 question 是：

```text
Where was Einstein born?
```

context 中有：

```text
Einstein was born in Ulm.
```

随着层数加深：

```text
Where
born
Ulm
```

对应 hidden state 的结构可能发生明显变化。

因此这实际上是在**可视化 BERT contextualized representation 的逐层演化**。

---

## 不过这段代码有两个值得注意的问题

第一个是：

```python
if word in answers[QUESTION-1].split():
```

这个判断并不严格。

因为 BERT 使用 WordPiece，例如：

```text
playing
```

可能被切成：

```text
play
##ing
```

此时：

```python
Tokenizer.decode(token_id)
```

得到的 token 和：

```python
answer.split()
```

不一定一致。

对于中文，这个问题会更加明显。

更可靠的方法应该根据 **answer 的 token position / offset mapping** 判断，而不是字符串 membership。

---

第二个问题是每一层都独立执行：

```python
PCA(...).fit_transform(...)
```

也就是说：

```text
Layer 1 有自己的 PCA 坐标系
Layer 2 有自己的 PCA 坐标系
...
Layer 12 有自己的 PCA 坐标系
```

所以不能直接说：

> Layer 1 中 `(1,2)` 的点移动到了 Layer 2 的 `(3,4)`。

因为两张图的 PCA 坐标轴本身就不同，甚至可能发生旋转、翻转。

它更适合观察的是：

> **每层内部 token 之间的相对分布和聚类结构发生了什么变化。**

而不是严格追踪一个 token 在统一二维坐标系中的“移动轨迹”。

一句话总结：

**这段代码就是把 BERT 每一层的 `[sequence_length, 768]` token hidden states 用 PCA 变成 `[sequence_length, 2]`，然后分别用红、绿、蓝表示 question、context、answer，从而观察 BERT 12 层中 token 表征结构如何逐渐演化。**