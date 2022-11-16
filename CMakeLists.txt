# cmake_minimum_required(VERSION <specify CMake version here>)
cmake_minimum_required(VERSION 3.19)
project(yijinjing)

set(CMAKE_BUILD_TYPE "Debug") #Release Debug
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -DOS_IS_LINUX")
add_compile_options(-std=c++17 -Wall -O2)

SET(ROOT_DIR "/home/work/sys/lib")
macro(SUBDIRLIST result curdir)
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)
    set(dirlist "")
    foreach(child ${children})
        if(IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${child})
        endif()
    endforeach()
    set(${result} ${dirlist})
endmacro()

SUBDIRLIST(all_sub_index ${ROOT_DIR})
foreach(child ${all_sub_index})
        list(APPEND all_include ${ROOT_DIR}/${child}/include)
        list(APPEND all_lib ${ROOT_DIR}/${child}/lib)
endforeach()

include_directories(${all_include})
link_directories(${all_lib})

aux_source_directory (./src/journal LIB_LIST)

SET(BROKER_LIBRARY "zhongxinjiantou_macli")
SET(BROKER "zhongxinjiantou_credit_broker")
SET(BROKER_TEST "test")

add_library(${BROKER_LIBRARY} ${LIB_LIST})
