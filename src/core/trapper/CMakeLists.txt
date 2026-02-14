# 创建静态库
add_library(trapper STATIC
    tcp_server.cpp
)

# 设置目标属性
target_include_directories(trapper PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${THIRD_PARTY_DIR}/include/spdlog
)

# 链接必要的库
target_link_libraries(trapper PUBLIC
    pthread
)