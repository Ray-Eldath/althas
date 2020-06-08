cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)

find_package(verilator REQUIRED HINTS $ENV{VERILATOR_ROOT} ${VERILATOR_ROOT})
if (NOT verilator_FOUND)
    message(FATAL_ERROR "Verilator was not found. Either install it, or set the VERILATOR_ROOT environment variable")
endif ()
set(verilator_INCLUDE_DIR ${VERILATOR_ROOT}/include)

add_executable(sim_pc sim_pc.cpp)
target_include_directories(sim_pc
        PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

verilate(sim_pc
        TRACE
        INCLUDE_DIRS "."
        SOURCES pc_cmd.sv pc.sv
        PREFIX PC)