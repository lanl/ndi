function(git_submodule_init name)
  if(NOT EXISTS "${${name}.submodule.path}/.git" )
    execute_process(
      COMMAND ${GIT_EXECUTABLE} submodule update --init -- ${${name}.submodule.path}
      WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
      OUTPUT_QUIET
      RESULT_VARIABLE failure
      ERROR_VARIABLE error_output)

    if(failure)
      message(FATAL "${error_output}")
    endif()
  endif()

  if(NOT DEFINED ${name}.submodule.branch)
    git_submodule_collect_state(${name})
  endif()
endfunction()
