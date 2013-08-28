cmake_minimum_required(VERSION 2.8)
project(drift_detection)
aux_source_directory(src SRC_LIST)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -std=c++0x")

find_package( OpenCV REQUIRED )
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(drift_detection ${OpenCV_LIBS})

SET(CMAKE_CXX_FLAGS "-std=c++0x -Wall")
SET(CMAKE_BUILD_TYPE Debug)



