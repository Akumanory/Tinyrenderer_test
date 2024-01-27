cmake_minimum_required(VERSION 3.13)

project(TgaImage-lib 
        VERSION 1.0 
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
    # -Werror
    # -Wall
    # -Wextra
    # -Wpedantic
    # -Wcast-align
    # -Wcast-qual
    # -Wconversion
    # -Wctor-dtor-privacy
    # -Wenum-compare
    # -Wfloat-equal
    # -Wnon-virtual-dtor
    # -Wold-style-cast
    # -Woverloaded-virtual
    # -Wredundant-decls
    # -Wsign-conversion
    # -Wsign-promo
)

if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()


set(HEADERS
    tga_image.h)

set(SOURCES
    tga_image.cpp)

add_library(${PROJECT_NAME} STATIC
               ${HEADERS}
               ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC .)
