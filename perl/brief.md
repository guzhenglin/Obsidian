### 进制转换
hex(), oct()：接受字符串
### 特殊字符
chr()：输入
ord()：取得代码点
### undef
defined()：undef返回假，已定义返回真
### 数组
$#返回最后一个索引
### 列表
```perl
(1,2,3)
(1..3)
```
quoted word：qw(value1 value2 value3)
pop push：最右侧元素
shift unshift：最左侧元素
splice：4参数{目标数组，开始位置，操作个数，替换列表}
遍历：默认控制变量$_
```perl
foreach $a (@b){
	print $a;
}
```
reverse：反序
each：返回键值对
### 上下文
scalar：强制转换为标量
### 子程序
```perl
sub name{
	
}
#调用
&name;
```
返回值：最后一次运算的结果
私有变量传参：
```perl
my($m,$n)=@_;
```
持久化私有变量：在子程序中保持
```perl
state $n;
```
子程序签名：
```perl
sub name ($m=1, $n=5){
	;
}
```
### 输入
\<STDIN>，带换行符
chomp()：去掉换行符
```perl
chomp($text=<STDIN>);
```
<>读取用户指定的内容
双钻石操作符<<>>修复管道问题
### 输出
printf数组
```perl
my @items=qw(a b c);
my $format="The items are ".("%10s " x @items);
printf $format,@items;
#or
printf "The items are ".("%10s " x @items), @items;
```
### 文件句柄
建议全大写命名
保留句柄：STDIN, STDOUT, STDERR, DATA, ARGV, ARGVOUT
```perl
open FILE, '<', "filename";
```
select修改默认句柄
### 哈希
```perl
$hash{some_key};
```
keys %hash：键列表
values %hash：值列表
存在：exists $hash{key}
删除：delete $hash{key}
胖箭头赋值等效于逗号=>
### 正则
"/ /"内放匹配字符串
通配符："."匹配单个字符，除换行符<==>"\N"
量词：
?: 前一个字符出现1次或0次
\*: 前一个字符出现无数次或0次
+: 前一个字符至少出现1次
{N}: 前一个字符出现N次
{m,n}: 前一个字符出现m到n次
{m,}: 前一个字符至少 出现m次
( ): 模式分组，括号内字符串作为整体
\\N: 第N组重复   #5.10后引入\\g{N}
g{-N}: 左侧相对位置
|: 择一匹配，或
字符集: \[abcxyz\]匹配任意一个字符 \[a-z\]连字符表示范围
字符集简写: 

|简写|匹配字符|
| : -- : | : ---------- : |
|\\d|十进制数字  |
|\\s|空白字符      |
|\\h|水平空白字符|
|\\v|纵向空白字符|
|\\R|一般化行结尾符|
|\\w|单词字符(命名字符集)|
|\\n|换行符|
大写表示反义，不匹配
\\A: 字符串绝对开头锚位
\\z: 字符串绝对末尾
\\b: 单词首位锚位
### 高级匹配
m+任意成对符号定界
修饰符：正则表达式末尾
/i: 大小写无关
/s: “.” 匹配换行符
/x: 加入辅助空白字符
注释中不能出现定界符
/m: 多行匹配, ^ &表示行首和行尾
=~: 绑定操作符，改变默认匹配 目标
```perl
$name=~m//        #左侧写待匹配变量，右侧写正则表达式
```
捕获：()内字符串，$1,$2表示
捕获变量保存至下一次匹配成功
(?: ): 不捕获括号
(?<LABEL>): 命名捕获，访问$+{LABEL}
