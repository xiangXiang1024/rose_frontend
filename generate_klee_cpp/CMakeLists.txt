CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(generate_main)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

include_directories(/usr/rose/include/rose)
link_directories(/usr/rose/lib)

link_libraries(rose)

find_package(Boost REQUIRED COMPONENTS
        #要使用的boost库
        system
        chrono
        )
if(NOT Boost_FOUND)
    message("Not found Boost")
endif()

include_directories(${Boost_INCLUDE_DIRS})

AUX_SOURCE_DIRECTORY(./src SRC_LIST)
AUX_SOURCE_DIRECTORY(./include INCLUDE_LIST)

include_directories(./include)

ADD_EXECUTABLE(generate_main  ${SRC_LIST} ${INCLUDE_LIST} )
target_link_libraries(generate_main ${Boost_LIBRARIES} /usr/lib/x86_64-linux-gnu/libquadmath.so.0) 
