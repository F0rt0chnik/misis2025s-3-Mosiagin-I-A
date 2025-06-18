cmake_minimum_required(VERSION 3.15)
project(task08)

set(CMAKE_CXX_STANDARD 20)

# === Поиск OpenCV ===
find_package(OpenCV REQUIRED)

# === Пути к библиотеке semcv ===
set(SEMCV_INCLUDE_DIR /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/semcv/include)
set(SEMCV_LIBRARY /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/semcv/build/libsemcv.a)

# === Добавление исполняемого файла ===
add_executable(task08 task08.cpp)

# === Подключение заголовков semcv ===
target_include_directories(task08 PRIVATE ${SEMCV_INCLUDE_DIR})

# === Линковка библиотек ===
target_link_libraries(task08 PRIVATE ${SEMCV_LIBRARY} ${OpenCV_LIBS})
