set(importName "NodeTiler")

get_filename_component(NodeTiler_ROOT "../NodeTiler" ABSOLUTE)


set(NodeTiler_LIB           ${NodeTiler_ROOT})

file(GLOB NodeTiler_Sources ${NodeTiler_ROOT}/src/*.cpp)
file(GLOB NodeTiler_Headers ${NodeTiler_ROOT}/include/*.h)

set(NodeTiler_INCLUDE_DIR   ${NodeTiler_ROOT}/include ${NodeTiler_ROOT}/lib/nlohmann_json/include)


message(STATUS "include ${importName} from ${NodeTiler_INCLUDE_DIR}")
