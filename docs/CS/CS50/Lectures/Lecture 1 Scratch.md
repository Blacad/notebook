- 字符表示
	- ASCII 码
		- 一开始是 seven bits 后来拓展到 eight bits 正好 one byte 来编码 --- 也是定长
		- 用此方法表示 一个英文字符用 one byte 一个汉字用 two bytes
	- Unicode 定长编码
		- 世界上所有的语言包含的符号，都在unicode里面有唯一的编码。Uicode采用两个字节的编码方式，把世界上所有的语言的文字字符都容纳了进来
		- 用此方法表示 英文和汉字都用 two bytes
	- UTF-8 编码
		- UTF，全称Unicode Transfer Format，统一码传输格式。它是一种针对Unicode的一种可变长度的字符编码方案
		- 使用1~4个字节对所有的字符进行编码
		- 对于ASCIl码的那些字符采用一个字节，从而保证与ASCIl的完全兼容
		- 对于拉丁文、希伯来文等字母采用2个字节进行编码；对于中日韩、东南亚等文字，采用3个字节进行编码
	- 自Unicode之后，字符表示更多东西 比如 emoji等，而因为设备厂商的不同 相同字符表示的相同含义的 emoji 可能看上去不同

- 表示颜色
	- 一个原色采用 one byte 来表示，总共三原色，即每个像素点使用three byte 来记




- Scratch 有点意思可以用来做小动画或者小游戏挺好玩的