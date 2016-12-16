cmake_minimum_required (VERSION 2.6)
project (proctr CXX)
enable_testing()
file(GLOB TESTING_SRC *.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
add_executable(run_tests ${TESTING_SRC})
target_link_libraries(run_tests gtest pthread gurobi_c++ $ENV{GUROBI_LIB_NAME}
    proctr ${Boost_LIBRARIES})
