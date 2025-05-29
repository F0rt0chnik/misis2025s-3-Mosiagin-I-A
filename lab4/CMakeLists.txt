cmake_minimum_required(VERSION 3.12)
project(task04_all)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)
find_package(nlohmann_json REQUIRED)

# Пути к библиотеке semcv
set(SEMCV_INCLUDE_DIR /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/semcv/include)
set(SEMCV_LIBRARY /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/semcv/build/libsemcv.a)

# Добавляем исполняемые файлы
add_executable(task04_01 "task04_01.cpp")
add_executable(task04_02 "task04_02.cpp")
add_executable(task04_03 "task04_03.cpp")

# Подключаем заголовки semcv
target_include_directories(task04_01 PRIVATE ${SEMCV_INCLUDE_DIR})
target_include_directories(task04_02 PRIVATE ${SEMCV_INCLUDE_DIR})
target_include_directories(task04_03 PRIVATE ${SEMCV_INCLUDE_DIR})

# Линкуем библиотеки
target_link_libraries(task04_01 PRIVATE opencv_core opencv_highgui nlohmann_json::nlohmann_json ${SEMCV_LIBRARY})
target_link_libraries(task04_02 PRIVATE opencv_core opencv_highgui nlohmann_json::nlohmann_json ${SEMCV_LIBRARY})
target_link_libraries(task04_03 PRIVATE opencv_core opencv_highgui nlohmann_json::nlohmann_json ${SEMCV_LIBRARY})
