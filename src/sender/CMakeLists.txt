# 创建静态库
add_library(sender SHARED
    sender.cpp
)

# 设置目标属性
target_include_directories(sender PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/thirdpart/include
)

# 链接必要的库
target_link_libraries(sender PUBLIC
    pthread
    ${THIRD_PARTY_DIR}/lib/protobuf
)