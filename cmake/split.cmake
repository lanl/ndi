FILE(READ "ndi_keywords.h" contents)
string(REGEX REPLACE "\n" ";" lines "${contents}")

set(output "
      integer, parameter :: NDI_REAL = c_double
      integer, parameter :: NDI_INT  = c_int32_t")

foreach( line IN LISTS lines )                               
  string(REGEX REPLACE "s+" ";" elements "${line}")            
  list(LENGTH elements size)                                 
  list(GET 0 elements first)                                 
  if(size EQUAL '3' AND first STREQUAL "#define")            
    list(GET 1 elements second)                              
    list(GET 2 elements third)                               
    set(output "${output}
      integer(NDI_INT), parameter :: ${second} = ${third}")
  endif()
endforeach()                                                 

FILE(WRITE "ndi_keywords.i" "${output}")
