
cmake_minimum_required(VERSION 3.4)
project(uv-cpp-interface)
set(CMAKE_CXX_STANDARD 17)

set(uv_sources
    main.cpp 
    EventLoop.cpp
    EventAsync.cpp

    UvTest.cpp
)

add_executable(uvWrap ${uv_sources})

target_include_directories(uvWrap PRIVATE uv/include)
target_link_directories(uvWrap PRIVATE uv/lib)
target_link_libraries(uvWrap PRIVATE uv)