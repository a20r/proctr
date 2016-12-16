cmake_minimum_required (VERSION 2.6)
project (proctr CXX)

file(GLOB SRC_FILES *.cpp)
add_library(proctr STATIC ${SRC_FILES})
target_link_libraries(proctr pthread gurobi_c++ gurobi65)
