include_directories(
    ${INC_ROOT}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

include(${PROJECT_SOURCE_DIR}/cmake/UnitTest.cmake)

CreateUnitTest(heap "DataStructures Catch2::Catch2")
CreateUnitTest(avltree "DataStructures Catch2::Catch2")




