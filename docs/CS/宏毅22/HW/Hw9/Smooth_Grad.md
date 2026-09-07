**SmoothGrad**：

> 对同一张原图加入多次随机噪声，每次计算一张 Saliency Map，最后把这些 Saliency Map 平均，从而降低普通 Saliency Map 中的噪声。

可以把它写成：

$\mathrm{SmoothGrad}(x) = \frac{1}{N} \sum_{i=1}^{N} S(x+\epsilon_i)$

其中：

$\epsilon_i\sim\mathcal N(0,\sigma^2)$

而 $S(\cdot)$ 就是前面讲的 gradient-based saliency。

---

你的代码实际流程就是：

```text
原图 x
 │
 ├─ + noise₁ → x₁ → model → loss → |∂L/∂x₁| ─┐
 │                                             │
 ├─ + noise₂ → x₂ → model → loss → |∂L/∂x₂| ─┤
 │                                             │
 ├─ + noise₃ → x₃ → model → loss → |∂L/∂x₃| ─┤
 │                                             │
 │                    ...                      │
 │                                             │
 └─ + noise₅₀₀ → ... → |∂L/∂x₅₀₀| ──────────┤
                                               ↓
                                          求平均
                                               ↓
                                          SmoothGrad
```

### 为什么这样能变 smooth？

普通 Saliency Map：

S(x)=∣∂L∂x∣S(x)= $\left| \frac{\partial L}{\partial x} \right|$

有一个问题：神经网络在单个输入点附近的梯度可能非常不稳定。

比如原图附近几个非常接近的点：

$x+\epsilon_1,\quad x+\epsilon_2,\quad x+\epsilon_3$

得到的梯度可能：

```text
位置 A：

原图       0.8
noise 1    0.3
noise 2    0.9
noise 3    0.4
...
```

里面会有一些偶然产生的高梯度。

但如果某个区域**真的对模型重要**，那么在原图附近很多输入上，它通常都会保持较大的梯度：

```text
重要区域：
0.8, 0.7, 0.9, 0.75, 0.82 ...
                ↓
              平均后仍大

偶然噪声区域：
0.8, 0.01, 0.03, 0.02, 0.01 ...
                ↓
              平均后变小
```

所以 SmoothGrad 本质上就是通过**邻域平均消除梯度噪声**。

---

你代码里的核心部分：

```python
for i in range(epoch):
    noise = ...
    x_mod = x + noise

    y_pred = model(x_mod)
    loss = loss_func(y_pred, y)

    loss.backward()

    smooth += x_mod.grad.abs()
```

这里 `epoch=500` 实际上不是训练 epoch，更准确的名字应该是：

```python
num_samples = 500
```

也就是：

> 对这张图片生成 500 个不同的 noisy versions。

最后：

```python
smooth = normalize(smooth / epoch)
```

就是：

$\frac{1}{500} \sum_{i=1}^{500} \left| \frac{\partial L(x+\epsilon_i)} {\partial (x+\epsilon_i)} \right|$

然后再归一化到 `[0,1]`。

---

不过这段代码有一个值得注意的问题：

```python
.normal_(mean, sigma**2)
```

PyTorch 的：

```python
normal_(mean, std)
```

第二个参数是**标准差**，不是方差。

所以如果你的：

```python
sigma
```

本身定义的是标准差，那么这里应该写：

```python
.normal_(mean, sigma)
```

而不是：

```python
.normal_(mean, sigma**2)
```

因为高斯分布记作：

$\epsilon\sim\mathcal N(0,\sigma^2)$

这里虽然数学表达中的第二项是 $\sigma^2$，但 PyTorch API 要传入的是 $\sigma$。

---

还有一点和你前面的 Saliency Map 实现稍有不同。

前面是：

```python
saliencies, _ = torch.max(x.grad.abs(), dim=1)
```

把 RGB 三个通道：

(C,H,W)(C,H,W)

压成：

(H,W)(H,W)

而这里：

```python
smooth += x_mod.grad.abs()
```

没有对 channel 做 `max`，所以最终：

```text
smooth.shape
≈ (1, 3, 128, 128)
```

然后你后面：

```python
img.reshape(3,128,128)
```

再直接当 RGB 图显示。

因此这个 SmoothGrad 实现保留的是**三个通道各自的梯度显著性**。

如果你想和前面的 Saliency Map 完全一致，更常见的是最后再：

```python
smooth = smooth.mean(axis=0)   # 去掉 batch
smooth = np.max(smooth, axis=0)
```

得到：

```text
(128, 128)
```

的单通道显著图。

所以一句话总结：

> **普通 Saliency Map 是在原图 $x$ 上计算一次梯度；SmoothGrad 是在 $x$ 周围加入很多小的随机噪声，计算很多次 Saliency Map 后取平均，从而获得更稳定、更平滑的显著性解释。**

而且这里的 `smooth` 并不是对最终生成的 Saliency Map 做传统的图像模糊，而是**通过输入邻域上的梯度平均来实现“平滑”**。