cmake_minimum_required(VERSION 3.0)
project(bga)

find_package(OpenCV REQUIRED)
#find_package(zbar REQUIRED)

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
    src/detect_box.cpp
    src/decode_barcode.cpp
    src/detect_text.cpp
)
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${OpenCV__INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(
    ${PROJECT_NAME}
    ${OpenCV_LIBS}
    zbar
    tesseract
)