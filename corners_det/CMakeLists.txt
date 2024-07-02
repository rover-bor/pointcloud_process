cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME corners_det)
project(${PROJECT_NAME} VERSION 1.0)

set(CMAKE_BUILD_TYPE "Release")
message(STATUS "${HOME}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out)
# set(CMAKE_PREFIX_PATH "${HOME}/open3d_install/lib/cmake/Open3D" ${CMAKE_PREFIX_PATH})

find_package(OpenCV REQUIRED)
list(APPEND CAMKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

message(STATUS "OpenCV library status:")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

aux_source_directory(src SOURCE_LIST)   # 查找arg1中的所有源文件，并将名称列表存储在arg2中
# add_subdirectory(include)     # 包含其他目录的CMakeLists.txt

# add_executable(${PROJECT_NAME} ${SOURCE_LIST})
add_executable(${PROJECT_NAME} src/parsing_bin.cpp)

link_directories(${OpenCV_LIBRARY_DIRS})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
# target_link_libraries(${PROJECT_NAME} UTILS)
