cmake_minimum_required(VERSION 4.0)
project(Swarm)

option(SWARM_USE_VULKAN "Use Vulkan backend" OFF)
#option(GFX_USE_DX12   "Use DirectX12 backend" OFF)
#option(GFX_USE_GL     "Use OpenGL backend" OFF)

set(CMAKE_CXX_STANDARD 20)

add_library(Swarm STATIC)
FILE(GLOB_RECURSE COMMON_SRC src/common/*.cpp)
target_sources(Swarm PRIVATE ${COMMON_SRC})

if(SWARM_USE_VULKAN STREQUAL "ON")
    target_compile_definitions(Swarm PUBLIC SWARM_USE_VULKAN)
    FILE(GLOB_RECURSE VULKAN_SRC src/vulkan/*.cpp)
    target_sources(Swarm PRIVATE ${VULKAN_SRC})

    FIND_PACKAGE(Vulkan REQUIRED)
    target_link_libraries(Swarm PRIVATE vulkan)

    add_subdirectory(extern/vk-bootstrap)
    target_link_libraries(Swarm PRIVATE vk-bootstrap)
endif ()

target_include_directories(Swarm PUBLIC include)
target_include_directories(Swarm PRIVATE src/common)