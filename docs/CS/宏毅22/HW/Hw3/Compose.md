`transforms.Compose([...])` 中常见的数据增强主要包括随机翻转、随机裁剪、随机旋转、颜色扰动等。

一个常见的训练集写法：

```python
from torchvision import transforms

train_tfm = transforms.Compose([
    transforms.Resize((144, 144)),

    # 随机裁剪为 128×128
    transforms.RandomCrop((128, 128)),

    # 50% 概率水平翻转
    transforms.RandomHorizontalFlip(p=0.5),

    # 随机旋转
    transforms.RandomRotation(degrees=15),

    # 随机调整亮度、对比度、饱和度和色调
    transforms.ColorJitter(
        brightness=0.2,
        contrast=0.2,
        saturation=0.2,
        hue=0.1
    ),

    transforms.ToTensor(),

    # 随机擦除必须放在 ToTensor 之后
    transforms.RandomErasing(p=0.2),
])
```

常用方法可以这样分类。

### 几何变换

```python
transforms.RandomHorizontalFlip(p=0.5)
```

随机水平翻转，适用于大多数自然图像。

```python
transforms.RandomVerticalFlip(p=0.5)
```

随机垂直翻转。对于人物、动物、文字等图像通常不适合，因为上下翻转可能改变语义。

```python
transforms.RandomRotation(15)
```

在 ([-15^\circ,15^\circ]) 范围内随机旋转。

```python
transforms.RandomCrop(128)
```

随机裁剪为 (128\times128)。

```python
transforms.RandomResizedCrop(
    size=128,
    scale=(0.8, 1.0)
)
```

随机裁剪一块区域，再缩放到 (128\times128)。这是图像分类中非常常用的增强方式。

```python
transforms.RandomAffine(
    degrees=10,
    translate=(0.1, 0.1),
    scale=(0.9, 1.1)
)
```

随机旋转、平移和缩放。

### 颜色变换

```python
transforms.ColorJitter(
    brightness=0.2,
    contrast=0.2,
    saturation=0.2,
    hue=0.1
)
```

随机调整图像颜色属性。

```python
transforms.RandomGrayscale(p=0.1)
```

以一定概率转成灰度图。

```python
transforms.GaussianBlur(kernel_size=3)
```

随机高斯模糊。

### Tensor 级增强

这些通常放在 `ToTensor()` 之后：

```python
transforms.RandomErasing(
    p=0.25,
    scale=(0.02, 0.2)
)
```

随机遮挡图像中的一块区域，提高模型对遮挡的鲁棒性。

```python
transforms.Normalize(mean, std)
```

严格来说它不是数据增强，而是归一化。例如 ImageNet：

```python
transforms.Normalize(
    mean=[0.485, 0.456, 0.406],
    std=[0.229, 0.224, 0.225]
)
```

### 比较推荐的基础配置

对于普通自然图像分类，可以写成：

```python
train_tfm = transforms.Compose([
    transforms.RandomResizedCrop(
        128,
        scale=(0.8, 1.0)
    ),
    transforms.RandomHorizontalFlip(),
    transforms.RandomRotation(10),
    transforms.ColorJitter(
        brightness=0.2,
        contrast=0.2,
        saturation=0.2
    ),
    transforms.ToTensor(),
    transforms.Normalize(
        mean=[0.485, 0.456, 0.406],
        std=[0.229, 0.224, 0.225]
    ),
])
```

验证集和测试集一般不要使用随机增强：

```python
test_tfm = transforms.Compose([
    transforms.Resize((128, 128)),
    transforms.ToTensor(),
    transforms.Normalize(
        mean=[0.485, 0.456, 0.406],
        std=[0.229, 0.224, 0.225]
    ),
])
```

你注释中说的测试阶段增强叫 **Test-Time Augmentation，TTA**。它会对一张测试图片生成多个版本，例如原图、水平翻转图、不同裁剪图，然后把多个预测结果取平均：

$$
p(y\mid x)
=
\frac{1}{K}  
\sum_{k=1}^{K}p(y\mid T_k(x))  
$$

但普通测试流程通常仍使用确定性的 `test_tfm`，只有需要进一步提升性能时才考虑 TTA。