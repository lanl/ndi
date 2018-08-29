cmake_minimum_required(VERSION 3.3.0)
project(shacl_cmake LANGUAGES C CXX Fortran VERSION 1.0.0)

include(CTest)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/..)

add_subdirectory(Fortran)
add_subdirectory(Intel)
add_subdirectory(LinkOptions)
add_subdirectory(GeneratedSources)
add_subdirectory(Sanitizers)
# add_subdirectory(Git/Submodule/Packages)
add_subdirectory(DependencyInjection)
add_subdirectory(Warnings)
add_subdirectory(JSONParser)
