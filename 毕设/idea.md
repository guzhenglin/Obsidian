多块计算阵列自由组合，插寄存器
NOC
### Lenet5
Conv1  66   32x32   5x5x6  ->  28x28x6
Pool1  147      28x28x6  2x2  ->  14x14x6
Conv2   168  14x14x6  5x5x16  ->  10x10x16
Pool2   50    10x10x16  2x2  ->  5x5x16
FC1    1503   5x5x16  120  ->   120
FC2     317    120   84  ->   84
### AlexNet
Conv1: 227x227x3  11x11x96  （4） ->   55x55x96   
大  (55x55/32)=95  (373+96-8)x19=8759
小  (373+32-8)x95x3=113145

Conv2: 27x27x48    5x5x128    （1）x2    ->    27x27x128   x2   
大 (27x27/32)=23   4x4    (210+120)x6x6x2    23760
小  (210+24)x6x23x4x2    258336

Conv3: 13x13x256   3x3x384  (1)  ->  13x13x384
大  (13x13/32)=6  (82+128-8)x32x3x2  38784
小   (82+32-8)x32x12x6  244224

Conv4: 13x13x192   3x3x192    (1)   x2   ->    13x13x192   x2
大  （82+192-8)x24x1x3x2 38304
小     (82+32-8)x24x6x6x2   183168

Conv5: 13x13x192   3x3x128   （2） x2   ->   6x6x128    x2
大   (6x6/32)=2   (82+128-8)x24x2  9696
小   (82+32-8)x24x2x4x2x2  81408

|Layer name|IFMAP Height|IFMAP Width|Filter Height|Filter Width|Channels|Num Filter|Strides|
|Conv1|227|227|11|11|3|96|4|
|Conv2|27|27|5|5|96|256|1|
|Conv3|13|13|3|3|256|384|1|
|Conv4|13|13|3|3|384|384|1|
|Conv5|13|13|3|3|384|256|1|
![[idea_image_1.png]]
### AlexNet_16x16x16
##### conv1
227 227 3   11 11 96  (4) -> 55 55 96
大  3x5  55x55/16=190  (373+40)x38x2=31388
小  (370+8)x190x6=430920
##### conv2
27 27 96    5 5 256   ->   27 27 256
大   4x4   27x27/16=46  (2418+56)x4x12=118752
小   2418x46x16=1179648
##### conv3
13 13 256 3 3 384  ->  13 13 384
大   4x4 13x13/16=11  2370x3x6=42660
小  448127
##### conv4
13 13 192  3 3 384  ->  13 13 384
大  4x4  3522x3x6=63396
小  669311
##### conv5
13 13 384  3 3 256   (2)  ->   6  6  256
大   5x3  6x6/16=3   3538x4=14152
小  3474x16x3=166752