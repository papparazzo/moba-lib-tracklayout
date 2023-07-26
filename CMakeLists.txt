cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(moba-lib-tracklayout VERSION 0.2.0)

configure_file(config.h.in config.h)

include(GNUInstallDirs)
add_library(
    moba-lib-tracklayout STATIC

    src/moba/symbol.cpp
)

install(TARGETS moba-lib-tracklayout)

target_include_directories(moba-lib-tracklayout PUBLIC "${PROJECT_BINARY_DIR}")
