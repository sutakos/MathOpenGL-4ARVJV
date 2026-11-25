cmake_minimum_required(VERSION 3.10)
project(BezierProject)

set(CMAKE_CXX_STANDARD 17)

# Trouve les packages installés via vcpkg
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

# Ton fichier exécutable
add_executable(BezierProject src/main.cpp)

# Lie les bibliothèques à ton exécutable
target_link_libraries(BezierProject PRIVATE glfw GLEW::GLEW OpenGL::GL)