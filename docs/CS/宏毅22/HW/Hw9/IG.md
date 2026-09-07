这段代码实现的是 **Integrated Gradients（积分梯度，IG）**，用于分析：

> **输入图片中的每个像素/通道，对模型预测某个目标类别 `target_class` 的贡献有多大。**

它和前面的 Saliency Map 都属于 **基于梯度的可解释性方法**，但 Integrated Gradients 不只计算原图这一个点的梯度，而是从一个 baseline 一路走到原图，沿途计算梯度并累积。

---

## 1. Integrated Gradients 想解决什么问题

普通 Saliency Map 直接计算：

∂Fc(x)∂x\frac{\partial F_c(x)}{\partial x}

其中：

- $x$：输入图片；
    
- $F_c(x)$：模型对类别 $c$ 的输出 score。
    

它的问题是：**原图所在位置的梯度可能很小，甚至接近 0，但这个特征实际上可能非常重要。**

例如某个神经元已经进入饱和区：

```text
          输出
           │           ______
           │         /
           │       /
           │_____/
           └──────────── 输入
                         ↑
                         x
```

此时当前点的：

∂F(x)∂x≈0\frac{\partial F(x)}{\partial x}\approx 0

但这不意味着输入不重要，只是当前点的局部梯度很小。

Integrated Gradients 因此不只看 $x$，而是考察：

```text
baseline → → → → → 原图 x
```

整条路径上的梯度。

---

# 2. 这段代码使用的 baseline 是什么

这里：

```python
xbar_list = [
    input_image * step / steps
    for step in range(steps)
]
```

实际上隐含选择了：

x′=0x'=0

作为 baseline。

所以生成的图片大概是：

```text
0 × image
0.01 × image
0.02 × image
...
0.99 × image
```

也就是从一张全零图片逐渐变成原图。

一般 Integrated Gradients 的路径定义为：

x(α)=x′+α(x−x′)x(\alpha) = x'+\alpha(x-x')

其中：

$\alpha\in[0,1]$

你的代码因为：

x′=0x'=0

所以变成：

$x(\alpha)=\alpha x$

---

# 3. `generate_images_on_linear_path`

```python
def generate_images_on_linear_path(self, input_image, steps):
    xbar_list = [
        input_image * step / steps
        for step in range(steps)
    ]
    return xbar_list
```

假设：

```python
steps = 5
```

那么大约生成：

```text
0.0 x
0.2 x
0.4 x
0.6 x
0.8 x
```

所以这是在构造：

> baseline 和原图之间的线性路径上的多个采样点。

---

# 4. `generate_gradients` 在计算什么

核心：

```python
model_output = self.model(input_image)
```

假设模型有 5 类：

```text
model_output =
[class0, class1, class2, class3, class4]
```

然后：

```python
one_hot_output = torch.FloatTensor(
    1, model_output.size()[-1]
).zero_().cuda()

one_hot_output[0][target_class] = 1
```

例如：

```python
target_class = 2
```

则：

```text
one_hot_output =
[0, 0, 1, 0, 0]
```

之后：

```python
model_output.backward(
    gradient=one_hot_output
)
```

本质上就是只对：

$F_2(x)$

进行反向传播。

最终：

```python
input_image.grad
```

得到：

$\frac{\partial F_2(x)} {\partial x}$

即：

> **目标类别 2 的 score 对每个输入像素的梯度。**

所以这里和你前面那段基于 CrossEntropyLoss 的 Saliency Map 有一点区别。

这里直接解释：

Fc(x)F_c(x)

也就是目标类别的 score；

而之前那段代码解释的是：

LCEL_{\mathrm{CE}}

对输入的敏感性。

---

# 5. Integrated Gradients 的核心计算

代码：

```python
for xbar_image in xbar_list:

    single_integrated_grad = self.generate_gradients(
        xbar_image,
        target_class
    )

    integrated_grads = (
        integrated_grads
        + single_integrated_grad / steps
    )
```

假设路径上有 $m$ 个点：

$x_1,x_2,\ldots,x_m$

那么它实际上在计算：

$\frac{1}{m} \sum_{k=1}^{m} \frac{\partial F_c(x_k)} {\partial x_k}$

也就是：

> 沿 baseline → 原图 的路径，把每个位置上的梯度取平均。

---

# 6. 标准 Integrated Gradients 其实还多一步

严格来说，标准 IG 定义是：

IGi(x)=(xi−xi′)∫01∂Fc(x′+α(x−x′))∂xidαIG_i(x) = (x_i-x'_i) \int_0^1 \frac{\partial F_c \left( x'+\alpha(x-x') \right)} {\partial x_i} d\alpha

数值近似：

IGi(x)≈(xi−xi′)1m∑k=1m∂Fc(x′+km(x−x′))∂xiIG_i(x) \approx (x_i-x'_i) \frac{1}{m} \sum_{k=1}^{m} \frac{ \partial F_c \left( x'+\frac{k}{m}(x-x') \right) }{ \partial x_i }

注意最前面还有：

(xi−xi′)(x_i-x'_i)

而你这段代码最后只有：

```python
integrated_grads += gradient / steps
```

**没有乘：**

```python
input_image - baseline
```

所以严格来说，这段实现计算的是：

> **沿路径的平均梯度**

而不是完整定义下的 Integrated Gradients attribution。

因为这里 baseline 是 0，标准实现最后应该类似：

```python
integrated_grads = input_image * average_gradients
```

即：

IG(x)=x⊙AverageGradientIG(x) = x\odot \text{AverageGradient}

---

# 7. 它最终分析的是什么

最终：

```python
return integrated_grads[0]
```

得到大约：

```text
(3, H, W)
```

例如：

```text
(3, 128, 128)
```

表示 RGB 三个通道中，每个输入位置对于：

```python
target_class
```

的贡献/敏感程度。

可以概括为：

```text
baseline
   ↓
生成 baseline → 原图之间的一系列图片
   ↓
每张图计算：
∂ target_class_score / ∂ input
   ↓
沿路径累积 / 平均梯度
   ↓
得到每个像素对目标类别的重要性
```

---

# 8. 和 Saliency Map、SmoothGrad 的区别

可以放在一起记：

|方法|核心思想|
|---|---|
|Saliency Map|只在原图 $x$ 计算一次梯度|
|SmoothGrad|在原图附近加随机噪声，多次算梯度再平均|
|Integrated Gradients|从 baseline 到原图沿一条路径，多次算梯度再积分|

Saliency：

$S(x) = \left| \frac{\partial F_c(x)} {\partial x} \right|$

SmoothGrad：

$SG(x) = \frac1N \sum_i \left| \frac{\partial F_c(x+\epsilon_i)} {\partial x} \right|$

Integrated Gradients：

$G_i(x) = (x_i-x'_i) \int_0^1 \frac{\partial F_c(x'+\alpha(x-x'))} {\partial x_i} d\alpha$

所以一句话记忆：

> **Saliency 看“当前点的梯度”，SmoothGrad 看“原图附近的平均梯度”，Integrated Gradients 看“从 baseline 走到原图整个路径上的累计梯度”。**

另外，你这份代码名字叫 `IntegratedGradients`，但从严格公式看**少了最后的 $(x-x')$ 这一项**，因此更准确地说它目前实现的是 Integrated Gradients 中的“路径平均梯度”部分。