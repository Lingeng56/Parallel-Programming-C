cmake_minimum_required(VERSION 3.17)
project(parallel C)


set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Xclang -fopenmp")
INCLUDE_DIRECTORIES(/opt/homebrew/opt/libomp/include)
LINK_DIRECTORIES(/opt/homebrew/opt/libomp/lib)
LINK_LIBRARIES(omp)

add_library(parallel library.c library.h timer.h opm/pro_homework/my_rand.h)

add_executable(demo01 pthread/demo01.c)
