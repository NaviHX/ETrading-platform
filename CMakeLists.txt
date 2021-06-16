cmake_minimum_required(VERSION 2.8)

project(ETrading-platform)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(Filemd5)

add_subdirectory(V1)
add_subdirectory(V2)
add_subdirectory(V3)
