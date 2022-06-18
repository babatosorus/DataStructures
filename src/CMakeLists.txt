set(DATA_STRUCTURES_SRCS
  ${INC_ROOT}/avltree.h
  heap.cpp
  undirectedSimpleGraph.cpp
  disjoinset.cpp
)

add_library(DataStructures ${DATA_STRUCTURES_SRCS})

target_link_libraries(DataStructures )

target_include_directories(
    DataStructures
  PUBLIC
    ${INC_ROOT}
  PRIVATE
) 

