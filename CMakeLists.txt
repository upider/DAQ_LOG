cmake_minimum_required(VERSION 3.10)
set(CMAKE_TOOLCHAIN_FILE /home/yzz/Softerware/vcpkg/scripts/buildsystems/vcpkg.cmake)
project(DAQ_LOG)
aux_source_directory(src LOG_SRC)

find_package(CURL CONFIG REQUIRED)
find_package(czmq CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS fiber thread filesystem)
find_package(unofficial-concurrentqueue CONFIG REQUIRED)

add_library(sylar_log SHARED ${LOG_SRC})

target_include_directories(
	sylar_log  PUBLIC
	include
	${Boost_INCLUDE_DIRS}
	)

target_link_libraries(
	sylar_log PRIVATE
	czmq-static
	jsoncpp_lib
	CURL::libcurl
	unofficial::concurrentqueue::concurrentqueue
	${Boost_LIBRARIES}
	)

add_executable(single_test ./example/singlefile.cpp)
target_link_libraries(single_test  sylar_log)
add_executable(backtrace_test ./example/backtrace_test.cpp)
target_link_libraries(backtrace_test sylar_log)
add_executable(http_log_test ./example/httptest.cpp)
target_link_libraries(http_log_test pthread sylar_log)
add_executable(macro_log_test ./example/macrotest.cpp)
target_link_libraries(macro_log_test pthread sylar_log)
add_executable(json_log_test ./example/json_test.cpp)
target_link_libraries(json_log_test pthread sylar_log)
add_executable(as_log_test ./example/asytest.cpp)
target_link_libraries(as_log_test pthread sylar_log)
add_executable(zmq_log_test ./example/zmqtest.cpp)
target_link_libraries(zmq_log_test pthread sylar_log)
add_executable(std_log_test ./example/stdtest.cpp)
target_link_libraries(std_log_test sylar_log -pthread)
add_executable(fileroll_log_test ./example/filerolltest.cpp)
target_link_libraries(fileroll_log_test sylar_log -pthread)
add_executable(conf_log_test ./example/config_test.cpp)
target_link_libraries(conf_log_test sylar_log -pthread)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set(INC ./include/appender.hpp
	./include/formatter.hpp
	./include/locationinfo.hpp
	./include/logevent.hpp
	./include/logger.hpp
	./include/loggerfactory.hpp
	./include/loglevel.hpp
	)

install(FILES ${INC} DESTINATION ${PROJECT_SOURCE_DIR}/include/)
