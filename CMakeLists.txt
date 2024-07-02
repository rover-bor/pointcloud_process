cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME pc_demo)
project(${PROJECT_NAME} VERSION 1.0)

set(CMAKE_BUILD_TYPE "Release")
message(STATUS "${HOME}")
set(Open3D_DIR /home/chenliangjin/open3d_install/lib/cmake/Open3D/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out)
# set(CMAKE_PREFIX_PATH "${HOME}/open3d_install/lib/cmake/Open3D" ${CMAKE_PREFIX_PATH})

find_package(OpenCV REQUIRED)
find_package(Open3D HINTS REQUIRED)
list(APPEND CAMKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
list(APPEND Open3D_LIBARIES dl)
if (Open3D_FOUND)
    message(STATUS "Found Open3D ${Open3D_VERSION}")
    link_directories(${Open3D_LIBARIES_DIRS})
endif()

message(STATUS "OpenCV library status:")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

aux_source_directory(src SOURCE_LIST)   # 查找arg1中的所有源文件，并将名称列表存储在arg2中
# add_subdirectory(include)     # 包含其他目录的CMakeLists.txt

add_executable(${PROJECT_NAME} ${SOURCE_LIST})

link_directories(${OpenCV_LIBRARY_DIRS})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
target_link_libraries(${PROJECT_NAME} ${Open3D_LIBRARIES})
# target_link_libraries(${PROJECT_NAME} UTILS)
