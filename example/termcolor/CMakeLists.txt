add_executable(test_termcolor test_termcolor.cpp)
target_include_directories(test_termcolor
        PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)