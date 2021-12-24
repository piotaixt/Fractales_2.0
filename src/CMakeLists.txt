# Create new project
project (libfractales)

# Recursively list all .h and .cc files instead of manually put them here.
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false *.cpp)


# Add all the sources to the library
add_library(${PROJECT_NAME} ${SOURCES})

# We need the OpenCV headers to build the lib
include_directories( ${OpenCV_INCLUDE_DIRS} )
