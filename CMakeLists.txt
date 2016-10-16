cmake_minimum_required(VERSION 2.8.4)
include(FindPkgConfig)

#set global exe name
set(MAIN "movsqr")

set(GLEW_DEFAULT_PREFIX "C:/libraries/GLEW")
set(GLEW_DEFAULT_LIBRARY "C:/libraries/GLEW/lib/Release/x64/")
set(GLEW_DEFAULT_PREFIX "C:/libraries/GLFW")
set(GLFW_DEFAULT_LIBRARY "C:/libraries/GLFW/lib-vc2015/")

set(ALL_PREFIX_PATH ${GLEW_DEFAULT_PREFIX})
list(APPEND ALL_PREFIX_PATH ${GLFW_DEFAULT_PREFIX})

set(ALL_LIBRARY_PATH ${GLEW_DEFAULT_LIBRARY})
list(APPEND ALL_LIBRARY_PATH ${GLFW_DEFAULT_LIBRARY})

project(${MAIN})
#include directory, same for all build types
include_directories(include)

#file GLOB matches files and stores them in the specified variable, in this case, SOURCES
#get main file for build
file(GLOB SOURCES "main.c")

#include sources that are platform specific
if(PSX)
        message(STATUS "PSX BUILD")
        
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin_psx)
        
        add_executable(${MAIN}.cpe ${SOURCES})
        
        set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${MAIN}.exe)
        
        add_custom_command(TARGET ${MAIN}.cpe POST_BUILD COMMAND CPE2XWIN.EXE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${MAIN}.cpe)
        
elseif(MINGW OR MSYS OR CYGWIN OR MSVC)
        message(STATUS "WINDOWS BUILD")
        
       set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin_win)

elseif(APPLE OR WATCOM OR BORLAND)
        message(FATAL_ERROR "CAN NOT BUILD FOR APPLE, WATCOM, BORLAND")
elseif(UNIX)
        message(STATUS "LINUX BUILD")

        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin_nix)
        
else()
        message(FATAL_ERROR "NO BUILD TARGET MATCH")
endif()

if(PSX)
        message(STATUS "LINKING PSX LIBS")
        
        target_compile_options(${MAIN}.cpe PUBLIC -O3 -Dpsx)
        target_link_libraries(${MAIN}.cpe libpad -Xo0x80010000)
else()
        message(STATUS "LINKING OPENGL, GLFW, GLEW LIBS")
        
        if(MSYS OR MSVC)
	  set(CMAKE_PREFIX_PATH ${ALL_PREFIX_PATH})
	  set(CMAKE_LIBRARY_PATH ${ALL_LIBRARY_PATH})
	  
	  find_path(GLFW_INCLUDE_DIRS GLFW/glfw3.h)
	  find_library(GLFW_STATIC_LIBRARIES NAMES glfw3 PATH_SUFFIXES lib64)
	  
	  include_directories(${GLFW_INCLUDE_DIRS})
	  link_directories(${GLFW_DEFAULT_LIBRARY})
        else()
	  find_package(PkgConfig REQUIRED)
	  pkg_search_module(GLFW_STATIC REQUIRED glfw3)
	  link_directories(${GLFW_LIBRARY_DIRS})
        endif()
        
        find_path(GLEW_INCLUDE_DIRS GL/glew.h)
	find_library(GLEW_STATIC_LIBRARY NAMES glew32s PATH_SUFFIXES lib64)
	
	include_directories(${GLEW_INCLUDE_DIRS})
	link_directories(${GLEW_DEFAULT_LIBRARY})
	
	find_package(OpenGL REQUIRED)
	
	message(STATUS ${GLEW_STATIC_LIBRARY})
	message(STATUS ${GLEW_INCLUDE_DIRS})
	message(STATUS ${GLFW_STATIC_LIBRARIES})
	message(STATUS ${GLFW_INCLUDE_DIRS})
	message(STATUS ${OPENGL_LIBRARY})
	
	add_definitions(-DGLEW_STATIC)
        
	add_executable(${MAIN} ${SOURCES})
        
	target_link_libraries(${MAIN} ${OPENGL_LIBRARY} ${GLEW_STATIC_LIBRARY} ${GLFW_STATIC_LIBRARIES})

endif()
