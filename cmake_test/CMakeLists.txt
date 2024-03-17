#指定cmake版本
cmake_minimum_required(VERSION 3.22.1)
#指定项目名（随意
project(test)
#显示指定要编译的文件生成文件链接
#set(src add.c;div.c;main.c;sub.c;mult.c)
#指定生成可执行文件路径
set(HOME /home/w/src/cmake_test/build)
set(EXECUTABLE_OUTPUT_PATH ${HOME})
#搜索当前txt文件目录下头文件
include_directories(${PROJECT_SOURCE_DIR}/include)
#搜索当前txt文件目录下.c文件2
#file(GLOB SRC_LIST ${PROJECT_SOURCE_DIR}/src/*c)
#搜索当前txt文件目录下.c文件
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC_LIST)
#设置库的输出文件地址 适用静态（没有权限）  LIBRARY_OUTPUT_PATH 
set(LIBRARY_OUTPUT_PATH /home/w/src/cmake_test/lib1)
#添加一个库.a为静态STATIC  .so为动态SHARE  
add_library(run STATIC ${SRC_LSIT})
#动态库
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib2)
add_library(run1 SHARED ${SRC_LIST})
#静态库
link_libraries(libxxx.a)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib1)
#动态库
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib2)
#指定使用c++版本（默认98
set(CMAKE_CXX_STANDARD 11)
# 定义工程会生成一个可执行程序 
add_executable(go ${SRC_LIST}) 
#连接动态库（只能在add_executanle后
target_link_libraries(go run2)
