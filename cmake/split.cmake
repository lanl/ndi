FILE(READ "${source_dir}/ndi_keywords.h" contents)
string(REGEX REPLACE "\n" ";" lines "${contents}")

set(output "
      integer, parameter :: NDI_REAL = 8
      integer, parameter :: NDI_INT  = 4")

foreach(line IN LISTS lines)
  if(line)
    string(REGEX REPLACE "[ \t]+" ";" elements "${line}")
    list(LENGTH elements size)
    list(GET elements 0 first)
    if(size EQUAL "3" AND first STREQUAL "#define")
      list(GET elements 1 second)
      list(GET elements 2 third)
      set(output "${output}
      integer(NDI_INT), parameter :: ${second} = ${third}")
    endif()
  endif()
endforeach()
set(output "${output}\n")

FILE(WRITE "${binary_dir}/ndi_keywords.i" "${output}")
