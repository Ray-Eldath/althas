cmake_minimum_required(VERSION 3.12)
project(althas_find_package)

#
# config example
#
set(CMAKE_CXX_STANDARD 17)
add_executable(exp_find_package sim_add_one.cpp)

#
# config Verilator
#
find_package(verilator REQUIRED HINTS $ENV{VERILATOR_ROOT} ${VERILATOR_ROOT})
if (NOT verilator_FOUND)
    message(FATAL_ERROR "Verilator was not found. Either install it, or set the VERILATOR_ROOT environment variable")
endif ()
set(verilator_INCLUDE_DIR ${VERILATOR_ROOT}/include)

verilate(exp_find_package
        TRACE
        INCLUDE_DIRS "."
        SOURCES add_one.sv
        PREFIX AddOne)

#
# config Althas
#
find_package(althas REQUIRED)
target_link_libraries(exp_find_package PRIVATE althas::althas)