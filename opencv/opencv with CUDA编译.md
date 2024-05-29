安装CUDA ToolKit，选择精简安装
将CUDNN的额外文件解压到C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\vxx.x下（CUDNN对CUDA的版本有要求）
下载OpenCV和opencv_contrib，版本对应
打开Cmake-gui，设置源文件路径为opencv及build路径（自定义）。
配置EXTRA_MODULES路径为opencv_contrib下的modules
第一次configure
勾选的选项
- NONFREE
- 搜索CUDA全部勾选
- 搜索java和python按需勾选
- 取消所有test选项
- 搜索opencv_world选项，勾选后只生成一个库文件
- 搜索install定义安装路径
- CUDA_ARCH_BIN选项保留符合显卡算力的值
再次configure，查看无法下载的组件
解决无法下载组件的问题
- 打开[opencv/opencv_3rdparty: OpenCV - 3rdparty (github.com)](https://github.com/opencv/opencv_3rdparty)
- 下载对应版本的文件，ffmpeg可以选择最新版
- 改名为/opencv/source/.cache中的文件并替代
- 重新configure通过，无报错
- 其中ffmpeg组件的MD5值在cmake文件中改为下载文件的值
generate
open project
编译时选择Release，直接生成可使用的库文件
生成ALL_BUILD（等待数小时）
生成INSTALL
进入visual studio进行配置
测试代码
```c++
#include<opencv2/opencv.hpp>
#include<opencv2/dnn.hpp>
#include<opencv2/cudawarping.hpp>
#include<opencv2/cudafilters.hpp>
#include<opencv2/cudastereo.hpp>
#include<opencv2/cudawarping.hpp>
#include<opencv2/cudaoptflow.hpp>
#include<opencv2/cudafeatures2d.hpp>
#include<opencv2/cudabgsegm.hpp>
#include<opencv2/cudaimgproc.hpp>
#include<iostream>

using namespace cv;
using namespace cv::cuda;
using namespace std;

int main() {
	cuda::printCudaDeviceInfo(cuda::getDevice());
	int count = getCudaEnabledDeviceCount();
	printf("device count:%d\n", count);
	system("pause");
	return 0;
}
#endif
```
输出显卡信息即成功