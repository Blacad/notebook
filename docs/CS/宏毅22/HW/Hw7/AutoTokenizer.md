- 对于Tokenizer
	- transformers 通常会实现两类，一类是慢速由纯python实现，另一类是快速基于Rust实现
	- 一般默认会返回 Fast 的实现

- 无论是 Fast Tokenizer 还是 Slow Tokenizer，调用 `tokenizer(...)` 返回的都是 `BatchEncoding` 对象；区别在于 `BatchEncoding` 内部是否包含 Rust 的 `Encoding` 对象，因此支持的方法不同

- Fast Tokenizer 返回的 BatchEncoding对象由于有 Rust 的 `Encoding` 对象，支持更多方法，如下
```python
encoding.char_to_token() --- 字符下标 所属的 token 下标
encoding.token_to_chars() --- token下标 所属的 字符(CharSpan对象)
encoding.word_ids()
encoding.word_to_tokens()
encoding.token_to_word()
```

- 当然常规的 `BatchEncoding` 对象，类似于 字典
	- 它通常有如下三个重要变量
		- |`input_ids`|tokenizer 编码后的 token ID|`[101, 2023, 2003, 102]`|
		- |`token_type_ids`|token 属于哪一段（Segment）|`[0,0,0,1,1]`|
		- |`attention_mask`|哪些位置是真实 token（1），哪些是 Padding（0）|`[1,1,1,0,0]`|
		- 有些可能不需要 `token_type_ids`，它是被用作当 segment embedding的，即标明token属于哪一段
	- 同时一般支持方法
		- `.tokens()` 返回编码后的tokens