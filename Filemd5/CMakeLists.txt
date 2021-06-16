add_library(Filemd5 Filemd5.cpp)
add_executable(refresh refresh.cpp)

target_link_libraries(refresh Filemd5 crypto ssl)
target_link_libraries(Filemd5 crypto ssl)
