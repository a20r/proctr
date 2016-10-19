cmake_minimum_required (VERSION 2.6)
project (proctr CXX)

enable_testing()
file(GLOB TESTING_SRC *.cpp)
add_executable(run_tests ${TESTING_SRC})
target_link_libraries(run_tests gtest pthread gurobi_c++ gurobi65 proctr cnpy)
