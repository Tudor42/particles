cmake_minimum_required(VERSION 3.14)
project(particle_simulator)

# Specify the source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw GLEW::GLEW imgui::imgui Eigen3::Eigen assimp::assimp)