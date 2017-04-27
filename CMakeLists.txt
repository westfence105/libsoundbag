cmake_minimum_required(VERSION 2.8)

project(libsoundbag)

set(CMAKE_CXX_STANDARD 11)

if(${DISABLE_SHARED})
	set(ENABLE_SHARED OFF)
endif()

include_directories(src/include)

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

if(ENABLE_SBGL OR NOT DEFINED ENABLE_SBGL)
	find_package(SDL2 REQUIRED)
	find_package(OpenGL REQUIRED)
	
	add_library(SDL_GL_Window ${LIB_TYPE} src/SDL_GL_Window.cpp)
	target_include_directories(SDL_GL_Window PUBLIC ${SDL2_INCLUDE_DIRS})
	target_include_directories(SDL_GL_Window PUBLIC ${OpenGL_INCLUDE_DIRS})
	target_link_libraries(SDL_GL_Window PUBLIC ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})
endif()

install(TARGETS soundbag SDL_GL_Window
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
	)

install(DIRECTORY src/include/soundbag DESTINATION include)

if(BUILD_TESTS)
	enable_testing()
	add_subdirectory(test)
endif()