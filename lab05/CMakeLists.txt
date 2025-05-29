cmake_minimum_required(VERSION 3.12)
project(kr_task)

set(CMAKE_CXX_STANDARD 17)

# Поиск OpenCV
find_package(OpenCV REQUIRED)

# Пути к semcv (если понадобится подключить вручную)
set(SEMCV_INCLUDE_DIR /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/semcv/include)
set(SEMCV_LIBRARY /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/semcv/build/libsemcv.a)

# Добавление исполняемого файла
add_executable(kr-task "kr-task.cpp")

# Подключение include-директории (на всякий случай)
target_include_directories(kr-task PRIVATE ${SEMCV_INCLUDE_DIR})

# Линковка 
target_link_libraries(kr-task PRIVATE ${OpenCV_LIBS})

target_link_libraries(kr-task PRIVATE ${SEMCV_LIBRARY})
