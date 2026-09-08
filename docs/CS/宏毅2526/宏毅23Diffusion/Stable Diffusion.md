- Stable Diffusion Framework
	- Text Encoder
	- Generation Model
	- Decoder
	- [Framework](StableDiffusion.pdf#page=2)
	- [Detailed Model](StableDiffusion.pdf#page=3)

- [DALL-E series](StableDiffusion.pdf#page=4)
- [Imagen](StableDiffusion.pdf#page=5)


- Text Encoder
	- Google 使用 T5 做 Text Encoder
	- [FID](StableDiffusion.pdf#page=8) --- 两个高斯分布之间的弗氏距离
	- [CLIP-score](StableDiffusion.pdf#page=9)
		- 文本Encoder 与 对应图片Encoder后 输出的向量相近分数越高


- Decoder
	- 中间产物是小图，训练资料就是 (小图, 大图) 
		- [如图](StableDiffusion.pdf#page=11)
	- 中间产物是 Latent Representation
		- 需要训练 Auto-encoder 去完成
		- [如图](StableDiffusion.pdf#page=12)

- Generation Model
	- 与原先 Diffusion Model 一致
	- [如图](StableDiffusion.pdf#page=14-16)