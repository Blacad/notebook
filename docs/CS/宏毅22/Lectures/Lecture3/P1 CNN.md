

- image 本身是三维的(3,100,100) ---- (channels, width, height)
	- channels 是颜色、width 和 height 是 image 的宽高
	- 在常规FNN中我们直接将其[拉直算](../../Slides/Lecture3/cnn_v4P1.pdf#page=3-4)，但是这会导致 input 输入维度大涨，进而使得模型复杂容易overfit


- [receptive field 感受野](../../Slides/Lecture3/cnn_v4P1.pdf#page=7-10)

- [相同特征出现的位置会不同](../../Slides/Lecture3/cnn_v4P1.pdf#page=11-15)
	- 共享参数 Filter

- CNN --- 专门为 image 处理设计 
 
- [CNN的具体设计](../../Slides/Lecture3/cnn_v4P1.pdf#page=17-25)
	- 在CNN中，如果输入的channel是3，filter的空间大小是 (3,3)，那么filter完整尺寸应该是 (channel,3,3)，同时一个filter只会产生一个channel，比如 filter 1 的完整尺寸是 (channel,3,3) 有channel个(3,3)的权重切片针对输入的不同通道做卷积，最后会把所有通道的结果相加形成最终的一个channel
	- [多层CNN](../../Slides/Lecture3/cnn_v4P1.pdf#page=22-23)
	- [CNN完整设计](../../Slides/Lecture3/cnn_v4P1.pdf#page=30)

- [pooling](../../Slides/Lecture3/cnn_v4P1.pdf#page=27-28) --- 减少运算量，pooling可有可无


- 应用
	- [下围棋](../../Slides/Lecture3/cnn_v4P1.pdf#page=31-33)
	- [其它](../../Slides/Lecture3/cnn_v4P1.pdf#page=34)

- [CNN的问题](../../Slides/Lecture3/cnn_v4P1.pdf#page=35)
