cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMakeFiles/modules/")

PROJECT(openal++)

INCLUDE (CheckIncludeFiles)
include (FindPkgConfig)

# build type
set(CMAKE_BUILD_TYPE Debug)

# compile option (Debug) 
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_DEBUG "-g")

# compile option (Release)
set(CMAKE_CXX_FLAGS_RELEASE "-W -Wall -O3")
set(CMAKE_C_FLAGS_RELEASE "-W -Wall -O3")

#if you don't want the full compiler output, remove the following line
set(CMAKE_VERBOSE_MAKEFILE ON)

set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.42.0 COMPONENTS thread REQUIRED )
if (NOT Boost_FOUND)
    message(SEND_ERROR "Boost not found - skipping building tests")
endif (NOT Boost_FOUND)

#find_package(OpenGL REQUIRED )
#if (NOT OpenGL_FOUND)
#    message(SEND_ERROR "OpenGL not found - skipping building tests")
#endif (NOT OpenGL_FOUND)

pkg_check_modules( GLFW REQUIRED libglfw )
if (NOT GLFW_FOUND)
    message(SEND_ERROR "GLFW not found - skipping building tests")
endif (NOT GLFW_FOUND)

pkg_check_modules( IL REQUIRED IL )
if (NOT IL_FOUND)
    message(SEND_ERROR "DevIL not found - skipping building tests")
endif (NOT IL_FOUND)

pkg_check_modules( ILU REQUIRED ILU )
if (NOT ILU_FOUND)
    message(SEND_ERROR "DevIL Utility not found - skipping building tests")
endif (NOT ILU_FOUND)

find_package(GLEW REQUIRED )
if (NOT GLEW_FOUND)
    message(SEND_ERROR "GLEW not found - skipping building tests")
endif (NOT GLEW_FOUND)

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR}/include ${Boost_INCLUDE_DIRS} ${GLFW_INCLUDE_DIR} ${GLEW_INCLUDE_PATH} ${IL_INCLUDE_DIR} ${ILU_INCLUDE_DIR} )

subdirs( src )
