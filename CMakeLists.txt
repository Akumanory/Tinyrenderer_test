cmake_minimum_required(VERSION 3.13)

project(TinyRenderer
        VERSION 1.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message("")
message("--- CONFIGURATION INFO START ---")
message(" ~ BUILD_TYPE    => " ${CMAKE_BUILD_TYPE})
message(" ~ COMPILER_INFO => " ${CMAKE_CXX_COMPILER_ID} " " ${CMAKE_CXX_COMPILER_VERSION})
message(" ~ COMPILER_PATH => " ${CMAKE_CXX_COMPILER})
message("--- CONFIGURATION INFO END ---")
message("")

add_subdirectory("libs/TgaImage-lib")

add_compile_options(
    # -Werror
    # -Wpaded?
    -Wall
    -Wextra
    -Wpedantic
    -Wcast-align
    -Wcast-qual
    -Wconversion
    -Wctor-dtor-privacy
    -Wenum-compare
    -Wfloat-equal
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual
    -Wredundant-decls
    -Wsign-conversion
    -Wsign-promo)

if(NOT CMAKE_CXX_EXTENSIONS)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

# set(HEADERS
#     include/*.h)

# set(SOURCES
#     source/*.cpp
#     main.cpp)

# NOTE(akumanory): 
# GLOB_RECURSE searces recursevly in given directory for the given pattern
# can be used as an upgrade for 'set' in projects that uses 'include' and 'source' pattern

file(GLOB_RECURSE SOURCES sources/*.cpp main.cpp)
file(GLOB_RECURSE HEADERS includes/*.h)

add_executable(${PROJECT_NAME}
               ${HEADERS}
               ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE TgaImage-lib)
target_include_directories(${PROJECT_NAME} PUBLIC includes)
