cmake_minimum_required(VERSION 3.10)
project(image_processing_tasks)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)

# Путь к заголовкам semcv
include_directories(/Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/semcv/include)

# Путь к собранной библиотеке semcv
link_directories(/Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/semcv/build)

# Основные исполняемые файлы
add_executable(generate_histograms task02.cpp)
add_executable(compute_statistics task02-stats.cpp)

# Линковка
target_link_libraries(generate_histograms PRIVATE ${OpenCV_LIBS} semcv)
target_link_libraries(compute_statistics PRIVATE ${OpenCV_LIBS} semcv)
