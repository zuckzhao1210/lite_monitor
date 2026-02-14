project(http)

add_library(http STATIC
    http_server.cpp
)

# 第三方库头文件
target_include_directories(http PUBLIC 
    ${THIRD_PARTY_DIR}/include/spdlog
    ${CMAKE_CURRENT_SOURCE_DIR}
)