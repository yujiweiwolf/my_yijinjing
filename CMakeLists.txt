# cmake_minimum_required(VERSION <specify CMake version here>)
cmake_minimum_required(VERSION 3.19)
project(my_yijinjing)

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
aux_source_directory (./src/strategy STRATEGY_LIST)

SET(BROKER_LIBRARY "journal")
SET(WRITEER "writer")
SET(READER "reader")

SET(STRATEGY_LIBRARY "base_strtegy")
SET(TEST_STRATEGY "test_strategy")

add_library(${BROKER_LIBRARY} ${LIB_LIST})
add_library(${STRATEGY_LIBRARY} ${STRATEGY_LIST})

add_executable(${WRITEER} src/test/writer.cpp)
target_link_libraries(${WRITEER}
        ${BROKER_LIBRARY} boost_date_time boost_filesystem boost_regex boost_system  boost_chrono boost_log boost_program_options boost_thread boost_iostreams z protobuf protobuf-lite sodium zmq ssl crypto iconv pthread dl)


add_executable(${READER} src/test/reader.cpp)
target_link_libraries(${READER}
        ${BROKER_LIBRARY} boost_date_time boost_filesystem boost_regex boost_system  boost_chrono boost_log boost_program_options boost_thread boost_iostreams z protobuf protobuf-lite sodium zmq ssl crypto iconv pthread dl)

add_executable(${TEST_STRATEGY} src/test_strategy/main.cpp src/test_strategy/test_strategy.cc)
target_link_libraries(${TEST_STRATEGY}
        ${STRATEGY_LIBRARY} ${BROKER_LIBRARY} boost_date_time boost_filesystem boost_regex boost_system  boost_chrono boost_log boost_program_options boost_thread boost_iostreams z protobuf protobuf-lite sodium zmq ssl crypto iconv pthread dl)


#FILE(COPY Dockerfile image.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#
#FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/image.sh "n=$1; v=`./$n -v`; ldd $n | grep /sys/lib/ | awk '{print $3}' | xargs -i cp -L -n {} .; docker build -t $n:$v .; rm -rf \${n}_$v.docker.tar.gz; docker save $n:$v > \${n}_$v.docker.tar && gzip \${n}_$v.docker.tar")
#add_custom_target(image COMMAND sh image.sh reader DEPENDS reader)