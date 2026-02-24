project(http)

add_library(http SHARED
    http_server.cpp
)

# 第三方库头文件
target_include_directories(http PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/thirdpart/include
)