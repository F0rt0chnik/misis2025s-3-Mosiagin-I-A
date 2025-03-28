add_library(semcv STATIC
    src/semcv.cpp
    include/semcv/semcv.hpp
)

target_include_directories(semcv PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(semcv PRIVATE ${OpenCV_LIBS})