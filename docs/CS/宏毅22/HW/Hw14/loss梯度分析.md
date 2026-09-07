
- `y_i=log_softmax(x_i,x)`
	- 记 $p_i = softmax(x_i,x)$
	- $\partial {y_i} / \partial x_i = 1-p_i$
	- $\partial {y_i} / \partial x_j = -p_j$
	- 你如果只要 p 就用 softmax，如果要 logp 就用 log_softmax



- `y=sigmoid(x)`
	- $\partial y / \partial x = (1-y)*y$

