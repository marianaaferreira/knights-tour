cmake_minimum_required(VERSION 3.20)

project(Passeio VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(passeio
    main.cpp
    src/PasseioDoCavalo.cpp
    src/PasseioParberry.cpp
    src/teste.cpp
    src/monitor.cpp
)

target_link_libraries(passeio psapi)
target_link_options(passeio PRIVATE -static -static-libgcc -static-libstdc++)

target_include_directories(passeio PRIVATE headers)
