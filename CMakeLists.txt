cmake_minimum_required(VERSION 3.30)
project(sdl3-bgfx-starter-c VERSION 1.0 LANGUAGES C CXX)

include(FetchContent)

# Deps
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG main
)

FetchContent_Declare(
    bgfx
    GIT_REPOSITORY https://github.com/bkaradzic/bgfx.cmake
    GIT_TAG master
)

FetchContent_MakeAvailable(SDL3 bgfx)

# Shaders
bgfx_compile_shaders(
  TYPE VERTEX
  SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/vs_basic_triangle.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders
  AS_HEADERS
)

bgfx_compile_shaders(
  TYPE FRAGMENT
  SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/fs_basic_triangle.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders
  AS_HEADERS
)

bgfx_compile_shaders(
  TYPE VERTEX
  SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/vs_basic_quad.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders
  AS_HEADERS
)

bgfx_compile_shaders(
  TYPE FRAGMENT
  SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/fs_basic_quad.sc
  VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc
  OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders
  AS_HEADERS
)

# Add the executable
add_executable(sdl3-bgfx-clear sdl3_bgfx_clear.c)
add_executable(sdl3-bgfx-triangle sdl3_bgfx_triangle.c vs_basic_triangle.sc fs_basic_triangle.sc)
add_executable(sdl3-bgfx-quad sdl3_bgfx_quad.c vs_basic_quad.sc fs_basic_quad.sc)
add_executable(sdl3-renderer sdl3_renderer.c)

target_include_directories(sdl3-bgfx-clear PUBLIC ${SDL3_SOURCE_DIR}/include)
target_include_directories(sdl3-bgfx-triangle PUBLIC ${SDL3_SOURCE_DIR}/include)
target_include_directories(sdl3-bgfx-quad PUBLIC ${SDL3_SOURCE_DIR}/include)
target_include_directories(sdl3-renderer PUBLIC ${SDL3_SOURCE_DIR}/include)
target_include_directories(sdl3-bgfx-clear PUBLIC ${bgfx_SOURCE_DIR}/include)
target_include_directories(sdl3-bgfx-triangle PUBLIC ${bgfx_SOURCE_DIR}/include)
target_include_directories(sdl3-bgfx-quad PUBLIC ${bgfx_SOURCE_DIR}/include)
target_include_directories(sdl3-renderer PUBLIC ${bgfx_SOURCE_DIR}/include)

# Link the libraries
target_link_libraries(sdl3-bgfx-clear PUBLIC SDL3::SDL3 bgfx bx)
target_link_libraries(sdl3-bgfx-triangle PUBLIC SDL3::SDL3 bgfx bx)
target_link_libraries(sdl3-bgfx-quad PUBLIC SDL3::SDL3 bgfx bx)
target_link_libraries(sdl3-renderer PUBLIC SDL3::SDL3)

