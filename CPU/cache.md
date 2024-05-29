主存 256KB 32位  $2^{5}X2^{16}$
cache 4KB
4路组相联
$2^{3}$组x$2^{2}$行/组x128字/行
随机替换
回写
每个cache行128字节，1个tag、valid、dirty
```verilog
cache[1033]=valid;
cache[1032]=dirty;        //为1时写回主存
cache[1031:1024]=tag;
cache[1023:0]=128Byte;

addr[15:8]=tag_addr;
addr[7:5]=index_addr;
addr[4:0]=offset_addr;
```
指令支持4字节整体读写，数据支持1、2、4读写
cache32位地址：
unused_addr：高位，恒为0                                       **14**
tag_addr：标签块，32位地址在cache中对应的地址段   **8**
index_addr：分组索引，由分组数决定                          **3**
offset_addr：块内地址，每行word数决定                     **5**
word_addr\[1:0\]：字节地址，word中的第几字节            **2**
主存16位地址：
tag_addr：   **8**
index_addr：**3**
offset_addr：**5**
COMPARE：根据地址比较读写请求==是否命中==，根据Cache的映射方式，比较请求地址数据是否在cache中。

如果当前cache块的valid有效，请求地址段的Tag部分和Cache块的Tag相同；
则==命中==hit信号有效，无论读写数据请求都返回Cache 就绪信号。
如果是写入请求，则当前Cache块的脏位Dirty信号有效。

如果==未命中==，则需要替换cache块内容，如果脏位dirty信号有效，则进入写回状态，否则进入载入状态。
![[Pasted image 20231109114955.png]]
xdc
tag: switch[15 : 8]
index: switch[7 : 5]
data_in: switch[3 : 0]