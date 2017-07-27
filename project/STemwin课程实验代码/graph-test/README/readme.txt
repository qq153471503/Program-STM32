	
	STM32F10X_HD,USE_STDPERIPH_DRIVER	大容量

	STM32F10X_MD,USE_STDPERIPH_DRIVER	中容量

	STM32F10X_LD,USE_STDPERIPH_DRIVER	小容量


小容量是指闪存存储器容量在16K至32K字节之间。
中容量是指闪存存储器容量在64K至128K字节之间。
大容量是指闪存存储器容量在256K至512K字节之间。
 
STM32F10X_LD_VL 对应于 LowDensity ValueLine，即小容量的STM32F100
STM32F10X_MD_VL 对应于 MediumDensity ValueLine，即中等容量的STM32F100
STM32F10X_HD_VL 对应于 HighDensity ValueLine，即大容量的STM32F100
STM32F10X_CL 对应于 ConnectivityLine，即STM32F105、STM32F107

其实更常用的是STM32F103：
STM32F10X_LD 对应于 LowDensity ，即小容量的STM32F103、101
STM32F10X_MD 对应于 MediumDensity ，即中等容量的STM32F103、101
STM32F10X_HD 对应于 HighDensity ，即大容量的STM32F103、101




1	STM32	STM32代表ARM Cortex-M内核的32位微控制器。
2	F	F代表芯片子系列。
3	103	103代表增强型系列。
4	R	R这一项代表引脚数，其中T代表36脚，C代表48脚，R代表64脚，V代表100脚，Z代表144脚，I代表176脚。
5	B	B这一项代表内嵌Flash容量，其中6代表32K字节Flash，8代表64K字节Flash，B代表128K字节Flash，C代表256K字节Flash，D代表384K字		节Flash，E代表512K字节Flash，G代表1M字节Flash。
6	T	T这一项代表封装，其中H代表BGA封装，T代表LQFP封装，U代表VFQFPN封装。
7	6	6这一项代表工作温度范围，其中6代表-40——85℃，7代表-40——105℃。



    这是ST提供的3个启动文件，分别适用于不同容量的STM32芯片。
    这三个文件为： 
        startup_stm32f10x_ld.s
        startup_stm32f10x_md.s
        startup_stm32f10x_hd.s

    其中，ld.s适用于小容量 产品；
          md.s适用于中等容量产品；
	  hd适用于大容量产品；

    这里的容量是指FLASH的大小.判断方法如下：
       小容量：FLASH≤32K
       中容量：64K≤FLASH≤128K
       大容量：256K≤FLASH
    所以，请大家根据自己所用的STM32芯片选择不同的.s文件。


