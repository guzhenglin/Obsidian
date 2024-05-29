# Synopsys安装
by guzhenglin
## 环境
- ubuntu 20.04 LTS
- gcc、g++ 9.4.0
- glibc 2.31
- shell环境切换为bash
```bash
sudo rm /bin/sh
sudo ln -s /bin/bash /bin/sh
或
sudo dpkg-reconfigure dash
选否
```
## 准备
- 建立安装目录
```bash
su
mkdir /home/synopsys
chmod -R 777 /home/synopsys
mkdir /usr/tmp
chmod -R 777 /usr/tmp
touch /usr/tmp/.flexlm
```
- 安装软件和依赖
```bash
sudo apt install lsb lsb-core lsb-base dos2unix firewalld
```
## 安装
1. 进入synopsysinstaller
2. 
```bash
chmod 777 SynopsysInstaller_v5.0.run
./SynopsysInstaller_v5.0.run
./setup.sh
```
3. 选择软件所在文件夹，安装目录 */home/synopsys*
4. 重复23，安装所有软件
## 激活
### 本机信息
```bash
hostname //得到HOST name
ifconfig  //sudo apt install net-tools安装，得到ipv6地址
```
### license
- window下运行license-2029内的scl_keygen.exe
- HOST ID两项都填入去掉冒号的ipv6地址，12位十六进制数
- 更改HOST name
- Port填入端口，如27000、28000等
- generate
- copy
- 写入文件，命名为license.dat
- 第二行后空格写入/home/synopsys/scl/2018.06/linux64/bin/snpslmd
- 复制到ubuntu系统
- ```dos2unix Synopsys.dat
- 用管理员权限创建/usr/local/flexlm/licenses文件夹，放入Synopsys.dat
- 在/home/synopsys/scl/2018.06/admin/license放入Synopsys.dat
- 在~/.bashrc文件末尾添加bash.txt内容(LM_LICENSE_FILE改hostname和port)
- source .bashrc
- 开放端口
```bash
firewall-cmd --zone=public --add-port=27000/tcp --permanent //改为license端口
firewall-cmd --reload
```
- lic开启license服务，看到
```bash
Running on Hypervisor: Not determined - treat as Physical
```
启动软件
licd关闭许可证服务
lick强行kill服务
- 每次开机执行lic
## 错误处理
### vcs
- undefined reference
vcs命令后加-LDFLAGS -Wl,--no-as-needed
```bash
vcs -full64 -LDFLAGS _WI,--no-as-needed_
```
### verdi
- 缺少**libpng12.so.o**文件
```bash
sudo add-apt-repository ppa:linuxuprising/libpng12
sudo apt update
sudo apt install libpng12-0
```
### spyglass
- 打开/home/synopsys/spyglass/SPYGLASS2017.12-SP2/SPYGLASS_HOME/lib/SpyGlass/standard-environment.sh
找到如下内容，补全linux5内容
```bash
Linux-3*)
            if [ X"$switch32bit" = "Xyes" ]; then
                echo "Linux2"
            elif  [ X"$switch64bit" = "Xyes" ]; then
                PROCNAME=`uname -p`
                if [ "X$PROCNAME" = "Xunknown" ]; then
                    PROCNAME=`uname -m`
                fi
                if [ X"$PROCNAME" = "Xx86_64" ]; then
                    echo "Linux4"
                else
                    echo "Linux2"
                fi
            else
                PROCNAME=`uname -p`
                if [ "X$PROCNAME" = "Xunknown" ]; then
                    PROCNAME=`uname -m`
                fi
                if [ X"$PROCNAME" = "Xx86_64" ]; then
                    if [ X"$defExeOn64Bit" = "X32" ]; then
                        echo "Linux2"
                    else
                        echo "Linux4"
                    fi
                else
                    echo "Linux2"
                fi
            fi
            ;;
Linux-4*)
            if [ X"$switch32bit" = "Xyes" ]; then
                echo "Linux2"
            elif  [ X"$switch64bit" = "Xyes" ]; then
                PROCNAME=`uname -p`
                if [ "X$PROCNAME" = "Xunknown" ]; then
                    PROCNAME=`uname -m`
                fi
                if [ X"$PROCNAME" = "Xx86_64" ]; then
                    echo "Linux4"
                else
                    echo "Linux2"
                fi
            else
                PROCNAME=`uname -p`
                if [ "X$PROCNAME" = "Xunknown" ]; then
                    PROCNAME=`uname -m`
                fi
                if [ X"$PROCNAME" = "Xx86_64" ]; then
                    if [ X"$defExeOn64Bit" = "X32" ]; then
                        echo "Linux2"
                    else
                        echo "Linux4"
                    fi
                else
                    echo "Linux2"
                fi
            fi
            ;;
