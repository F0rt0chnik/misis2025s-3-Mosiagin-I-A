cmake_minimum_required(VERSION 3.10)

# Указываем исходный файл для приложения
set(SOURCES main.cpp)

# Создаем исполнимый файл
add_executable(task01-01 ${SOURCES})

# Указываем зависимость от библиотеки semcv, которая была создана в папке semcv
target_link_libraries(task01-01 PRIVATE semcv ${OpenCV_LIBS})
