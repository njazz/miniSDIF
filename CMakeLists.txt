cmake_minimum_required (VERSION 2.8)

set (CMAKE_OSX_DEPLOYMENT_TARGET "10.10")

set (CMAKE_PREFIX_PATH ~/Qt/5.8/clang_64)
set (CMAKE_CXX_STANDARD 11)
# set (QMAKE_MACOSX_DEPLOYMENT_TARGET 10.10)

if (APPLE)
	set(CMAKE_MACOSX_RPATH FALSE)
endif()

#fixes warning for app name
cmake_policy(SET CMP0037 OLD)

project(miniSDIF)

add_definitions(-DHAVE_UNISTD_H)


set (HEADERS
    
)

set (SOURCES
#    ./src/mSDIF.cpp

    ./src/mSDIFFile.cpp
    ./src/mSDIFFrame.cpp
    ./src/mSDIFMatrix.cpp
)
find_package(Boost 1.47.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
include_directories( ${CMAKE_BINARY_DIR} )

#include_directories(./src)

add_library(miniSDIF ${TL_LIB_SOURCES} ${SOURCES})
target_link_libraries(miniSDIF "-undefined dynamic_lookup")
SET_TARGET_PROPERTIES(miniSDIF PROPERTIES PREFIX "")
#set_target_properties(miniSDIF PROPERTIES SUFFIX ".a")

#install(TARGETS timeline LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/../bin)


