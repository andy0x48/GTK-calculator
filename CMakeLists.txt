cmake_minimum_required(VERSION 3.31)
project(gtk_calculator C)

set(CMAKE_C_STANDARD 17)

# Find GTK3 package
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

# Include GTK directories
include_directories(${GTK3_INCLUDE_DIRS})

# Link GTK directories
link_directories(${GTK3_LIBRARY_DIRS})

# Add GTK compile flags
add_definitions(${GTK3_CFLAGS_OTHER})

# Create executable
add_executable(gtk_calculator src/main.c
        src/ui.h
        src/ui.c
        src/logic.h
        src/logic.c
)

# Link GTK libraries
target_link_libraries(gtk_calculator ${GTK3_LIBRARIES})

# For Windows: copy DLLs to build directory (optional but helpful)
if(WIN32)
    # This helps with running from CLion directly
    set_target_properties(gtk_calculator PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()
