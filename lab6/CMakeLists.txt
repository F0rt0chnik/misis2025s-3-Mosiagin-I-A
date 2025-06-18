cmake_minimum_required(VERSION 3.12)
project(task06)

set(CMAKE_CXX_STANDARD 20)

# Поиск OpenCV
find_package(OpenCV REQUIRED)

# Поиск nlohmann_json
find_package(nlohmann_json REQUIRED)

# Пути к semcv — укажи свои реальные пути
set(SEMCV_INCLUDE_DIR /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/semcv/include)
set(SEMCV_LIBRARY /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/semcv/build/libsemcv.a)

# Добавление исполняемого файла
add_executable(task06 "task06.cpp")

# Подключение include-директории semcv
target_include_directories(task06 PRIVATE ${SEMCV_INCLUDE_DIR})

# Линковка с OpenCV, semcv и nlohmann_json
target_link_libraries(task06 PRIVATE ${OpenCV_LIBS} ${SEMCV_LIBRARY} nlohmann_json::nlohmann_json)
