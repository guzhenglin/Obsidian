### clocking wizard: 生成不同频率的时钟
##### IP设置 
高电平复位
Enable Clock Monitoring ： 实时监测可以随时检测时钟.

Dynamic Reconfig : 可以动态在IP外修改IP配置.

Safe Clock Starup : 在时钟没稳定之前不输出,好处时钟更加稳定,坏处时间短.

Minimize Power : 对电源的一些优化.

Spread Spectrum : 使能一些时钟

Dynamic Phase Shift : 动态调整相位,在IP外做操作调整.

##### 抖动调整 :

Balznced : 各方面平衡模式.

Minimize Output Jitter : 最少输出抖动.

Maximize input Jitter filtering : 最大输入抖动调整.

power_down : 开关功能

clkfbstopped : 反馈时钟,使用要打开时钟监测

input_clk_stopped : 时钟切换标志

Active High、Active Low : 高电平复位还是低电平复位(H高L低)

### block memory generator: 创建RAM，ROM

nterface Type：RAM接口总线

Memory Type：存储器类型，可配置成Single Port RAM（单端口RAM）、Simple Dual Port RAM（伪双端口RAM）、True Dual Port RAM（真双端口RAM）、Single Port ROM（单端口ROM）和Dual Port ROM（双端口ROM）

ECC Options：Error Correction Capability，纠错能力选项，单端口RAM不支持ECC。

Write Enable：字节写使能选项，勾中后可以单独将数据的某个字节写入RAM中

Algorithm Options：算法选项。可选择Minimum Area（最小面积）、Low Power（低功耗）和Fixed Primitives（固定的原语）

Write Width：端口A写数据位宽，单位Bit

Read Width：端口A读数据位宽，一般和写数据位宽保持一致

Write Depth：写深度，即RAM所能访问的地址范围

Read Depth：读深度，默认和写深度保持一致。

Operating Mode：RAM读写操作模式。共分为三种模式，分别是Write First（写优先模式）、Read First（读优先模式）和No Change（不变模式）。写优先模式指数据先写入RAM中，然后在下一个时钟输出该数据；读优先模式指数据先写入RAM中，同时输出RAM中同地址的上一次数据；不变模式指读写分开操作，不能同时进行读写

Enable Port Type：使能端口类型。Use ENA pin（添加使能端口A信号）；Always Enabled（取消使能信号，端口A一直处于使能状态）

Port A Optional Output Register：端口A输出寄存器选项。其中“Primitives Output Register”默认是选中状态，作用是打开BRAM内部位于输出数据总线之后的输出流水线寄存器，虽然在一般设计中为了改善时序性能会保持此选项的默认勾选状态，但是这会使得BRAM输出的数据延迟一拍。其功能是在输出数据加上寄存器，可以有效改善时序，但读出的数据会落后地址两个周期。

Port A Output Reset Options：RAM复位信号选项
