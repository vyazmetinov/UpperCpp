cmake_minimum_required(VERSION 3.10)

project(PC LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    PC
    main.cpp
    blockage_diag/blockage_diag.cpp
    crop_box_filter/crop_box_filter.cpp
)

target_include_directories(PC PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(
    PC
    PRIVATE
    filter.hpp
    Logger.h
    PointCloud.h
    blockage_diag/blockage_diag.hpp
    crop_box_filter/crop_box_filter.hpp
)
