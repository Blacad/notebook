
这段代码计算的是一种最基础的 **gradient-based saliency map（梯度显著图）**。核心思想是：

> 看模型的损失对输入图片每个像素有多敏感。某个像素稍微变化就会让 loss 变化很大，那么这个像素就被认为比较重要。

代码如下：

```python
def compute_saliency_maps(x, y, model):
    model.eval()

    x = x.cuda()

    # we want the gradient of the input x
    x.requires_grad_()

    y_pred = model(x)

    loss_func = torch.nn.CrossEntropyLoss()
    loss = loss_func(y_pred, y.cuda())

    loss.backward()

    saliencies, _ = torch.max(
        x.grad.data.abs().detach().cpu(),
        dim=1
    )

    saliencies = torch.stack([
        normalize(item) for item in saliencies
    ])

    return saliencies
```

## 1. `model.eval()`

```python
model.eval()
```

把模型切换到推理模式。

主要影响：

- Dropout 关闭；
    
- BatchNorm 使用训练阶段保存的 running mean / variance。
    

因为现在目的是解释模型，而不是训练模型，所以通常使用 `eval()`。

不过需要注意：

> `eval()` 不等于关闭梯度。

后面依然可以正常 `backward()`。

---

## 2. 把输入放到 GPU

```python
x = x.cuda()
```

假设输入图片：

```text
x.shape = (B, C, H, W)
```

例如：

```text
(32, 3, 224, 224)
```

表示：

- 32 张图片；
    
- RGB 3 个通道；
    
- 高宽 224 × 224。
    

---

## 3. `x.requires_grad_()`

这是这段代码最关键的地方：

```python
x.requires_grad_()
```

平时训练神经网络，我们关注的是：

$\frac{\partial L}{\partial \theta}$

也就是 loss 对模型参数 $\theta$ 的梯度。

但 saliency map 关注的是：

$\frac{\partial L}{\partial x}$

也就是：

> **loss 对输入图片每一个像素值的梯度。**

所以必须让 PyTorch 对输入 `x` 保存梯度。

执行完：

```python
loss.backward()
```

以后，就可以通过：

```python
x.grad
```

得到：

$\frac{\partial L}{\partial x}$

---

## 4. 正常进行一次 forward

```python
y_pred = model(x)
```

假设模型有 5 个分类：

```text
y_pred.shape = (B, 5)
```

例如某张图片：

```python
y_pred[0]
```

可能是 logits：

```text
[1.2, -0.3, 4.7, 0.8, -1.1]
```

说明模型最倾向类别 2。

---

## 5. 计算 CrossEntropy loss

```python
loss_func = torch.nn.CrossEntropyLoss()
loss = loss_func(y_pred, y.cuda())
```

如果真实标签：

```python
y[0] = 2
```

那么 CrossEntropy 希望类别 2 的概率更高。

也就是说：

L=−log⁡P(y=2∣x)L = -\log P(y=2|x)

因此后面的：

\frac{\partial L}{\partial x}

表示：

> 输入的每个像素发生变化时，会对“当前分类损失”产生多大影响。

---

# 6. `loss.backward()` 到底算出了什么

```python
loss.backward()
```

PyTorch 会沿着整个网络反向传播：

```text
loss
 ↑
classifier
 ↑
feature
 ↑
conv
 ↑
x
```

最终：

```python
x.grad
```

得到与输入完全相同的 shape：

```text
(B, C, H, W)
```

即：

$\frac{\partial L}{\partial x_{b,c,h,w}}$

例如一个像素位置：

∣∂L∂x0,0,120,80∣=0.7$\left| \frac{\partial L} {\partial x_{0,0,120,80}} \right|$ =0.7

而另一个位置：

∣∂L∂x0,0,30,40∣=0.002$\left| \frac{\partial L} {\partial x_{0,0,30,40}} \right|$ =0.002

说明前一个像素稍微变化，loss 就会明显改变，因此模型对这个位置更加敏感。

所以：

> **梯度绝对值越大 → 这个输入位置对当前预测越重要。**

---

# 7. 为什么取绝对值

```python
x.grad.data.abs()
```

原始梯度可能有正有负：

```text
+0.8
-0.7
+0.01
```

正负代表变化方向不同：

- 正梯度：增大该像素会让 loss 增大；
    
- 负梯度：增大该像素会让 loss 减小。
    

但 Saliency Map 通常主要关心：

> 这个像素有多重要？

而不是当前变化方向。

所以取：

∣∂L∂x∣\left| \frac{\partial L}{\partial x} \right|

例如：

```text
+0.8 → 0.8
-0.7 → 0.7
```

两者都表示高度敏感。

---

# 8. 为什么 `torch.max(..., dim=1)`

这一句非常重要：

```python
saliencies, _ = torch.max(
    x.grad.data.abs().detach().cpu(),
    dim=1
)
```

之前：

```text
x.grad.shape
=
(B, C, H, W)
```

RGB 图片：

```text
(B, 3, H, W)
```

也就是说每个像素 `(h,w)` 有三个梯度：

