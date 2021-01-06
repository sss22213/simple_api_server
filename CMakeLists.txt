project(simple_api_server)
cmake_minimum_required(VERSION 3.3)
add_definitions(-std=c++14 -Wall -g)
# document
find_package(Doxygen REQUIRED)
set(DOXYGEN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile)
add_custom_target( docs ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_FILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc
    VERBATIM )
#
include_directories(/usr/local/include)
link_directories(/usr/local/lib)
add_executable(main src/main.cpp src/http_server.cpp src/http_client.cpp)
target_link_libraries(main -lsockpp -lpthread)