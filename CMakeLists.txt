cmake_minimum_required(VERSION 2.8)

project(Spark_Http_Service)

file(GLOB_RECURSE srcs ${PROJECT_SOURCE_DIR}/src/*.cpp)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/sdk/Spark2.0_Linux_SDK_v1.0/include
)

add_library(srv_spark SHARED ${srcs})