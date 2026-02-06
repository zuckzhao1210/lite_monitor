# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(lite_monitor)

# 设置C++标准为C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#第三方库目录
set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party)

# 包含头文件目录
include_directories(include)

# 第三方库头文件
include_directories(${THIRD_PARTY_DIR}/include)

# 第三方库路径
link_directories(${THIRD_PARTY_DIR}/lib)

# 添加可执行文件
add_executable(main main.cpp)

# 链接第三方库
target_link_libraries(main yaml-cpp)