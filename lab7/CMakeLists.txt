cmake_minimum_required(VERSION 3.12)
project(task07)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)

find_package(nlohmann_json REQUIRED)

set(SEMCV_INCLUDE_DIR /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/semcv/include)
set(SEMCV_LIBRARY /Users/ivanmosagin/Desktop/GIT_REpa/misis2025s-3-secname-I-A/semcv/build/libsemcv.a)

add_executable(task07-01 "task07-01.cpp")
target_include_directories(task07-01 PRIVATE ${SEMCV_INCLUDE_DIR})
target_link_libraries(task07-01 PRIVATE ${OpenCV_LIBS} ${SEMCV_LIBRARY} nlohmann_json::nlohmann_json)

add_executable(task07-02 "task07-02.cpp")
target_include_directories(task07-02 PRIVATE ${SEMCV_INCLUDE_DIR})
target_link_libraries(task07-02 PRIVATE ${OpenCV_LIBS} ${SEMCV_LIBRARY} nlohmann_json::nlohmann_json)
