# AD9959 Driver for STM32

## 前言

使用hal库实现的ad9959驱动程序，串行通信采用最简单的双线模式。

测试平台：STM32F407ZGT6，对应的芯片型号可直接使用cubeIDE打开使用，其他的芯片型号也可以使用其中的`ad9959.h`和`ad9959.c`自行配置

驱动路径：
- [ad9959.h](Core/Inc/ad9959.h)
- [ad9959.c](Core/Src/ad9959.c)

## 实现功能

- 在单频模式（Single Tune Mode）下，可以独立调整四个通道的幅值、频率和相位。
- 提供写入和读取寄存器的功能，可用于读取DDS的输出频率和幅值。
- 支持扫频模式。

## 移植使用

在CubeIDE或CubeMX中，需要将所有的必要IO配置为上拉电阻，推挽输出模式。

## IO命名及接线参考

请保证CubeMX的GPIO初始化中，与ad9959连接的IO被正确的设置为以下名称：

- SDIO0 
- SDIO1 
- SDIO2 
- SDIO3 
- PDC   
- RST   
- SCLK  
- CS    
- UPDATE
- PS0   
- PS1   
- PS2   
- PS3   


## TODO

- [x] 扫频模式
- [x] 读取寄存器函数
- [ ] 调制模式
- [ ] 参考时钟模式
