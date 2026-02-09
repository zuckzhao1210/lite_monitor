# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(lite_monitor)

# 设置C++标准为C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译模式
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# 定义相关路径
set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdpart)
file(GLOB_RECURSE SRCS "src/*.cpp")

# 包含头文件目录
include_directories(include)

# 第三方库头文件
include_directories(${THIRD_PARTY_DIR}/include)

# 第三方库路径
link_directories(${THIRD_PARTY_DIR}/lib)
link_directories(${THIRD_PARTY_DIR}/lib/yaml-cpp)
link_directories(${THIRD_PARTY_DIR}/lib/mysql)

# 添加可执行文件
add_executable(lite_monitor ${SRCS})

# 链接第三方库
target_link_libraries(lite_monitor 
    yaml-cpp 
    pthread
    mysqlclient
    dl
    ssl
    crypto
)

# 安装可执行文件
set_target_properties(lite_monitor PROPERTIES
    INSTALL_RPATH "$ORIGIN/../lib"
)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "Standard Install Path" FORCE)
endif()
install(TARGETS lite_monitor DESTINATION bin)
install(DIRECTORY configs/ DESTINATION etc)
install(DIRECTORY scripts/ 
        DESTINATION scripts
        FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ)
install(DIRECTORY ${THIRD_PARTY_DIR}/lib/mysql/ DESTINATION lib USE_SOURCE_PERMISSIONS)
