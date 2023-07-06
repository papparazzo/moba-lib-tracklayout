cmake_minimum_required(VERSION 3.20)

project(moba-lib-tracklayout VERSION 0.2.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

#configure_file(config.h.in config.h)

include(GNUInstallDirs)
add_library(
    moba-lib-tracklayout STATIC 

    src/moba/symbol.cpp
)

install(TARGETS moba-lib-tracklayout)

#target_include_directories(moba-lib-tracklaout PUBLIC "${PROJECT_BINARY_DIR}")
