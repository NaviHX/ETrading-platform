aux_source_directory(./src/ SRC_LIST)

include_directories(./include/ ../Filemd5)

add_executable(V3 ${SRC_LIST})

target_link_libraries(V3 Filemd5 crypto ssl)
target_compile_options(V3 PRIVATE -Wall -g -O0)
