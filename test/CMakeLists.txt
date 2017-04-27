cmake_minimum_required(VERSION 2.8)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

foreach( test_name IN ITEMS soundbag bits )
	add_executable(${test_name}_test ${test_name}_test.cpp)
	add_test(NAME ${test_name} COMMAND $<TARGET_FILE:${test_name}_test>)
	target_link_libraries(${test_name}_test ${GTEST_BOTH_LIBRARIES})
endforeach()

target_link_libraries(soundbag_test soundbag)