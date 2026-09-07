
- [Pointer Network](../../Slides/Lecture5/S2Pointer.pdf#page=6-7)
	- 机制
		- 用 key 与所有输入算 attention score，然后取最大的输入当输出，如此往复
	- 好处
		- 可以让输出强依赖于输入
	- 将 该机制与原先的decode机制结合
		- [copy mechanism](../../Slides/Lecture5/S2Pointer.pdf#page=8)