cmake_minimum_required(VERSION 3.12)
project(task03)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)

add_executable(task03 "task03.cpp")

# Подключаем заголовки semcv к task03
target_include_directories(task03 PRIVATE
    /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/semcv/include
)

# Линкуем OpenCV и semcv — полный путь к библиотеке semcv
target_link_libraries(task03 PRIVATE
    opencv_core
    opencv_highgui
    /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/semcv/build/libsemcv.a
)
