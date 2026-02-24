project(db)

add_library(db SHARED
    db_operator.cpp
    db_result.hpp
)

# 第三方库头文件
target_include_directories(db PUBLIC 
    ${THIRD_PARTY_DIR}/include
    ${CMAKE_SOURCE_DIR}/include
)

# 链接必要的库
target_link_libraries(db PUBLIC
    pthread
)