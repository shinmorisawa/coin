cmake_minimum_required(VERSION 3.15)
project(coin VERSION 0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL3 REQUIRED sdl3)

include_directories(${SDL3_INCLUDE_DIRS} include)
link_directories(${SDL3_LIBRARY_DIRS})

add_executable(coin)
target_sources(coin PRIVATE
	src/main.cpp
)

target_link_libraries(coin ${SDL3_LIBRARIES})

target_compile_options(coin PRIVATE ${SDL3_CFLAGS_OTHER})
