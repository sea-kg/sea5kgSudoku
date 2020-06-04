cmake_minimum_required(VERSION 3.0)

project(sea5kgSudoku C CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/src.wsjcpp/CMakeLists.txt)

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${sea5kgSudoku_SOURCE_DIR})

# include header dirs
list (APPEND WSJCPP_INCLUDE_DIRS "src")

list (APPEND WSJCPP_SOURCES "src/main.cpp")
list (APPEND WSJCPP_SOURCES "src/sea5kgSudoku.h")
list (APPEND WSJCPP_SOURCES "src/sea5kgSudoku.cpp")

#### BEGIN_WSJCPP_APPEND
#### END_WSJCPP_APPEND

include_directories(${WSJCPP_INCLUDE_DIRS})

add_executable (sea5kgSudoku ${WSJCPP_SOURCES})

target_link_libraries(sea5kgSudoku ${WSJCPP_LIBRARIES})

install(
    TARGETS
        sea5kgSudoku
    RUNTIME DESTINATION
        /usr/bin
)

