cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS include/*.h)

add_executable(Visualizer ${SRC_FILES} ${HEADER_FILES})

target_compile_features(Visualizer PRIVATE cxx_std_17)
target_link_libraries(Visualizer PRIVATE SFML::Graphics SFML::Audio)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Visualizer)

target_include_directories(Visualizer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_definitions(Visualizer PRIVATE ASSET_DIR="${CMAKE_SOURCE_DIR}/assets")