# 1. 设置 CMake 最低版本
cmake_minimum_required(VERSION 3.10)

# 2. 设置项目名称
project(OpenCV_Demo)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(OpenCV_DIR "C:/Data/opencv/build/x64/vc16/lib")
endif ()

# 3. 寻找 OpenCV 包
# REQUIRED 表示必须找到，否则报错
find_package(OpenCV REQUIRED)

# 4. 打印 OpenCV 版本信息 (可选，用于调试)
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include paths: ${OpenCV_INCLUDE_DIRS}")

#add_subdirectory(3rd-party/glm)

file(GLOB_RECURSE SRC_LIST "src/*.cpp")

# 2. 自动扫描 include 目录下的所有头文件 (可选)
file(GLOB_RECURSE HEADER_LIST "include/*.hpp")

# 5. 添加可执行文件
add_executable(demo ./test/main.cpp ${SRC_LIST})

target_include_directories(demo SYSTEM PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/3rd-party
    ${OpenCV_INCLUDE_DIRS}
)

# 6. 链接 OpenCV 库到你的可执行文件
target_link_libraries(demo ${OpenCV_LIBS})

# (可选) 指定 C++ 标准
set(CMAKE_CXX_STANDARD 17)