# MeteorBladeEnhancer
一款适用于游戏《流星蝴蝶剑.net》的图像增强补丁

由于丢失了老版本的代码，于是我重写了大部分代码。


### 特性
* 1.在不修改游戏内存的情况下，将 DX6 转换到 DX9。

### 未完成特性
* 1. 插件接口，用于加载开发者自行开发的插件。
* 2. 宽屏插件，用修改游戏内存的方式来达到支持宽屏的效果，以及其它游戏优化。

### 已知问题
* 1. 由于目前部分效果使用软件模拟，所以速度非常慢，详见 ISurface9Wrapper 和 SoftwareSurface9Wrapper。


### 本项目使用到了以下项目的代码：
[DirectX-Wrappers](#https://github.com/elishacloud/DirectX-Wrappers)
[MinHook](#https://github.com/TsudaKageyu/minhook)
[DXGL](#https://www.dxgl.info)