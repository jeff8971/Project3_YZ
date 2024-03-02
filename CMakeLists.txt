# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.12)

# Project's name and specified language as C++
project(Project3_YZ LANGUAGES CXX)

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the output directories for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Include directories from the 'include' directory within the project
include_directories(${CMAKE_SOURCE_DIR}/include)

# Automatically find and configure OpenCV or other libraries if needed
find_package(OpenCV REQUIRED)

# Task 1: Threshold the input video
# Add the executable that uses threshold_process.cpp and thresholding.cpp
add_executable(threshold_process ${CMAKE_SOURCE_DIR} ./src/threshold_process.cpp  ./src/thresholding.cpp)
# Link with OpenCV libraries
target_link_libraries(threshold_process PRIVATE ${OpenCV_LIBS})
# Ensure the OpenCV include directories are available to the target
target_include_directories(threshold_process PRIVATE ${OpenCV_INCLUDE_DIRS})

# Task 2: Clean up the binary image
# Add the executable that uses clean_process.cpp and morphological_filtering.cpp
add_executable(clean_process ${CMAKE_SOURCE_DIR} ./src/clean_process.cpp  ./src/thresholding.cpp  ./src/morphological_filtering.cpp)
# Link with OpenCV libraries
target_link_libraries(clean_process PRIVATE ${OpenCV_LIBS})
# Ensure the OpenCV include directories are available to the target
target_include_directories(clean_process PRIVATE ${OpenCV_INCLUDE_DIRS})

# Task 3: Segment the image into regions
# Add the executable that uses clean_process.cpp and morphological_filtering.cpp
add_executable(segment_process ${CMAKE_SOURCE_DIR} ./src/segment_process.cpp ./src/segment.cpp ./src/thresholding.cpp  ./src/morphological_filtering.cpp)
# Link with OpenCV libraries
target_link_libraries(segment_process PRIVATE ${OpenCV_LIBS})
# Ensure the OpenCV include directories are available to the target
target_include_directories(segment_process PRIVATE ${OpenCV_INCLUDE_DIRS})