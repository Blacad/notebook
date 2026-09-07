这段代码主要完成两个任务：

1. **Filter Activation Visualization**：观察一张给定图片经过某个卷积层的指定 filter 后，会产生怎样的 activation map。
    
2. **Filter Visualization**：反过来优化输入图片，寻找一张能够让指定 filter 激活尽可能大的图片，从而推测这个 filter 学到了什么特征。
    

---

### 1. 归一化函数

```python
def normalize(image):
    return (image - image.min()) / (image.max() - image.min())
```

将输入数据线性映射到 `[0, 1]`：

x′ =$\frac{x-x_{\min}}{x_{\max}-x_{\min}}$

主要用于后续可视化。

---

## 2. 保存中间层输出

```python
layer_activations = None
```

定义一个全局变量，用于保存指定 CNN 层 forward 后产生的 feature map。

---

## 3. `filter_explanation`

```python
def filter_explanation(x, model, cnnid, filterid, iteration=100, lr=1):
```

参数：

- `x`：输入图片。
    
- `model`：CNN 模型。
    
- `cnnid`：希望观察的卷积层编号。
    
- `filterid`：希望观察该层中的第几个 filter。
    
- `iteration`：优化输入图片的迭代次数。
    
- `lr`：优化输入图片时使用的学习率。
    

假设某层输出为：

```text
(B, C, H, W)
```

那么：

- `C` 就是该层 filter 的数量；
    
- `filterid` 指定其中一个 filter；
    
- 对应的 activation map 为 `(H, W)`。
    

---

# 一、Filter Activation：观察指定 filter 对原图的响应

### 4. 注册 Forward Hook

```python
def hook(model, input, output):
    global layer_activations
    layer_activations = output
```

Hook 的作用是：

> 当指定层进行 forward 时，把这一层的输出偷偷保存下来。

注册：

```python
hook_handle = model.cnn[cnnid].register_forward_hook(hook)
```

之后，只要模型 forward 经过：

```python
model.cnn[cnnid]
```

PyTorch 就会自动调用：

```python
hook(model, input, output)
```

其中：

```python
output
```

就是该层的输出 feature maps。

因此：

```python
layer_activations = output
```

会得到：

```text
(B, C, H, W)
```

---

### 5. 对原图进行 Forward

```python
model(x.cuda())
```

此时模型正常 forward。

当经过：

```python
model.cnn[cnnid]
```

时，hook 被触发，于是：

```python
layer_activations
```

保存了该层的 feature maps。

---

### 6. 取指定 Filter 的 Activation Map

```python
filter_activations = (
    layer_activations[:, filterid, :, :]
    .detach()
    .cpu()
)
```

如果：

```text
layer_activations.shape
=
(B, C, H, W)
```

那么：

```python
layer_activations[:, filterid, :, :]
```

得到：

```text
(B, H, W)
```

也就是第 `filterid` 个 filter 对输入图片产生的 activation map。

例如：

```text
原图
  ↓
Conv Layer
  ↓
Filter 0 → activation map 0
Filter 1 → activation map 1
Filter 2 → activation map 2
...
```

activation map 中值较大的位置表示：

> 该 filter 对输入图片这一局部区域响应较强。

例如某个 filter 学到了“竖直边缘”，那么图片中存在竖直边缘的位置可能会有较大的 activation。

这里：

```python
.detach().cpu()
```

表示：

- `detach()`：不再需要计算梯度；
    
- `cpu()`：移到 CPU，方便画图。
    

---

# 二、Filter Visualization：寻找最能激活 Filter 的图片

这一部分不是观察原图，而是：

> **把输入图片 `x` 本身作为待优化参数，通过梯度上升不断修改图片，使指定 filter 的 activation 最大。**

---

### 7. 让输入图片可以求梯度

```python
x = x.cuda()
x.requires_grad_()
```

通常训练模型时，我们优化：

θ\theta

即模型参数。

但这里模型参数不需要修改，而是优化：

xx

也就是输入图片本身。

因此需要：

```python
x.requires_grad_()
```

使 PyTorch 能够计算：

∂A∂x\frac{\partial A}{\partial x}

其中 $A$ 是指定 filter 的 activation。

---

### 8. 把图片交给优化器

```python
optimizer = Adam([x], lr=lr)
```

这里非常特殊。

正常训练：

```python
optimizer = Adam(model.parameters())
```

优化的是模型参数。

而现在：

```python
Adam([x])
```

表示：

> **Adam 优化器直接修改输入图片 `x`。**

也就是：

```text
普通训练：

loss
 ↓
∂loss/∂θ
 ↓
修改 model parameters


Filter Visualization：

filter activation
 ↓
∂activation/∂x
 ↓
修改 input image
```

---

## 9. 不断优化输入，使 Filter 激活越来越大

```python
for iter in range(iteration):
    optimizer.zero_grad()

    model(x)

    objective = -layer_activations[:, filterid, :, :].sum()

    objective.backward()

    optimizer.step()
```

### Step 1：Forward

```python
model(x)
```