gR,gG,gBg_R,\quad g_G,\quad g_B

例如：

```text
pixel (100, 120)

R梯度 = 0.1
G梯度 = 0.7
B梯度 = 0.2
```

代码：

```python
torch.max(..., dim=1)
```

是在 RGB channel 维度取最大值：

S(h,w)=max⁡c∣∂L∂xc,h,w∣S(h,w) = \max_c \left| \frac{\partial L}{\partial x_{c,h,w}} \right|

于是：

```text
max(0.1, 0.7, 0.2) = 0.7
```

原本：

```text
(B, 3, H, W)
```

变成：

```text
(B, H, W)
```

这样每个像素只剩下一个 saliency value，可以直接画成灰度热力图。

---

## 为什么用 max 而不是平均？

因为只要 RGB 中某个 channel 对模型非常敏感，就认为这个像素位置比较重要。

例如：

```text
像素A：
R=0.9 G=0.01 B=0.02
```

如果平均：

0.31

会削弱其显著程度。

而 max：

0.9

可以保留最强响应。

当然，也有其他 Saliency Map 实现会使用：

1C∑c∣gc∣\frac{1}{C}\sum_c |g_c|

或者：

∑cgc2\sqrt{\sum_c g_c^2}

并不是一定只能用 max。

---

# 9. `.detach().cpu()`

```python
.detach().cpu()
```

其中：

```python
.detach()
```

表示后续不再需要 autograd 跟踪。

因为现在已经拿到结果了，只需要拿来显示。

而：

```python
.cpu()
```

把数据从 GPU 搬到 CPU，方便后续 NumPy / matplotlib 处理。

---

# 10. normalization 是干什么的

```python
saliencies = torch.stack([
    normalize(item)
    for item in saliencies
])
```

因为不同图片的梯度绝对大小可能差很多。

例如：

```text
图片1 saliency：
0 ~ 0.005

图片2 saliency：
0 ~ 20
```

如果直接显示，颜色尺度会完全不同。

所以通常会对每张图单独归一化，例如：

S′=S−Smin⁡Smax⁡−Smin⁡S' = \frac{S-S_{\min}} {S_{\max}-S_{\min}}

让：

S′∈[0,1]S'\in[0,1]

于是所有图片都能比较清楚地显示：

```text
0   → 不重要
1   → 非常重要
```

---

# 11. 最终返回的 Saliency Map 是什么

最终：

```python
saliencies.shape
```

一般是：

```text
(B, H, W)
```

例如：

```text
(32, 224, 224)
```

每个位置的值代表：

> **该像素位置的 RGB 通道中，loss 对输入最敏感的梯度大小。**

然后可以：

```python
plt.imshow(saliencies[0], cmap='hot')
```

得到热力图。

亮的区域：

```text
梯度绝对值大
    ↓
输入稍微改变
    ↓
模型输出/损失变化明显
    ↓
模型比较依赖这里
```

---

# 12. 和刚才 LIME 的区别

这两个方法其实非常适合放在一起理解。

LIME 是：

```text
遮掉一个 superpixel
        ↓
重新运行模型
        ↓
看输出变化多少
        ↓
判断区域重要性
```

属于**扰动式解释**。

而 Saliency Map 是：

```text
输入 x
 ↓
forward
 ↓
loss
 ↓
backward
 ↓
∂loss/∂x
 ↓
判断像素重要性
```

属于**梯度式解释**。

所以可以概括：

||LIME|Saliency Map|
|---|---|---|
|是否需要模型梯度|不需要|需要|
|是否需要访问模型内部|不需要|需要|
|基本解释单位|superpixel|pixel|
|方法|扰动 + 局部代理模型|输入梯度|
|核心量|superpixel 权重|$\left|

---

## 一个需要特别注意的地方

你的代码计算的是：

∣∂LCE∂x∣\left| \frac{\partial L_{\mathrm{CE}}}{\partial x} \right|

严格来说，经典 Saliency Map 也经常直接计算**目标类别 score 对输入的梯度**：

∣∂fy(x)∂x∣\left| \frac{\partial f_y(x)} {\partial x} \right|

例如直接：

```python
score = y_pred[torch.arange(len(y)), y]
score.sum().backward()
```

这样问的是：

> 哪些像素最影响类别 $y$ 的 score？

而你的实现问的是：

> 哪些像素最影响当前样本的 CrossEntropy loss？

两者通常高度相关，但概念上并不完全一样。尤其因为

L=−log⁡P(y∣x)L=-\log P(y|x)

所以 loss gradient 的方向实际上是“使分类变差/变好”的方向，而 class-score gradient 更直接对应“这个类别分数对输入的敏感性”。

如果是为了理解最经典的 Saliency Map 定义，建议记住：

Sh,w=max⁡c∣∂fy(x)∂xc,h,w∣\boxed{ S_{h,w} = \max_c \left| \frac{\partial f_y(x)} {\partial x_{c,h,w}} \right| }

而你这段代码只是把 $f_y(x)$ 换成了 CrossEntropy loss。