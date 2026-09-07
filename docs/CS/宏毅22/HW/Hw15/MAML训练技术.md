在 MAML / Meta-Learning 里，通常有两层更新。

假设内层 loss 是：

$$L_{\text{train}}(\theta)$$

先算梯度：

```
grads = torch.autograd.grad(
    loss,
    fast_weights.values(),
    create_graph=True
)
```

得到：

$$
g=\nabla_\theta L_{\text{train}}(\theta)
$$

然后用它更新 fast weights：

$$
\theta' = \theta-\alpha g = \theta-\alpha\nabla_\theta L_{\text{train}}(\theta)
$$

接下来会用更新后的参数 $\theta'$ 计算 query / validation loss：

$$
L_{\text{query}}(\theta')
$$

最终 Meta-Learning 要更新的不是 $\theta'$ 本身，而是原始参数 $\theta$，所以需要计算：

$$\frac{\partial L_{\text{query}}(\theta')} {\partial \theta}$$

但由于：

$$\theta' = \theta-\alpha\nabla_\theta L_{\text{train}}(\theta)$$

因此链式法则：

$$\frac{\partial L_{\text{query}}}{\partial\theta} = \frac{\partial L_{\text{query}}}{\partial\theta'} \frac{\partial\theta'}{\partial\theta}$$

而：

$$\frac{\partial\theta'}{\partial\theta} = I-\alpha \frac{\partial^2 L_{\text{train}}} {\partial\theta^2}$$

你会发现这里出现了：

$$\frac{\partial^2 L}{\partial\theta^2}$$

也就是**二阶梯度 / Hessian**，因此我们必须将求梯度这个过程本身也记录进新的计算图(`creat_graph=True`)
于是这条链是完整可微的：

$$\theta \rightarrow L \rightarrow \nabla_\theta L \rightarrow \theta' \rightarrow L_{\text{query}}$$

如果这里不用 `create_graph=True` 默认不会保留计算图，链路到 `grad` 这里就截断了变成独立的两个计算图：

$$\theta \rightarrow L \rightarrow \nabla_\theta L$$


$$\nabla_\theta L \rightarrow \theta' \rightarrow L_{query}$$

正常的反向传播会依据链式法则，将后续节点传回的梯度与当前算子的局部导数进行组合，从而计算更前面变量的梯度。此时，反向传播过程本身只是用于得到梯度结果，默认不会将“梯度如何由原变量计算得到”这一过程继续构造成新的可微计算图。

因此，普通 backward 得到的一阶梯度通常不能继续参与后续求导，也就无法直接计算该梯度关于原变量的更高阶导数。

当设置 `create_graph=True` 时，PyTorch 会在计算一阶梯度的同时，将反向传播过程中用于生成该梯度的张量运算也记录到新的计算图中，使得所得梯度本身仍然保持对原变量的可微依赖关系。

因此：

- 默认 backward：计算一阶梯度，但不保留梯度计算过程的可微图；
- `create_graph=True`：在计算一阶梯度的同时，为梯度计算过程建立新的计算图，从而支持二阶及更高阶导数的计算。