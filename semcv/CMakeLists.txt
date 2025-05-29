cmake_minimum_required(VERSION 3.15)
project(semcv)

add_library(semcv semcv.cpp include/semcv/semcv.hpp)

set_property(TARGET semcv PROPERTY CXX_STANDARD 20)

target_include_directories(semcv PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

find_package(OpenCV REQUIRED)

target_link_libraries(semcv PRIVATE opencv_core opencv_highgui)
