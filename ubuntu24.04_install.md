## 24.04环境配置（问题很多）：
- 显卡驱动自带，535.183.01,cuda限制12.2,不需要换显卡驱动，好评
- qq:正常安装
- k4a相机环境搭建，一般笔记本是amd64，小电脑是arm64架构，20.04以上版本需要换源
  > https://blog.csdn.net/qq_42108414/article/details/129015474

  > https://blog.csdn.net/wxd1233/article/details/121779276
- chrome：刚装上去不能启动，用指令也没有报错，试着改了安装方式，修改系统配置，删除系统文件，都没有起作用，最后重启之后正常工作
- utools：较为顺利，快捷键与ubuntu的快捷键冲突，需要修改，设置自启动有些麻烦 
   >https://www.cnblogs.com/xzh-personal-issue/p/18321354
- qqmusic：问题和22.04一致，需要改系统配置 
  >https://blog.csdn.net/LouisLCherry/article/details/133844022
- clash：用的文件比较老，指令启动需要加--no-sandbox，所以无法自启动，目前是采用shell脚本加上python脚本进行开机自启动，相对麻烦
- gcc，g++，cmake：正常安装，cmake需要配置ninja-build
- vscode：正常安装，配置了一些好玩的插件
- opencv-4.9：正常安装，一遍成功
- k4a驱动：需要安装libsoundio1，要换源，和22.04一致
- qt5：正常安装 
  > https://blog.csdn.net/weixin_48560325/article/details/124373125
- vtk：问题极多，心态爆炸，9.3版本一遍成功，但在后续编译pointcloud代码时有关于vtk的问题，查阅相关资料发现可能是版本问题，换之前的9.2.6进行安装，报错很奇怪，网上资料很少，几乎都是源代码问题，改了几个发现改不完，选择换8.2版本进行安装，仍然是很多问题，依旧需要修改源代码，最后装了9.3版本。
  >https://blog.csdn.net/qq_38606680/article/details/137019631  
- pcl-1.13.1：虽然一遍成功，但是make中出现了一些问题，对于报错导致程序停止的情况，就重新进行make，最后磕磕绊绊装完了，运行代码编译时没有太大问题，只是在vtk调用上有问题，准备最后装好vtk-9.3更新pcl版本时发现能正确编译了，就没有再更新版本（可能都是隐患）
  >https://blog.csdn.net/qq_41092406/article/details/117930972   

  >https://blog.csdn.net/m0_72734364/article/details/134183991?spm=1001.2101.3001.6661.1&utm_medium=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ECtr-1-134183991-blog-123754272.235%5Ev43%5Epc_blog_bottom_relevance_base2&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ECtr-1-134183991-blog-123754272.235%5Ev43%5Epc_blog_bottom_relevance_base2&utm_relevant_index=1
- anaconda：正常安装，一遍成功
  >https://developer.huawei.com/consumer/cn/blog/topic/03940616429410292
- cuda-12.1：有问题，报错是gcc版本问题，改了很多版本，在官网上查看了匹配的gcc版本，也没有解决，最后换了12.2成功安装
  >https://blog.csdn.net/ckhxhxxss/article/details/136951689 

  >https://blog.csdn.net/aizsa111/article/details/129455363 
- cudnn：似乎和之前装的流程不太一样，但也装上了，检测能用 
  >https://blog.csdn.net/weixin_37926734/article/details/123033286 
  
  >https://blog.csdn.net/qq_42731705/article/details/123797571?utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_baidulandingword~default-0-123797571-blog-135015041.235^v43^pc_blog_bottom_relevance_base2&spm=1001.2101.3001.4242.1&utm_relevant_index=3
- 最后运行pointcloud代码时的问题：常见问题软链接解决
  >https://blog.csdn.net/HITORANGE/article/details/135315149
- python环境的配置：很顺利，短时间可以达到onnx的转换和训练，说明我改的yolov5的包很牛，就是目前已经用了110G的空间,pip开始有点问题，用了conda安装，后面突然就好了,其他的一些配置常见问题（例如缺少某个包）很容易解决和搜索，在此不过多赘述
  >https://blog.csdn.net/weixin_45552562/article/details/109668589
- tensorRT的环境配置：很顺利，似乎和之前的配置也不太一样
  >https://blog.csdn.net/ytusdc/article/details/140143354?spm=1001.2101.3001.6650.3&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EYuanLiJiHua%7EPosition-3-140143354-blog-85255760.235%5Ev43%5Epc_blog_bottom_relevance_base2&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EYuanLiJiHua%7EPosition-3-140143354-blog-85255760.235%5Ev43%5Epc_blog_bottom_relevance_base2&utm_relevant_index=4

  >https://blog.csdn.net/weixin_43863869/article/details/128571567
- tensorRT可以直接将onnx模型转为engine模型或者trt模型，方便后面的架构，protubuf没有装，新的架构不需要而且这个也很容易出一些奇奇怪怪的问题，在上一个22.04版本时用了几天就出现版本问题，试过重新进行搭建环境或者升级降级版本均没有作用，所以后期用小电脑直接调试了
  >https://github.com/shouxieai/tensorRT_Pro
- 输入法：一坨，难以忍受！！！
- steam配置成功，可以运行支持linux的游戏，例如缺氧，古墓丽影系列，生化危机的部分版本，消逝的光芒等等
- ros/ros2应该还好，有一键安装
  >https://blog.csdn.net/jianlai_/article/details/123545130
- cuda和cudnn的版本匹配
  >https://blog.csdn.net/tangjiahao10/article/details/125225786
--- 
目前来看，配置了整整一天才勉强能用，不太好配，估计配了python环境问题更多