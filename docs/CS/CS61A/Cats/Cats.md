---
comments: true
---

- [项目说明文档](https://www.learncs.site/docs/curriculum-resource/cs61a/cs61a_zh/project/cats)
- 去除句子或段落中的标点符号
	- 该项目中提供了函数`remove_punctuation`有效去除句子标点符号。
	- 首先`strip`函数去除首尾空白字符
		- 如果不传参数，默认去除前后所有空白字符（如空格、`\n`、`\t` 等）。
		- 如果传入参数 `chars`，会移除字符串两端所有出现在 `chars` 中的字符（**逐个匹配，不是子串匹配**）。
		- s = ">>>hello<<<"
		- print(s.strip("><"))  # 输出: "hello"
	- 接着使用`make_trans`和`translate`函数配合去除标点
		- `str.maketrans('', '', string.punctuation)`
		- 以下是去除元音字母
		- table = str.maketrans("", "", "aeiou")
		- s = "hello world"
		- print(s.translate(table))  # 输出: "hll wrld"
	- 同时使用`make_trans`和`translate`函数也可以进行映射
		- 以下是一个例子
		- table = str.maketrans("abc", "123")
		- s = "a black cat"
		- print(s.translate(table))  # 输出: "1 1l1k 31t"

- `Note：cats.py中的run函数根据运行参数来执行不同的相应逻辑`
- `Note:run_typing_test函数就是CLI的实现`

- GUI的实现 cats_gui.py
	- 和Hog一样的我们用到了 gui_files 提供 GUI的一些资源绘制网页和完成响应请求
	- 通过调用我们实现的底层接口完成各种工作
	- 这不是重点但是这种基础的BS很有趣


- 使用递归实现 =="最少编辑数问题“== Problem 7
	- 此问题是我通过ChatGPT解决的
	- 代码实现 [Cats-1](../Cats-1.md)
	- 本题最重要的思考方式就是 i 和 j 分别记录 typed 和 source 处理的位置，然后依据相应的关系设置Baseline和多种类型和情况
	-  ==`Note:一定要明晰各个函数的作用。明晰多种递归的关联和含义。需要记录和回溯的量需要保存在递归参数中`==


## 第三阶段 多人游戏
---
- 五个部分
	- 你的GUI，负责处理网页浏览器里的文字颜色和显示。
	- 你的`cats_gui.py`，是一个Web服务器，它使用你在`cats.py`里写的代码和你的GUI进行通讯。
	- 你的对手的 `cats_gui.py`。
	- 你的对手的 GUI。
	- CS 61A 多人游戏服务器，它将玩家匹配在一起并传递消息。
- BS的架构
	- 当您键入时，您的 GUI 会将您键入的内容上传到您的 `cats_gui.py` 服务器，该服务器会计算您已取得的进展并返回进度更新。它还会将进度更新上传到多人游戏服务器，以便您对手的 GUI 可以显示它。
	- 同时，您的 GUI 显示始终尝试通过从 `cats_gui.py` 请求进度更新来保持最新，而 `cats_gui.py` 又从多人游戏服务器请求该信息。
	- 每个玩家都有一个 `id` 号，服务器使用该号码来跟踪打字进度。

- 问题 8
	- upload 函数很有意思最后可以把[Cats的CLI和BS架构](Cats的CLI和BS架构.md)梳理一下，看看python的BS设置






