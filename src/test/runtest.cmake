find_package(Perl REQUIRED)

execute_process(
  COMMAND ${exe}
  OUTPUT_VARIABLE stdout
  ERROR_VARIABLE stderr
  RESULT_VARIABLE result)

file(WRITE ${output} "${stdout}")

if(NOT ${result} EQUAL "0")
  message(STATUS "return code: ${result}")
  message(STATUS "stdout: ${stdout}")
  message(STATUS "stderr: ${stderr}")
  message(FATAL_ERROR "trouble while executing: ${exe}")
endif()

execute_process(
  COMMAND ${PERL_EXECUTABLE} ${script} ${output} ${reference}
  OUTPUT_VARIABLE stdout
  ERROR_VARIABLE stderr
  RESULT_VARIABLE result)

if(NOT ${result} EQUAL "0")
  message(STATUS "stdout: ${stdout}")
  message(STATUS "stderr: ${stderr}")
  message(FATAL_ERROR "diff in output from: ${exe}")
endif()
