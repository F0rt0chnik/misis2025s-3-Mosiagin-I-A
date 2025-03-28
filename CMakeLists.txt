cmake_minimum_required(VERSION 3.10)
project(YourProjectName)

# Явно указываем архитектуру
if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "Build architecture") # Для M1/M2
    # set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE STRING "Build architecture") # Для Intel
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Явно указываем путь к OpenCV
set(OpenCV_DIR "/opt/homebrew/opt/opencv/lib/cmake/opencv4") # Для M1/M2
# set(OpenCV_DIR "/usr/local/opt/opencv/lib/cmake/opencv4") # Для Intel

find_package(OpenCV REQUIRED)

add_subdirectory(semcv)
add_subdirectory(apps/task01-01)