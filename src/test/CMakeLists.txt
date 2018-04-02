file(GLOB c_sources "${CMAKE_CURRENT_LIST_DIR}/*.c")

foreach(c_source IN LISTS c_sources)
  get_filename_component(exe ${c_source} NAME_WE)
  add_executable(ndi.${exe} ${c_source})
  target_link_libraries(ndi.${exe} PUBLIC ndi_C_32 ndi_C_64)
endforeach()

