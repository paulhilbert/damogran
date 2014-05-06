cmake_minimum_required(VERSION 2.8.8)
project(libdamogran)

set (libdamogran_VERSION_MAJOR 0)
set (libdamogran_VERSION_MINOR 1)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/include")
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

if(CMAKE_COMPILER_IS_GNUCXX)
	add_definitions(-fPIC)
	add_definitions(-O3)
	add_definitions(-g)
	add_definitions(-std=c++1y)
endif()

find_package(Eigen)
find_package(Boost)# COMPONENTS system regex filesystem mpi)

file (GLOB_RECURSE obj RELATIVE "${PROJECT_SOURCE_DIR}" "src/*.cpp")
#file (GLOB_RECURSE units RELATIVE "${PROJECT_SOURCE_DIR}" "test/units/*.cpp")
if (Boost_FOUND AND EIGEN_FOUND) # AND Boost_FOUND AND PCL_FOUND AND EIGEN_FOUND)
	include_directories(${EIGEN_INCLUDE_DIRS})
	message(STATUS ${EIGEN_INCLUDE_DIRS})
	include_directories(${Boost_INCLUDE_DIRS})
	add_library(damogran SHARED ${obj})
	target_link_libraries(damogran "dl")
	#add_definitions(-Dprotected=public)
	#add_definitions(-DTESTING)
	#add_executable(test "test/test.cpp" ${units})
	#target_link_libraries(test damogran ${GLEW_LIBRARIES} "dl" "pthread" "unittest")

	# install binary
	install (TARGETS damogran DESTINATION lib)
	# install header
	install (DIRECTORY include/ DESTINATION include/damogran)
endif()

