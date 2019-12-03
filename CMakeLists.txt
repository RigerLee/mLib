cmake_minimum_required(VERSION 3.5)

project(mLib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp -lpthread")

find_package(Eigen3 REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)
include_directories(
    ${EIGEN3_INCLUDE_DIR}
)

add_library(
    mLib
    SHARED
    # add library need .cpp
    ${PROJECT_SOURCE_DIR}/include/mLibCommon.cpp
)

# install to build/install by default
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install" CACHE PATH "..." FORCE)
endif()

# write mLib.cmake.in to build dir
configure_file(${PROJECT_SOURCE_DIR}/src/mLib.cmake.in
  "${PROJECT_BINARY_DIR}/mLibConfig.cmake" @ONLY)

# install libmLib.so
install(
    TARGETS mLib
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
# install headers
install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/include/"
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/mLib
)

# copy mLibConfig.cmake to header dir and lib dir
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/mLibConfig.cmake"
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/mLib
)
