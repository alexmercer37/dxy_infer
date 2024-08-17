dxy-infer配置
### locate的问题
ubuntu可以利用locate进行目标文件定位，但在使用过程中出现了添加或者删除文件不及时的问题，需要对数据库进行更新，因为locate指令是针对数据库运行的
> https://blog.csdn.net/accumulate_zhang/article/details/77587136
### gcc,g++编译器不支持问题
由于当前环境需要nvcc以及gcc,g++联合编译，系统为24.04，默认安装的gcc,g++版本很高，是13.2,而当前只支持小于12版本的编译器，所以需要对其进行降级处理，cmake版本目前是3.28,不过目前来看没有太大影响
> https://huanghailiang.github.io/2017/07/22/ubuntu-Reduce-the-version-gcc-g++/

> https://blog.csdn.net/CharlieVV/article/details/111242143
### undefined reference to 问题解决
首先怀疑是编译器冲突，所以对nvcc与c++作了分开处理，首先对.cu文件使用nvcc编译器进行编译，生成一个静态链接库，之后通过调用静态链接库编译.cpp文件，没有解决，其次发现没有被链接的函数并不是全部，所以对于一部分函数作了处理，直接在.hpp文件中进行定义，需要用inline函数，解决了一部分问题，但是opencv的函数仍然有相同报错，所以怀疑是opencv的版本问题，于是将版本从4.9.0降到了4.5.1,仍然没有解决问题，最后在查找相关资料时发现可能是cmake的配置问题，opencv的配置和cuda的配置可能不匹配，对cmakelists.txt进行了修改，发现问题解决了
> https://blog.csdn.net/LLY_A_/article/details/120385775

> https://mbd.baidu.com/ug_share/mbox/4a83aa9e65/share?product=smartapp&tk=e3c721c7f5214c2c21bcee5a54cecc3b&share_url=https%3A%2F%2Fyebd1h.smartapps.cn%2Fpages%2Fblog%2Findex%3FblogId%3D139819798%26_swebfr%3D1%26_swebFromHost%3Dbaiduboxapp&domain=mbd.baidu.com
### 安装opencv
降低opencv版本时碰到了问题，首先是cmake进行编译时与anaconda有冲突，导致安装不了，可以暂时将anaconda的安装包删除，可以解决冲突问题，其他出现的问题详见下方网页
> https://blog.csdn.net/KIK9973/article/details/118830187

> https://blog.csdn.net/u010003609/article/details/100087245

> https://blog.csdn.net/CSDNWWC/article/details/129205596
### tensorRT版本问题
目前所用的版本是8.6.1.6,可以正常运行，高版本目前测试10.0以上版本无法正常运行，安装的版本没有报错即可使用。报错如下
> class "nvinfer1::ICudaEngine" has no member "getNbBindings"
> 
> class "nvinfer1::ICudaEngine" has no member "bindingIsInput"
> 
> error: class "nvinfer1::ICudaEngine" has no member "getBindingName"
> 
> error: class "nvinfer1::ICudaEngine" has no member "getBindingDimensions"
### engine文件的生成
直接用tensorRT即可通过.onnx文件生成.engine文件，注意使用的文件目前支持fp16，不支持fp32，本架构支持yolov5,yolov7,yolov8,yolov8-seg，对于yolov5，有成熟的修改后的安装包，可作保底方案，但对于目前的比赛来说有些过时，包括今年比赛的推理环境也需要更新，所以今年可以尝试用此推理环境和yolov8进行项目搭建
> https://blog.csdn.net/qq_43799400/article/details/134182459
### 运行代码报错解决
- 报错Error Code 1: Cuda Runtime (an illegal memory access was encountered)，CUDA Runtime error💥 cudaFreeHost(cpu_) # an illegal memory access was encountered, code = cudaErrorIllegalAddress [ 700 ]
原因是加载的engine的类型不正确（fp32不支持），需要在训练时指定类型

- 报错CUDA Runtime error💥 cudaPeekAtLastError() # no kernel image is available for execution on the device, code = cudaErrorNoKernelImageForDevice [ 209 ]
原因是cmakelists.txt设置的gpu算力和本电脑的显卡gpu的算力不匹配
> https://blog.csdn.net/FL1623863129/article/details/124207995
> https://en.wikipedia.org/wiki/CUDA#GPUs_supported
### 环境配置
- 显卡驱动rtx2060，版本535.183.01，最高支持cuda12.2
- cuda12.2,cudnn和cuda版本匹配即可
  >https://blog.csdn.net/tangjiahao10/article/details/125225786
- opencv4.5.1，高版本应该也适配
- pcl1.13.1，目前没有深入进行编译测试
- gcc,g++版本11.4,cmake版本3.28.3
- tensorRT版本8.6.1.6
- gpu算力75
### 使用方法
- 进入tensorRT的路径，例如
  > /home/ddxy/project/TensorRT-8.6.1.6.Linux.x86_64-gnu.cuda-12.0/TensorRT-8.6.1.6/targets/x86_64-linux-gnu/bin 

- 利用可执行文件trtexec将onnx文件进行转换，例如
  > ./trtexec --onnx=yolov8n.transd.onnx \
    --minShapes=images:1x3x640x640 \
    --maxShapes=images:16x3x640x640 \
    --optShapes=images:1x3x640x640 \
    --saveEngine=yolov8n.transd.engine

  > ./trtexec --onnx=yolov8n-seg.b1.transd.onnx \
    --saveEngine=yolov8n-seg.b1.transd.engine
- #### 注意其中的报错或者警告
- .pt文件转换onnx文件教程后续添加
- 将转换的.onnx通过tensorRT转换为.engine文件，放在指定路径下，通过函数load即可调用
- 对文件中的src/kernels进行单独编译，生成的libcu.a静态链接库可以在build文件夹中找到，复制到workspace文件夹中，之后删除build文件夹，对该文件中的另一个cmakelists.txt进行编译，得到可执行文件pro，在workspace文件夹中，运行即可
- 可以使用time函数进行速度检测，经测试，速度可以达到0.5ms一次推理，但由于相机帧数的限制，只能达到30帧的速度
### 代码介绍
- cpm.hpp：生产者-消费者模型，对于直接推理任务，cpm.hpp 可以转变为自动多批次生产者-消费者模型
- infer.hpp：tensorRT 的重新包装，接口简单
- yolo.hpp：yolo 任务的包装器。基于 infer.hpp
---
