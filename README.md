# Week3
=====================================================
# Development Enviroment
 - Visual Studio 2017

#Program Description
 - 主程序`week3.cpp`，整体结构以及函数借鉴了雷霄骅的博客：https://blog.csdn.net/leixiaohua1020/article/details/47068015 。
 - 编译后，在Debug/Release中运行exe, 亦可以直接在Debug文件夹中运行week3.exe。
 - 运行方法为，在week3.exe的路径中打开cmd 输入 week3 MP4文件名.mp4。
 - 程序目前仅测试了MP4的播放，其余视频封装格式未测试。
 - 程序仅支持画面播放，不可以同时进行其他操作，结束视频仅支持任务管理器强制关闭 。
 - 上述问题将在后续版本中尝试用线程解决。

#Developer
 - YYP

#Instructions
 - 程序目录下打开cmd， 输入week3 test.exe
 - test.mp4 是测试视频,文件名作为启动参数
 - 其余mp4 视频理论上也支持播放，前提是输入正确的路径，或与week3.exe在同一文件夹下
 - 传递给程序的启动参数是mp4文件的文件路径，若mp4文件与`videoPlayer.exe`文件在同一文件夹下，则可直接将mp4文件名作为启动参数  
 - 若遇到`查找不到.dll文件`错误信息，请将所需的`.dll`文件复制到week3.exe的相同目录下即可解决。  
