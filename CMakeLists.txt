cmake_minimum_required(VERSION 2.8)

project(libsoundbag)

set(CMAKE_CXX_STANDARD 11)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")
endif()

if(${DISABLE_SHARED})
	set(ENABLE_SHARED OFF)
endif()

if(${ENABLE_SHARED})
	set(LIB_TYPE "SHARED")
else()
	set(LIB_TYPE "STATIC")
endif()

if(WIN32)
	set(CMAKE_SHARED_LIBRARY_PREFIX "")
	set(CMAKE_SHARED_MODULE_PREFIX "")
	set(CMAKE_STATIC_LIBRARY_PREFIX "lib")
endif()

add_library(soundbag ${LIB_TYPE} src/soundbag.cpp)
target_include_directories(soundbag PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/include)

if(ENABLE_SBGL OR NOT DEFINED ENABLE_SBGL)
	find_package(SDL2 REQUIRED)
	find_package(OpenGL REQUIRED)
	find_package(GLEW REQUIRED)
	
	add_library(SDL_GL_Window ${LIB_TYPE} src/SDL_GL_Window.cpp)
	target_include_directories(SDL_GL_Window PUBLIC ${SDL2_INCLUDE_DIRS} ${OpenGL_INCLUDE_DIRS})
	target_link_libraries(SDL_GL_Window PUBLIC soundbag ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})

	add_library(SBGL ${LIB_TYPE} src/SBGL_Dialog.cpp src/SBGL_Text.cpp)
	target_include_directories(SBGL PUBLIC ${SDL2_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${OpenGL_INCLUDE_DIRS} )
	target_link_libraries(SBGL PUBLIC soundbag SDL_ttf ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} )
endif()

if(NOT SOUNDBAG_NO_INSTALL)
	install(TARGETS soundbag SDL_GL_Window
			RUNTIME DESTINATION bin
			LIBRARY DESTINATION lib
			ARCHIVE DESTINATION lib
		)

	install(DIRECTORY src/include/soundbag DESTINATION include)
endif()

if(BUILD_TESTS)
	enable_testing()
	find_package(GTest REQUIRED)
	include_directories(${GTEST_INCLUDE_DIRS})
	
	foreach( test_name IN ITEMS soundbag bits )
		add_executable(${test_name}_test test/${test_name}_test.cpp)
		add_test(NAME ${test_name} COMMAND $<TARGET_FILE:${test_name}_test>)
		target_link_libraries(${test_name}_test ${GTEST_BOTH_LIBRARIES})
	endforeach()
	
	target_link_libraries(soundbag_test soundbag)
endif()