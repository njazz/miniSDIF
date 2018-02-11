cmake_minimum_required (VERSION 2.8)

set (CMAKE_CXX_STANDARD 11)

if (APPLE)
	set(CMAKE_MACOSX_RPATH FALSE)
endif()

project(miniSDIF)

set (SOURCES
    ./src/mSDIFFile.cpp
    ./src/mSDIFFrame.cpp
    ./src/mSDIFMatrix.cpp
    ./src/mSDIFTypes.cpp
)

add_library(miniSDIF ${TL_LIB_SOURCES} ${SOURCES})
SET_TARGET_PROPERTIES(miniSDIF PROPERTIES PREFIX "")



