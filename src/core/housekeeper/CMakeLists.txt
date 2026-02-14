project(housekeeper)

# 包含头文件目录
include_directories(include)

add_library(housekeeper STATIC
    keeper.cpp
    history_ring_buffer.cpp
    history_syncer.cpp
    trends_syncer.cpp
)

# 第三方库头文件
target_include_directories(housekeeper PUBLIC 
    ${THIRD_PARTY_DIR}/include/spdlog
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 链接必要的库
target_link_libraries(housekeeper PUBLIC
    pthread
)