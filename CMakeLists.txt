cmake_minimum_required(VERSION 3.21)

if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()

PROJECT(DataStructures LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INC_ROOT ${PROJECT_SOURCE_DIR}/include)

include(${PROJECT_SOURCE_DIR}/cmake/dependencies.cmake)

if(BUILD_TESTING AND ${PROJECT_NAME}_IS_TOP_LEVEL)
    include(CTest)
    add_subdirectory(tests)
endif()

add_subdirectory(src)




