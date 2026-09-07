当 `torch.min` / `torch.max` 用来比较**两个张量**时，默认行为是：**逐元素比较，返回对应位置的最小值/最大值**。

例如：

```python
a = torch.tensor([1, 5, 3])
b = torch.tensor([2, 4, 6])

torch.min(a, b)
# tensor([1, 4, 3])

torch.max(a, b)
# tensor([2, 5, 6])
```

也就是：

ci=min⁡(ai,bi)c_i = \min(a_i,b_i)

或者：

ci=max⁡(ai,bi)c_i = \max(a_i,b_i)

对于二维张量也是逐位置比较：

```python
a = torch.tensor([
    [1, 5],
    [7, 2]
])

b = torch.tensor([
    [3, 4],
    [6, 8]
])

torch.min(a, b)
```

得到：

```text
tensor([
    [1, 4],
    [6, 2]
])
```

因为逐元素进行：

```text
min(1,3) = 1
min(5,4) = 4
min(7,6) = 6
min(2,8) = 2
```

### 支持 broadcasting

两个 tensor 不一定 shape 完全一致，只要满足 PyTorch 的 broadcasting 规则即可：

```python
a = torch.tensor([
    [1, 5, 3],
    [7, 2, 9]
])

b = torch.tensor([2, 4, 6])

torch.min(a, b)
```

`b` 会广播成：

```text
[[2, 4, 6],
 [2, 4, 6]]
```

因此结果为：

```text
[[1, 4, 3],
 [2, 2, 6]]
```

---

需要注意，`torch.min` 有几种不同的调用方式，行为不同：

```python
torch.min(a, b)
```

是**两个张量逐元素比较**。

而：

```python
torch.min(a)
```

是对整个 tensor 做 reduction，得到全局最小值：

```python
a = torch.tensor([[1, 5], [3, 2]])

torch.min(a)
# tensor(1)
```

而：

```python
torch.min(a, dim=1)
```

是沿某个维度求最小值：

```python
torch.min(a, dim=1)
```

返回两个东西：

```text
values=tensor([1, 2])
indices=tensor([0, 1])
```

也就是每一行的最小值，以及最小值的位置。

`torch.max` 完全同理。

所以可以记成：

|写法|含义|
|---|---|
|`torch.min(a, b)`|`a` 和 `b` 逐元素取最小|
|`torch.max(a, b)`|`a` 和 `b` 逐元素取最大|
|`torch.min(a)`|`a` 全局最小值|
|`torch.max(a)`|`a` 全局最大值|
|`torch.min(a, dim=k)`|沿第 `k` 维取最小值，同时返回 index|
|`torch.max(a, dim=k)`|沿第 `k` 维取最大值，同时返回 index|

如果你看到类似 PPO 中的 `torch.min(surr1, surr2)`，它就是对两个 loss tensor **逐样本、逐元素选择更小的那个值**，并不是对整个 tensor 求一个最小值。