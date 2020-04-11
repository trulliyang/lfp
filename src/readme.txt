第一部分 环境配置
在Ubuntu下安装opencv
参考
https://www.cnblogs.com/chenzhen0530/p/12109868.html
https://blog.csdn.net/lj19940110/article/details/101012837?depth_1-utm_source=distribute.pc_relevant.none-task&utm_source=distribute.pc_relevant.none-task

第二部分 源代码
流程为
1、loadConfiguration()读取json配置
2、loadPhotos()加载照片
3、processPhotos()添加特效
4、processVideo()导出视频
备注
_path0,_path1,...,_pathN用于存放输入的照片路径。
_finalWidth,_finalHeight是导出视频的尺寸。

第三部分 命令行
在main.cpp的目录下，执行g++ -std=c++11 main.cpp `pkg-config --libs --cflags opencv4` -o result
这样可以编译出可执行文件result，main.cpp为源文件。
执行./result lalala "{\"photoPaths\":[\"/a/b/c/0.png\",\"/a/b/c/1.png\",\"/a/b/c/2.png\"],\"templateParameters\":{\"finalWidth\":720,\"finalHeight\":1280,\"totalNumberOfPhotos\":2}}"
这样可以到处视频outlalala,mp4。
参数lalala是输出视频的独有名字部分。
参数"..."是json字符串，photoPaths是每张照片路径，finalWidth和finalHeight是输出视频尺寸，totalNumberOfPhotos是照片总数。
