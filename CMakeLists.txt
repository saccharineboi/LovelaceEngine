# project settings
cmake_minimum_required(VERSION 3.10)
project(LovelaceEngine VERSION 0.1)

# debug mode
option(DEBUG "Enable Debug Mode" ON)

# configure settings
configure_file(${CMAKE_SOURCE_DIR}/src/config.h.in ${CMAKE_SOURCE_DIR}/src/config.h)

# set the directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# c++ settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
list(APPEND CMAKE_CXX_FLAGS "-Wall \
                             -Wextra \
                             -Wno-unused \
                             -Werror \
                             -O \
                             -march=native \
                             -g \
                             -Wcast-align \
                             -Wctor-dtor-privacy \
                             -Wdisabled-optimization \
                             -Wformat=2 \
                             -Wformat-nonliteral \
                             -Wformat-security \
                             -Wformat-y2k \
                             -Wwrite-strings \
                             -Wvariadic-macros \
                             -Wunreachable-code \
                             -Wstack-protector \
                             -Wmissing-noreturn \
                             -Wmissing-format-attribute \
                             -Wmissing-field-initializers \
                             -Winvalid-pch \
                             -Wimport \
                             -Winit-self \
                             -Wlogical-op \
                             -Wmissing-include-dirs \
                             -Woverloaded-virtual \
                             -Wredundant-decls \
                             -Wshadow \
                             -Wsign-promo \
                             -Wstrict-null-sentinel \
                             -Wundef \
                             -Wfloat-equal \
                             -pedantic \
                             -pedantic-errors ")

# add source files
file (
        GLOB_RECURSE SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/*.cpp
)

# add header files
file (
        GLOB_RECURSE HEADER_FILES
        ${CMAKE_SOURCE_DIR}/include/*.h
        ${CMAKE_SOURCE_DIR}/src/*.h
)

# include directories
include_directories (
        "${CMAKE_SOURCE_DIR}/include"
        "${CMAKE_SOURCE_DIR}/src"
)

# define the library
add_library(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# add external libraries
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)

# third party code included within the project
add_library(glad "thirdparty/glad/glad.c")
set(LIBS ${LIBS} glad)

add_library(stb_image "thirdparty/stb/stb_image.cpp")
set(LIBS ${LIBS} stb_image)

# set the external libraries
set(LIBS ${LIBS} OpenGL::OpenGL glfw assimp rt m dl)
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBS})

# start building examples
file (
        GLOB_RECURSE SPONZA_HEADER_FILES
        ${CMAKE_SOURCE_DIR}/examples/sponza/*.h
)
file (
        GLOB_RECURSE SPONZA_SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/examples/sponza/*.cpp
)
set(EXAMPLE_SPONZA sponza)
add_executable(${EXAMPLE_SPONZA} ${SPONZA_HEADER_FILES} ${SPONZA_SOURCE_FILES})
target_link_libraries(${EXAMPLE_SPONZA} PUBLIC ${LIBS} ${PROJECT_NAME})
