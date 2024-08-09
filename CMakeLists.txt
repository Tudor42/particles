cmake_minimum_required(VERSION 3.5)
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
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

find_package(OpenMP REQUIRED)
if(OPENMP_FOUND)
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE glfw GLEW::GLEW imgui::imgui glm assimp::assimp)

add_dependencies(${PROJECT_NAME} Ext_NeighborhoodSearch)

target_include_directories(${PROJECT_NAME} PUBLIC ${NEIGHBORHOOD_SEARCH_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC INTERFACE ${NEIGHBORHOOD_SEARCH_LIBRARIES})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)

