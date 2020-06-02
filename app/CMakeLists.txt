cmake_minimum_required(VERSION 3.3)
project(native-lib)

set(CMAKE_CXX_STANDARD 14)

set(JNI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/jni)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/main/jni")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/tony")

add_library(
        native-lib
        SHARED
        ${JNI_DIR}/JNIWrapper.cpp
        ${JNI_DIR}/native-lib.cpp
)

target_link_libraries(native-lib tony tflite)
target_include_directories(native-lib PUBLIC)