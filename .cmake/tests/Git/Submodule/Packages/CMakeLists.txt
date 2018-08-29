include(CTest)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../..)

include(Git/Submodule/Packages)

set(JSON_BuildTests OFF CACHE INTERNAL "")
find_package(json)
find_package(Catch2)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/bar.cpp
"#include <cassert>

 int main(){
   assert(\"${json.submodule.hash}\" == \"7bfc406ded0434c438dd22139a8baa97f2ffa90e\" );
   assert(\"${Catch2.submodule.branch}\" == \"master\" );
 }")

add_executable(Git.Submodule.Packages.test ${CMAKE_CURRENT_BINARY_DIR}/bar.cpp)

add_test(
  NAME Git.Submodule.Packages.test
  COMMAND Git.Submodule.Packages.test)
