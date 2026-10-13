

- CUDA runtime API & CUDA driver API
	- CUDA runtime API 基于底层CUDA driver API构建
	- CUDA运行时API是使用CUDA在C++中最常用的方式

- CUDA 的必要安装(Linux Ubuntu)
	
	=== "安装的前置"
		- `lspci | grep -i nvidia` 查看硬件是否支持 GPU
			- 直接查PCIe主线，看真实连接的GPU有哪些
		- 查看足量系统信息 `hostnamectl`
		- 查看`gcc --version`
		- 安装 CUDA Toolkit 
			- [网址](https://developer.nvidia.com/cuda-downloads?_gl=1*he3hi6*_gcl_au*MTg1NTAwMTUwMS4xNzkwMDQzMzUwLi0uLS4xNzkwMDQzNDA5LjE5MTkxNzk2NzcuMTc5MDA0MzQwOS4xNzkwMDQ0NDA2)
			- 应该有很多支持的版本
			- Deb 系统集成型，深度集成 `apt` 等包管理工具，相对省心
			- Runfile 完全独立，不依赖任何包管理工具(同时也不会被包管理工具记录)，更加灵活，但是需要自己解决依赖等
			- 一般都是使用 Deb
		
	=== "检查CUDA Toolkit"
		- 使用 `apt` 包管理工具安装的话
			- `apt list --installed | grep -i cuda`
			- `apt list --installed | grep -E 'cuda-toolkit-[0-9]+'`
		- 如果要查看具体的文件位置可以采用以下手段
			- `which nvcc`
			- `readlink -f $(which nvcc)`
		- 切换版本
			- 系统级切换：`sudo ln -sfn /usr/local/cuda-12.x /usr/local/cuda` 
			- 临时切换(环境变量)
			```shell
			cuda() { export CUDA_HOME=/usr/local/cuda-$1 export PATH=$(echo "$PATH" | tr ':' '\n' | grep -v '/usr/local/cuda' | paste -sd ':' -) export LD_LIBRARY_PATH=$(echo "$LD_LIBRARY_PATH" | tr ':' '\n' | grep -v '/usr/local/cuda' | paste -sd ':' -) export PATH=$CUDA_HOME/bin:$PATH export LD_LIBRARY_PATH=$CUDA_HOME/lib64:$LD_LIBRARY_PATH echo "CUDA switched to $1" nvcc --version }
			```
			- 注意这里的目录可能不一致，需要以实际目录为准，Linux默认该位置
		- amd64 编译好的 x86-64 机器码，跟 CPU 架构绑定
		- CUDA Toolkit 包含
			- nvcc 编译器
			- 头文件 .h
			- 运行时和开发库 .so
			- 数学库 如 cuBLAS 等
			- 调试/性能工具 如 compute-sanitizer
			- CUDA headers + static/shared libraries

	=== "关于CUDA版本的补充说明"
		- `nvidia-smi` 会 看到 Driver 版本 + CUDA 版本，这个CUDA版本是你这个驱动最高能支持的CUDA版本，但是由于CUDA 有向后兼容的特性，比如我这个Driver是在CUDA12.8出来之前出的，那么当时最新的就是CUDA12.6，所以我这里显示的CUDA12.6，但是实际上我这个Driver也能支持12.8。最保险的来说，看这里的CUDA版本，这里的CUDA一定是最适配的，但是也可以对应Driver的版本去看更高版本的CUDA
		- `nvcc --version` 会看到你本机安装并指定的 CUDA Toolkit 版本(你可以下载多个版本)，应当低于上述讲到的可支持的最高版本CUDA
		- `torch.version.cuda`  是编译该 pytorch 使用的CUDA Toolkit版本(torch需要编译好的cuda运行时库)，你的驱动支持这个版本的cuda就能正常地跑该torch(不需要本地的cuda toolkit再去编译)

	=== "关于CUDA Driver的下载" 
		- pre-actions
			- `hostnamectl`
			- `uname -r`
		- preparation
			- `apt install linux-headers-$(uname -r)`
			- 也可以直接检查 `ls -ld /lib/modules/$(uname -r)/build` 是否正常(正常就过)
		- 添加 NVIDIA Network Repository --- 添加 NVIDIA 的软件源扩充 apt 软件源
			- `$ wget https://developer.download.nvidia.com/compute/cuda/repos/<distribution>/x86_64/cuda-keyring_1.1-1_all.deb`
			- `dpkg -i cuda-keyring_1.1-1_all.deb`
			- `apt update`
			- `cuda-keyring_1.1-1_all.deb` 是一个用于配置 NVIDIA APT 仓库和签名密钥的安装包
		- Install
			- `sudo apt install nvidia-open`
			- `sudo reboot`

- 通过上述四个步骤 你就成功下载了 CUDA 并可以愉快地使用了
