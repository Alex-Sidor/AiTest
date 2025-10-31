cmake_minimum_required(VERSION 3.10)
project(AiTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(deps/SFML)

file(GLOB SOURCES "src/*.cpp")

add_executable(AiTest ${SOURCES})

target_include_directories(AiTest PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(AiTest
    PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
)
