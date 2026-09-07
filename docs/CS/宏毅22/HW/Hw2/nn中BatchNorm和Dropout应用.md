
## BatchNorm

**核心用途：**  
对中间特征进行标准化，稳定训练、加快收敛，并缓解梯度不稳定。

**常见位置：**

```text
Linear / Conv → BatchNorm → Activation
```

**适用情形：**

- CNN、MLP 等常规模型
    
- batch size 较稳定且不太小时
    
- 希望提升训练稳定性和收敛速度时
    

**注意：**

- 训练时使用当前 batch 的均值和方差
    
- 推理时使用训练期间累计的统计量
    
- 小 batch 下效果可能不稳定
    
- Transformer 中通常更常用 LayerNorm
    

**参考代码：**

```python
import torch
import torch.nn as nn

class MLPWithBN(nn.Module):
    def __init__(self, input_dim, hidden_dim, output_dim):
        super().__init__()

        self.net = nn.Sequential(
            nn.Linear(input_dim, hidden_dim),
            nn.BatchNorm1d(hidden_dim),
            nn.ReLU(),
            nn.Linear(hidden_dim, output_dim)
        )

    def forward(self, x):
        return self.net(x)
```

CNN 中常用：

```python
self.block = nn.Sequential(
    nn.Conv2d(3, 32, kernel_size=3, padding=1),
    nn.BatchNorm2d(32),
    nn.ReLU()
)
```

---

## Dropout

**核心用途：**  
训练时随机将部分特征置零，减少神经元之间的过度依赖，抑制过拟合。

**常见位置：**

```text
Linear / Conv → Activation → Dropout
```

**适用情形：**

- 训练集效果明显好于验证集
    
- 模型容量较大、数据较少
    
- 全连接层较多
    
- 希望增强模型泛化能力
    

**注意：**

- 只在训练阶段生效
    
- 推理阶段自动关闭
    
- `p` 表示丢弃概率，如 `Dropout(0.5)`
    
- `p` 过大可能导致欠拟合
    

**参考代码：**

```python
class MLPWithDropout(nn.Module):
    def __init__(self, input_dim, hidden_dim, output_dim):
        super().__init__()

        self.net = nn.Sequential(
            nn.Linear(input_dim, hidden_dim),
            nn.ReLU(),
            nn.Dropout(p=0.5),
            nn.Linear(hidden_dim, output_dim)
        )

    def forward(self, x):
        return self.net(x)
```

---

## BatchNorm 与 Dropout 组合

常见顺序：

```text
Linear / Conv → BatchNorm → Activation → Dropout
```

参考代码：

```python
class Network(nn.Module):
    def __init__(self, input_dim, hidden_dim, output_dim):
        super().__init__()

        self.net = nn.Sequential(
            nn.Linear(input_dim, hidden_dim),
            nn.BatchNorm1d(hidden_dim),
            nn.ReLU(),
            nn.Dropout(p=0.3),

            nn.Linear(hidden_dim, output_dim)
        )

    def forward(self, x):
        return self.net(x)
```

训练和测试时：

```python
model.train()  # BatchNorm 使用当前 batch；Dropout 生效

output = model(x)

model.eval()   # BatchNorm 使用运行统计量；Dropout 关闭
with torch.no_grad():
    output = model(x)
```

## 核心区别

| 方法        | 核心作用        | 训练阶段         | 推理阶段    |
| --------- | ----------- | ------------ | ------- |
| BatchNorm | 稳定特征分布、加快训练 | 使用 batch 统计量 | 使用运行统计量 |
| Dropout   | 抑制过拟合       | 随机置零         | 自动关闭    |