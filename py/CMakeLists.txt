cmake_minimum_required (VERSION 2.8)

set (CMAKE_CXX_STANDARD 11)

if (APPLE)
	set(CMAKE_MACOSX_RPATH FALSE)
endif()

project(miniSDIF)

set (SOURCES
    ../src/mSDIFFile.cpp
    ../src/mSDIFFrame.cpp
    ../src/mSDIFMatrix.cpp
    ../src/mSDIFTypes.cpp

		./mini-sdif-py.cpp
)

include_directories(../src)

include(FindPython)
find_package(Python 3 REQUIRED COMPONENTS Development)
include_directories(${Python_INCLUDE_DIRS})

SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined dynamic_lookup")

message("python path:")
message(${Python_INCLUDE_DIRS})

add_library(minisdifpy SHARED ${TL_LIB_SOURCES} ${SOURCES})
SET_TARGET_PROPERTIES(minisdifpy  PROPERTIES PREFIX "")
