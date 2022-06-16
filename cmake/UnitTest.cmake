function( CreateUnitTest TARGET_NAME DEPENDENCIES ADDITIONAL_LABELS)

  set(TEST_NAME ${TARGET_NAME}Test)
  set(COMMAND_ARGS ${ARGN})
  set(LIBSList  ${DEPENDENCIES})
  set(LabelsList ${ADDITIONAL_LABELS})
  separate_arguments(LIBSList)
  separate_arguments(LabelsList)

  if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_NAME}.cxx)
    message( STATUS  "Create ${TEST_NAME} unit test")
    configure_file( ${PROJECT_SOURCE_DIR}/cmake/unitTestTemplate.cxx.in
                    ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_NAME}.cxx)
  endif()
  add_executable( ${TEST_NAME}
      ${TEST_NAME}.cxx 
  )
  
  target_link_libraries( ${TEST_NAME} PUBLIC ${LIBSList} )

  add_test(
    NAME    ${TEST_NAME}
    COMMAND ${TEST_NAME} ${COMMAND_ARGS}
  )

endfunction( CreateUnitTest )
