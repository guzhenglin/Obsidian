### 系统扩容
- 在windows下使用DiskDenius移动EFI分区
- 制作启动盘
- 使用ubuntu
- 打开Gparted
- 调整根目录大小
### 键盘映射
HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Keyboard Layout\\Scancode Map
二进制值
00 00 00 00 00 00 00 00
02 00 00 00 1C 00 3A 00              //00000002：两个功能交换  |  键盘扫描值(小端序)，用001C(Enter)实现003A(Caps_Lock)的功能
3A 00 1C 00 00 00 00 00              //00 00 00 00结尾
