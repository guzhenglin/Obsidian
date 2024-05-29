### 模块定义
- InstrMen: 指令存储器
- nextPC: 下地址
- InstrParse: 指令解析
- InstrToImm: 立即数扩展器
- Control: 控制器
- BranchControl: 跳转控制

### 控制信号定义
ExtOp[2:0]：控制立即数扩展器的输出数据，宽度为3位。
RegWr：寄存器堆写使能信号，高电平有效。
ALUASsrc：选择ALU输入端A的来源。
ALUBSrc[1:0]：选择ALU输入端B的来源，宽度为2位。
ALUctr[3:0]：选择ALU执行的操作，宽度为4位。
Branch[2:0]：用于生成分支控制信号，宽度为3位。
MemtoReg：选择寄存器rd写回数据来源，ALU输出还是数据存储器输出。
MemWr：数据存储器写使能信号。
MemOp[2:0]：用于数据存储器读写数据字节长度（字节、半字、字）以及在读取时是否进行符号位扩展，宽度为3位。
opcode[6:0]：操作码
funct3[2:0]：功能码
funct7[6:0]：功能码
![[单周期CPU_image_1.png]]
![[单周期CPU_image_2.png]]
