
>[!note] 
>这节课主要又是导论性质的，感觉没什么意思就不管了
> 来看看这个吧 [CUDA C++](program/1%20Pre-requisite.md)


- 架构
	- 机架(Rack) 内采用 NVLink 互联，NVLink 可以连接 GPU 与 GPU 也可以连接 GPU 与 CPU，之前 GPU 与 CPU 通信靠 PCIe，但是现在 GPU 与 CPU 相连 已经是 NVLink + PCIe的结合了，两者各司其职前者主要数据通道，后者主要控制通道
	- 机架之间采用 IB 等相连
	- 机架并不是单机，一个机架可以有多机，靠 NVSwitch 调控
	- 当然，有些服务器GPU之间也靠PCIe连就很慢
