cmake_minimum_required(VERSION 3.0)

project(Spark_Http_Service)

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -ggdb -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wno-unused")

file(GLOB_RECURSE srcs ${PROJECT_SOURCE_DIR}/src/*.cpp)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/sdk/Spark2.0_Linux_SDK_v1.0/include
)

add_library(srv_spark SHARED ${srcs})