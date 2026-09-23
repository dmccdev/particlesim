cmake_minimum_required(VERSION 3.20)

# Your project folder is named particlesim
project(particlesim LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tell CMake where Raylib is
add_subdirectory(C:/raylib/raylib raylib)

# Create your executable target

add_executable(particlesim
    src/main.cpp
    src/calibri.ttf
    src/ui.hpp
    src/ui.cpp
    src/button.cpp
    src/button.hpp
    src/pairwise_algorithm/pairwise.hpp
    src/pairwise_algorithm/pairwise.cpp
    src/pairwise_algorithm/ParticlesState.hpp
    src/pairwise_algorithm/ParticlesState.cpp
    src/integrator/Integrator.cpp
    src/integrator/Integrator.hpp
    src/integrator/Verlet.cpp
    src/integrator/Verlet.hpp
    src/integrator/RK4.cpp
    src/integrator/RK4.hpp
    src/integrator/Euler.cpp
    src/integrator/Euler.hpp
)

    # Tell CMake to search inside the 'third_party' folder of this project
target_include_directories(particlesim PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party
    ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Copy the font file to the build directory automatically
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/src/calibri.ttf"
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/src")


# Link Raylib to your program
target_link_libraries(particlesim PRIVATE raylib)

# Find the OpenMP package on your system
find_package(OpenMP REQUIRED)

# Link OpenMP to your specific executable target
if(OpenMP_CXX_FOUND)
    target_link_libraries(particlesim PRIVATE OpenMP::OpenMP_CXX)
endif()



