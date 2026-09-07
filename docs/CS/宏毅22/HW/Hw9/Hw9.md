- 说明
	- 本次作业的目标是 解释模型的表现
	- 基于 作业3(cnn) 和 作业7(bert) 的模型，运用第9堂课的知识做各种可解释性分析
	- 这次作业很特殊，它不是训练模型去解决问题，而是解释模型本身，因此作业设计了30个问题，需要你运行相应代码并做出回答
	- 具体的问题和解答，可以看 hw9 的 slide 和 hw9 的jupyter
	- 这里记录实践中用到的方法 即具体如何实践各种可解释性分析
	- 我做了很多尝试依然无法找到代码中指定模型下载位置，因此本次作业主要还是学习代码实践，无法实操

- Lime (Q1-Q4)
	- Lime 的本质是用简单线性模型去模拟黑盒模型的部分输出(区域存在向量->概率)，进而分析哪些输入重要
	- 具体来讲 [Lime的拟合方法](Lime的拟合方法.md)
	- `from lime import lime_image`  ---- 有 lime 包
	- `explainer = lime_image.LimeImageExplainer()` --- 得到可解释器
	- `from skimage.segmentation import slic` --- 导入分割图片方法
	- `slic(input, n_segments=200, compactness=1, sigma=1, start_label=1)` --- 分割图片
	- `explaination = explainer.explain_instance(image=x,classifier_fn=predict,segmentation_fn=segmentation)` --- 给 可解释器 输入、预测方法 以及 分割方法
		- 将输入图片分割为多个区域，lime 会对各区域进行遮蔽处理形成多个扰动样本，然后用线性模型拟合不同样本的预测结果，进而分析出图片的各区域对结果的重要性
	- `lime_img, mask = explaination.get_image_and_mask(label=label.item(),positive_only=False,hide_rest=False,num_features=11,min_weight=0.05)` --- 得到分析后的图片和mask
		- `label=label.item()`：指定你想解释的类别。比如你的分类器有 5 类，`label.item() == 2`，那就是取“类别 2”的 LIME 解释，也就是读取类似 `explanation.local_exp[2]` 的结果
		- `positive_only=False`：不仅保留对类别 2 有正向贡献的 superpixel，也保留负向贡献的 superpixel。也就是说，权重 `+0.3` 和 `-0.2` 的区域都可能被选中
		- `hide_rest=False`：不隐藏没有被选中的区域，所以 `lime_img` 仍然基本保留整张原图。如果设成 `True`，则主要只显示被选中的重要区域，其余区域会被隐藏
		- `num_features=11`：最多选择 11 个重要 superpixel
		- `min_weight=0.05`：只有 LIME 权重绝对值足够大的区域才考虑


- Saliency Map(Q5-9)
	- Saliency Map 是损失对输入求导，进而分析哪些输入重要
	- [Saliency Map](Saliency_Map.md)

- Smooth Grad(Q10-13)
	- 对同一张原图加入多次随机噪声，每次计算一张 Saliency Map，最后把这些 Saliency Map 平均，从而降低普通 Saliency Map 中的噪声
	- [Smooth Grad](Smooth_Grad.md)


- Filter Explanation(Q14-17)
	- filter 究竟识别了什么，由两个方法 filter activation 和 filter visualization
		- filter activation --- 挑几张图片出来，看看图片中哪些位置会activate该filter
		- filter visualization --- 找出怎样的图片可以最大程度 activate 该 filter
	- pytorch 提供了 hook 方法去得到每层的输出
	- [Filter](Filter.md)



- Integrated Gradients(Q18-20)
	- Saliency 看“当前点的梯度”，SmoothGrad 看“原图附近的平均梯度”，Integrated Gradients 看“从 baseline 走到原图整个路径上的累计梯度”
	- [IG](IG.md)


- Attention Visualization(Q21-24)
	- 呈现各层 attention layer 的 Attention Map(attention score)
	- 这里使用网址看


- Embedding Visualization(Q25-27)
	- 把 BERT 每一层中每个 token 的 768 维 hidden state 用 PCA 降到二维，然后画出来，观察随着 BERT 层数加深，question、context、answer 对应 token 的表示如何变化
	- [Embedding Visualization](./Embedding.md)

- Embedding Analysis(Q28-30)
	- 它从 BERT 第 12 层提取同一个目标字在不同句子中的 768 维 contextual embeddings，然后计算这些向量之间的两两欧氏距离并画成热力图，以观察上下文是否导致 BERT 对同一个字形成不同的语义表示


- Local 可解释性分析
	- LIME
	- Saliency Map
	- Smooth Grad
	- IG
	- AV
	- EV/EA

- Global 可解释性分析
	- Filter Explanation