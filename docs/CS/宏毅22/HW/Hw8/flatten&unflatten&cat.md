
- `flatten` 把多个维度合并成一个维度
- `unflatten` 把某个维度拆分成多个维度

- `tensor.flatten(start_dim, end_dim)`
	- 可以指定合并从哪个维度开始，到哪个维度结束
	- 默认是从0维到最后1维

- `nn.Flatten(start_dim, end_dim)`
	- 可以指定合并从哪个维度开始，到哪个维度结束
	- 默认是从第1维到最后1维，不会合并 Batch维度


- `tensor.unflatten(dim, sizes)`
	- `dim` 指定要被拆分的维度
	- `sizes` 指定该维度拆分后的多个维度大小
	- 比如 `(2,12).unflatten(1,(3,4))` -> `(2,3,4)`
	- 也可以用 -1 的自动推断(这个也是常使用的技巧)
		- `(2,12).unflatten(1,(3,-1,2))` -> `(2,3,2,2)`


- `tensor.cat(tensors, dim)`
	- `tensors` 用于合并的 tensor 可迭代对象
	- `dim` 合并的维度