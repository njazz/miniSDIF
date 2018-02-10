cmake_minimum_required (VERSION 2.8)

set (CMAKE_PREFIX_PATH ~/Qt/5.8/clang_64)
set (CMAKE_CXX_STANDARD 11)

if (APPLE)
	set(CMAKE_MACOSX_RPATH FALSE)
endif()

#fixes warning for app name
cmake_policy(SET CMP0037 OLD)

project(miniSDIF)

add_definitions(-DHAVE_UNISTD_H)

set (SOURCES
    ./src/mSDIFFile.cpp
    ./src/mSDIFFrame.cpp
    ./src/mSDIFMatrix.cpp
    ./src/mSDIFTypes.cpp
)

add_library(miniSDIF ${TL_LIB_SOURCES} ${SOURCES})
target_link_libraries(miniSDIF "-undefined dynamic_lookup")
SET_TARGET_PROPERTIES(miniSDIF PROPERTIES PREFIX "")



