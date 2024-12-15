cmake_minimum_required(VERSION 3.30)
project(Parallel_Programming)

set(CMAKE_CXX_STANDARD 23)
enable_language(Fortran)

find_package(MPI REQUIRED)

add_executable(Parallel_Programming
#        exp3/exp3_3.cpp
#        "exp4/Parallel Monte Carlo Operation (Blocking).cpp"
#        "exp4/Parallel Monte Carlo Operation (Non-blocking).cpp"
        "exp4/Parallel Monte Carlo Operation Reduced Version.cpp"
#        "exp4/Serial Monte Carlo Operation Version.cpp"
        #        exp3/exp3_2.cpp
)

if (MPI_FOUND)
    target_link_libraries(Parallel_Programming PRIVATE MPI::MPI_CXX)
else ()
    message(FATAL_ERROR "MPI not found!")
endif ()
