cmake_minimum_required(VERSION 3.20)
project(football_ana)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")


set(BOOST_ROOT D:/boost_1_69_0)
set(BOOST_INCLUDEDIR ${BOOST_ROOT})
set(BOOST_LIBRARYDIR ${BOOST_ROOT}/stage/copy_from_lib)
find_package(Boost COMPONENTS REQUIRED log_setup log thread regex  filesystem)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

message("boost lib:${Boost_LIBRARIES}")
message("boost inc:${Boost_INCLUDE_DIR}")

set(lib_deps
        ${Boost_LIBRARIES}

        )

add_executable(football_ana main.cpp)
target_link_libraries(football_ana ${lib_deps})