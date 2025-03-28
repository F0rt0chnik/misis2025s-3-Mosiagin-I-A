target_link_libraries(task01-02 PRIVATE
    semcv
    ${OpenCV_LIBS}
    "-ldl"
    "-framework Accelerate"
    "-framework CoreFoundation"
)