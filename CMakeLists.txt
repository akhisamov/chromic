cmake_minimum_required(VERSION 3.26)
project(chromic)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

add_subdirectory(imgui)

add_executable(chromic)
target_include_directories(chromic PRIVATE include)
target_link_libraries(chromic PRIVATE
  # SDL2main
  ImGui
)

add_subdirectory(src)

if (${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  set(RES_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res)
else ()
  set(RES_OUTPUT_DIR ${CMAKE_BINARY_DIR}/res)
endif ()

add_custom_command(TARGET chromic PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/res ${RES_OUTPUT_DIR}
)
