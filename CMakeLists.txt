cmake_minimum_required(VERSION 3.14)
project(particle_simulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})
set(ExternalInstallDir "${CMAKE_BINARY_DIR}/extern/install" CACHE INTERNAL "")
set(EXT_CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE INTERNAL "")
if (NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(EXT_CMAKE_BUILD_TYPE "Release" CACHE INTERNAL "")
endif()

include(NeighborhoodSearch)

# Specify the source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})
include_directories(${CMAKE_SOURCE_DIR}/src)


find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw GLEW::GLEW imgui::imgui Eigen3::Eigen assimp::assimp)

add_dependencies(${PROJECT_NAME} Ext_NeighborhoodSearch)

target_include_directories(${PROJECT_NAME} PUBLIC ${NEIGHBORHOOD_SEARCH_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC INTERFACE ${NEIGHBORHOOD_SEARCH_LIBRARIES})