Linux-5*)
            if [ X"$switch32bit" = "Xyes" ]; then
                echo "Linux2"
            elif  [ X"$switch64bit" = "Xyes" ]; then
                PROCNAME=`uname -p`
                if [ "X$PROCNAME" = "Xunknown" ]; then
                    PROCNAME=`uname -m`
                fi
                if [ X"$PROCNAME" = "Xx86_64" ]; then
                    echo "Linux4"
                else
                    echo "Linux2"
                fi
            else
                PROCNAME=`uname -p`
                if [ "X$PROCNAME" = "Xunknown" ]; then
                    PROCNAME=`uname -m`
                fi
                if [ X"$PROCNAME" = "Xx86_64" ]; then
                    if [ X"$defExeOn64Bit" = "X32" ]; then
                        echo "Linux2"
                    else
                        echo "Linux4"
                    fi
                else
                    echo "Linux2"
                fi
            fi
            ;;
    *)             echo UNKNOWN ;;
    esac
}
```
- 打开/home/synopsys/spyglass/SPYGLASS2017.12-SP2/perl/bin/perl
找到如下内容并补全linux5
```bash
    Linux-3*)      ##now we have Linux-64 bit also
            PROCNAME=`uname -p`
            if [ "X$PROCNAME" = "Xunknown" ]; then
               PROCNAME=`uname -m`
            fi
            if [ X"$PROCNAME" = "Xx86_64" ]; then
                       species=Linux4
                    else
                       species=Linux2
                    fi
              ;;
     Linux-4*)      ##now we have Linux-64 bit also
            PROCNAME=`uname -p`
            if [ "X$PROCNAME" = "Xunknown" ]; then
               PROCNAME=`uname -m`
            fi
            if [ X"$PROCNAME" = "Xx86_64" ]; then
                       species=Linux4
                    else
                       species=Linux2
                    fi
              ;;
     Linux-5*)      ##now we have Linux-64 bit also
            PROCNAME=`uname -p`
            if [ "X$PROCNAME" = "Xunknown" ]; then
               PROCNAME=`uname -m`
            fi
            if [ X"$PROCNAME" = "Xx86_64" ]; then
                       species=Linux4
                    else
                       species=Linux2
                    fi
              ;;

    *)             echo "ERROR(perl): Unknown platform: $PLAT" 1>&2; exit 1;;
esac
```
### PrimeTime
- Error: Library Compiler executable path is not set
1. 执行/home/synopsys/pts/O-2018.06-SP1/admin/install/syn/bin/install_pt
2. 第一次输入地址为 .(当前地址)
3. 第二次输入地址为lc(Library Compiler)安装地址
### wv(WaveViewer)
- /lib64/libm.so.6: version 'GLIBC_2.29' not found
1. 在lib文件夹内找到libm.so.6
2. 复制到/home/synopsys/wv/N-2017.12-SP2/platforms/linux64/lib文件夹内，代替原来的文件
### 其他动态库报错（需要文件在lib文件夹内）
#### libtiff.so.3
```bash
sudo ln -s /usr/lib/x86_64-linux-gnu/libtiff.so.5 /usr/lib/x86_64-linux-gnu/libtiff.so.3
```
#### libmng.so.1
```bash
sudo apt-get install libmng2  
cd /usr/lib/x86_64-linux-gnu  
sudo ln -s libmng.so.2 libmng.so.1
```
#### libz.so.1: version 'ZLIB_1.2.9' not found
1. 官网<a href="https://www.zlib.net/fossils/" title="zlib">zlib</a>下载zlib1.2.9
2. 编译
```bash
tar -xvf zlib-1.2.9.tar.gz
cd zlib-1.2.9
su
./configure
make
make install
```
3. 链接
```bash
ln -s -f /usr/local/lib/libz.so.1.2.9  /lib/x86-linux-gnu/libz.so.1
```
#### /lib64/libfontconfig.so.1: undefined symbol: FT_Done_MM_Var
1. 找到lib文件夹内的libfontconfig.so.1.11.1
2. 以管理员身份复制到/usr/lib/x86_64-linux-gnu
3. 
 ```bash
su
cd /usr/lib/x86_64-linux-gnu
mv libfontconfig.so.1 libfontconfig.so.1.bak //备份
ln -s libfontconfig.so.1.11.1 libfontconfig.so.1
```
## TO DO
1. PrimeTime gui不能启动
2. SiliconSmart启动失败
