cmake_minimum_required(VERSION 2.6)

project(lenactions)
set(lenactions_MAJOR_VERSION 0)
set(lenactions_MINOR_VERSION 1)
set(lenactions_REVISION_VERSION 0)

#-------------- PLATFORM SPECIFIC COMPILATION FLAGS -----------------------
# Requires CMake >= 2.4.7
# Only tested on Linux...
#
#string(COMPARE EQUAL ${CMAKE_SYSTEM_NAME} "AIX" AIX)
#string(COMPARE EQUAL ${CMAKE_SYSTEM_NAME} "Darwin" APPLE)
#string(COMPARE EQUAL ${CMAKE_SYSTEM_NAME} "Linux" LINUX)
#string(COMPARE EQUAL ${CMAKE_SYSTEM_NAME} "SunOS" SUNOS)
#string(COMPARE EQUAL ${CMAKE_SYSTEM_NAME} "FreeBSD" FREEBSD)
#string(COMPARE EQUAL ${CMAKE_SYSTEM_NAME} "CYGWIN" CYGWIN)
#
#if( CYGWIN )
#  add_definitions(-D__cygwin__)
#elseif( AIX )
#  add_definitions(-D__aix__)
#elseif( APPLE )
#  add_definitions( -D__macosx__)
#elseif( LINUX )
#  add_definitions(-D__gnulinux__)
#elseif( SUNOS )
#  add_definitions(-D__sunos__)
#elseif( FREEBSD )
#  add_definitions(-D__freebsd__)
#endif( CYGWIN )
#add_definitions(-D_GRIDRPC_COMPILATION)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.7")
    message(FATAL_ERROR "${CMAKE_CXX_COMPILER} v${CMAKE_CXX_COMPILER_VERSION} found when g++ 4.7 or later is required")
    message(FATAL_ERROR "If installed, please use -DCMAKE_CXX_COMPILER:FILEPATH=g++-4.7 for example")
  endif()
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -o3")


add_subdirectory(src)
