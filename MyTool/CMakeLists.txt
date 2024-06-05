cmake_minimum_required(VERSION 3.10)
project(MyClangTool)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

set(CMAKE_CXX_STANDARD 17)

# Add compile options to explicitly disable optimizations
add_compile_options(-O0 -fno-exceptions -fno-rtti)

add_executable(my_clang_tool tool.cpp)

# Ensure we link against the necessary LLVM and Clang libraries
llvm_map_components_to_libnames(llvm_libs support core irreader)
target_link_libraries(my_clang_tool
  PRIVATE
  ${llvm_libs}
  clangTooling
  clangBasic
  clangASTMatchers
  clangFrontend
  clangSerialization
  clangDriver
  clangParse
  clangSema
  clangAnalysis
  clangEdit
  clangAST
  clangLex
  clangRewrite
)

# Remove the separate setting for RTTI if it's handled globally in add_compile_options
# set_target_properties(my_clang_tool PROPERTIES
#   COMPILE_FLAGS "-fno-rtti"
# )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

