CMAKE_MINIMUM_REQUIRED(VERSION 3.3)
PROJECT(tony)

SET(CMAKE_CXX_STANDARD 14)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tflite")

add_library(tony SHARED
        src/model/ModelReader.cpp
        src/Controller.cpp
        src/Invoker.cpp
        src/utils/TimeUtil.cpp
        )

target_link_libraries(tony PUBLIC tflite z log)
target_include_directories(tony PUBLIC include)

#===================================================================================================
#------ Add sub-modules ----------------------------------------------------------------------------
list(APPEND as_submodule tony)
#===================================================================================================