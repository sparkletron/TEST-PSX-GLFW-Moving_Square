cmake_minimum_required(VERSION 2.8.4)

#set global exe name
set(MAIN "movsqr")

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

        add_executable(${MAIN} ${SOURCES})
elseif(APPLE OR WATCOM OR BORLAND)
        message(FATAL_ERROR "CAN NOT BUILD FOR APPLE, WATCOM, BORLAND")
elseif(UNIX)
        message(STATUS "LINUX BUILD")

        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin_nix)
        
        add_executable(${MAIN} ${SOURCES})
else()
        message(FATAL_ERROR "NO BUILD TARGET MATCH")
endif()

if(PSX)
        message(STATUS "LINKING PSX LIBS")
        
        target_compile_options(${MAIN}.cpe PUBLIC -O3 -Dpsx)
        target_link_libraries(${MAIN}.cpe libpad -Xo0x80010000)
else()
        message(STATUS "LINKING OPENGL, GLFW, GLEW LIBS")
        
        find_package(glfw3 3.2 REQUIRED)
        find_package(GLEW 1.10 REQUIRED)
        find_package(OpenGL REQUIRED)

	target_link_libraries(${MAIN} ${OPENGL_LIBRARY} glfw GLEW)

endif()