由于之前已经注册 hook：

```python
layer_activations
```

再次得到指定 CNN 层的输出。

---

### Step 2：取指定 Filter 的总激活

```python
layer_activations[:, filterid, :, :].sum()
```

假设 activation map 是：

Ah,wA_{h,w}

则这里计算：

A=∑h,wAh,wA=\sum_{h,w}A_{h,w}

我们希望找到一张图片，使得：

AA

尽可能大。

即：

x∗=arg⁡max⁡x∑h,wAfilterid(x)h,wx^*= \arg\max_x \sum_{h,w} A_{filterid}(x)_{h,w}

---

### Step 3：为什么前面加负号

代码：

```python
objective = -layer_activations[:, filterid, :, :].sum()
```

因为 PyTorch 的优化器默认执行的是：

min⁡xL\min_x L

但我们的目标是：

max⁡xA(x)\max_x A(x)

因此等价地改写为：

min⁡x−A(x)\min_x -A(x)

所以定义：

L(x)=−∑h,wAfilterid(x)h,wL(x) = -\sum_{h,w} A_{filterid}(x)_{h,w}

这样最小化 `objective`，就相当于最大化 filter activation。

---

### 10. 对输入图片求梯度

```python
objective.backward()
```

计算：

∂L∂x\frac{\partial L}{\partial x}

由于：

L=−AL=-A

因此：

∂L∂x=−∂A∂x\frac{\partial L}{\partial x} = -\frac{\partial A}{\partial x}

这个梯度告诉我们：

> 输入图片应该怎样改变，才能使指定 filter 的 activation 进一步增加。

---

### 11. 修改输入图片

```python
optimizer.step()
```

Adam 根据：

∂L∂x\frac{\partial L}{\partial x}

直接修改图片 `x`。

因此整个迭代过程为：

```text
初始图片 x
    ↓
Forward
    ↓
指定 filter activation
    ↓
计算 activation 对 x 的梯度
    ↓
修改 x
    ↓
activation 更大
    ↓
再次 Forward
    ↓
...
```

经过很多次迭代以后，`x` 会逐渐变成一张能够强烈激活该 filter 的图片。

---

## 12. 得到 Filter Visualization

```python
filter_visualizations = x.detach().cpu().squeeze()
```

此时 `x` 已经不再是最开始的输入图片，而是经过多次梯度优化后的图片：

x∗=arg⁡max⁡xAfilterid(x)x^* = \arg\max_x A_{filterid}(x)

这张图片可以帮助理解：

> **该 filter 最喜欢检测什么样的视觉模式。**

例如某个 filter 的最终 visualization 可能表现为：

- 水平边缘；
    
- 竖直边缘；
    
- 某种颜色；
    
- 某种纹理；
    
- 圆形结构；
    
- 更深层网络中的复杂物体局部。
    

通常来说，CNN 层越浅：

```text
edge / color / simple texture
```

越深：

```text
complex texture / shape / semantic parts
```

---

# 13. 删除 Hook

```python
hook_handle.remove()
```

`register_forward_hook()` 注册的 hook 会一直存在。

如果不删除，那么以后每一次：

```python
model(x)
```

经过这个层时都会继续执行 hook。

因此使用完成以后需要：

```python
hook_handle.remove()
```

避免：

- 重复执行 hook；
    
- 多次注册产生混乱；
    
- 不必要的内存占用。
    

---

# 14. 两个返回值的区别

```python
return filter_activations, filter_visualizations
```

### `filter_activations`

```text
给定一张真实图片 x
        ↓
经过指定 filter
        ↓
这个 filter 在哪里响应最强？
```

即 **Activation Map**。

回答：

> **这个 filter 在当前图片的哪些位置被激活？**

---

### `filter_visualizations`

```text
不断修改输入图片
        ↓
让指定 filter 激活越来越强
        ↓
最终得到一张人工合成图片
```

回答：

> **什么样的输入模式最容易激活这个 filter？**

因此二者不要混淆：

|方法|输入是否改变|目的|
|---|---|---|
|Filter Activation|不改变|看 filter 在原图哪些地方响应|
|Filter Visualization|不断优化输入|看 filter 喜欢什么视觉模式|

---

## 整体流程

```text
                    指定 CNN Layer + Filter
                              │
             ┌────────────────┴────────────────┐
             │                                 │
     Filter Activation                 Filter Visualization
             │                                 │
         固定原图 x                         优化输入 x
             │                                 │
         model(x)                          model(x)
             │                                 │
        Forward Hook                      Forward Hook
             │                                 │
      得到 activation map             得到 activation map
             │                                 │
   看原图哪里激活该 filter          最大化该 filter activation
                                               │
                                          ∂A / ∂x
                                               │
                                          修改图片 x
                                               │
                                           重复迭代
                                               │
                                     最能激活 filter 的图片
```

核心区别可以记成：

> **Activation Map 是“给定图片，看 filter 响应在哪里”；Filter Visualization 是“给定 filter，反过来寻找它最喜欢看到什么”。**