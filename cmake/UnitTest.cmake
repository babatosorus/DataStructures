function( CreateUnitTest TARGET_NAME DEPENDENCIES)

  set(TEST_NAME ${TARGET_NAME}Test)
  set(COMMAND_ARGS ${ARGN})
  set(LIBSList  ${DEPENDENCIES})
  set(LabelsList ${ADDITIONAL_LABELS})

  separate_arguments(LIBSList)

  separate_arguments(LabelsList)

  if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_NAME}.cpp)
    message( STATUS "missing  test file " )
    return()
  endif()

  add_executable( ${TEST_NAME}
      ${TEST_NAME}.cpp
  )
  
  target_link_libraries( ${TEST_NAME} PUBLIC ${LIBSList} )

  add_test(
    NAME    ${TEST_NAME}
    COMMAND ${TEST_NAME} ${COMMAND_ARGS}
  )

endfunction( CreateUnitTest )
