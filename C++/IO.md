**输入** ：std::cin >> value;
Ctrl Z（end-of-file）  结束输入
**输出** ：std::cout >> value >> std::endl;
std::endl用于换行
cout是重载运算符，对不同参数有调整行为。
cin使用空格、TAB、换行符确定字符串结束。
cin.getline()用换行符确定结尾，并舍弃换行符。cin.get().get()效果相同，返回对象仍为cin。
short最少16位，int至少大于short，long至少32位，long long至少64位
类型的长度依赖实现，sizeof()返回实际字节
C++11使大括号初始化器用于任何类型
十进制第一位1~9，八进制第一位0，十六进制第一位0x
cout进制转换仅限int
flout类型用f/F后缀，double类型用l/L后缀
auto声明：根据初始值推断变量类型
让编译器计算数组元素的个数是很糟糕的做法
strcpy将字符串复制到字符数组，strcat将字符串追加到字符数组
单行输入字符串getline(cin,str)
原始字符串R"+* (string)+* "
cout.fixed浮点数以固定点和小数点表示，避免科学计数法
cout.precision(n)保留n位有效数字
cout.setf(ios_base::showpoint)显示小数点后末尾的零
### 文件输入输出
头文件fstream
写入类型 ofstream File
读取类型 ifstream File
File.is_open()检查是否正确打开
File.good()检查文件是否正确打开，读写是否正确
