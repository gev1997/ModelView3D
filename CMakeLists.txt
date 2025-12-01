cmake_minimum_required(VERSION 3.15)

include(vcpkg/vcpkg.cmake)

project(ModelView3D VERSION 1.0 LANGUAGES CXX)

# -----------------------------
# C++ Standard
# -----------------------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -----------------------------
# Include directories
# -----------------------------
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# -----------------------------
# Source files
# -----------------------------
file(GLOB_RECURSE SOURCES
    src/*.cpp
)

# -----------------------------
# Find packages via vcpkg
# -----------------------------
find_package(glfw3 REQUIRED)
find_package(glad CONFIG REQUIRED)   # glad provides a CMake config via vcpkg
find_package(Eigen3 REQUIRED)
find_package(libigl CONFIG REQUIRED)

# -----------------------------
# Add executable
# -----------------------------
add_executable(ModelView3D ${SOURCES})

# -----------------------------
# Link libraries
# -----------------------------
target_link_libraries(ModelView3D PRIVATE
    glfw
    glad::glad
    Eigen3::Eigen
    igl::igl_stb
    igl::igl_core
)

# -----------------------------
# Optional: Output folder
# -----------------------------
set_target_properties(ModelView3D PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Copy shaders folder
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
)
