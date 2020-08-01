# CMakeList.txt: week3 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project(week3)

include_directories("include")

link_libraries("lib")

# 将源代码添加到此项目的可执行文件。
add_executable (week3 "week3.cpp")

target_link_libraries(
	avcodec
	avdevice
	avfilter
	avformat
	avutil
	postproc
	swresample
	swscale
	SDL2
	SDL2main
)

# TODO: 如有需要，请添加测试并安装目标。
