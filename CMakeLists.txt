cmake_minimum_required(VERSION 3.14)

# Enable latest C++ features like std::filesystem::absolute()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(fractales)

# We need OpenCV
find_package( OpenCV REQUIRED )

# Look for OpenMP in path and add it to compile options
find_package( OpenMP )
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
# set(CMAKE_CXX_FLAGS "-g -Wall")

# add_subdirectory(src)

add_executable(exe main.cpp)

target_link_libraries(
    exe
    PUBLIC ${OpenCV_LIBS}
)

# target_link_libraries(
#     exe
#     PUBLIC libfractales
# )

# target_include_directories(
#     exe
#     PUBLIC src
# )
