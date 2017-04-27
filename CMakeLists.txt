cmake_minimum_required(VERSION 2.8)

project(libsoundbag)

set(CMAKE_CXX_STANDARD 11)

if(DISABLE_SHARED OR NOT DEFINED ENABLE_SHARED)
	set(ENABLE_SHARED OFF)
endif()

include_directories(src/include)

if(ENABLE_SHARED)
	add_library(soundbag SHARED src/soundbag.cpp)
else()
	add_library(soundbag STATIC src/soundbag.cpp)
endif()

file(GLOB HEADERS "src/include/*.hpp")

install(FILES ${HEADERS} DESTINATION include/soundbag)

if(BUILD_TESTS)
	enable_testing()
	add_subdirectory(test)
endif()