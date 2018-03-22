cmake_minimum_required(VERSION 3.8)

project(tablestore VERSION 4.5.1)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

# 第三方库
find_package(Protobuf REQUIRED)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS system chrono thread)

# 源代码
add_subdirectory("src")
