#cmake最低版本需求，不加入此行会受到警告信息
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
#项目名称
PROJECT(FRONTEND)

#add nullptr resolve
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

#rose
include_directories(/usr/rose/include/rose)
link_directories(/usr/rose/lib)

#把目录(./src)下所有源代码文件和头文件加入变量SRC_LIST
AUX_SOURCE_DIRECTORY(./src SRC_LIST)
AUX_SOURCE_DIRECTORY(./include INCLUDE_LIST)

link_libraries(rose)

#boost库
find_package(Boost REQUIRED COMPONENTS
        #要使用的boost库
        system
        chrono
        )
if(NOT Boost_FOUND)
    message("Not found Boost")
endif()

include_directories(${Boost_INCLUDE_DIRS})
message("${Boost_INCLUDE_DIRS}")
message("${Boost_LIBRARIES}")

include_directories(./include)
#生成应用程序 frontend
ADD_EXECUTABLE(frontend ${SRC_LIST} ${INCLUDE_LIST})

target_link_libraries(frontend ${Boost_LIBRARIES} /usr/lib/x86_64-linux-gnu/libquadmath.so.0) #链接到工程
install(TARGETS frontend RUNTIME DESTINATION bin)
