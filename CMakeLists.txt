cmake_minimum_required(VERSION 3.24)
project(miniengine
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for IDEs (CLion, VS Code)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -------------------------------------------------------------
# Configuration & Dependencies Options
# -------------------------------------------------------------
# bgfx / bx / bimg configuration
set(BX_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/bx" CACHE PATH "Location of bx" FORCE)
set(BIMG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/bimg" CACHE PATH "Location of bimg" FORCE)
set(BGFX_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/bgfx" CACHE PATH "Location of bgfx" FORCE)
set(BGFX_BUILD_EXAMPLES OFF CACHE BOOL "Build bgfx examples" FORCE)
set(BGFX_BUILD_TESTS OFF CACHE BOOL "Build bgfx tests" FORCE)
set(BGFX_BUILD_TOOLS ON CACHE BOOL "Build bgfx tools" FORCE)
set(BGFX_INSTALL OFF CACHE BOOL "Install bgfx" FORCE)
set(BGFX_CUSTOM_TARGETS ON CACHE BOOL "Include convenience custom targets" FORCE)
set(BGFX_LIBRARY_TYPE "STATIC" CACHE STRING "Linking type for bgfx" FORCE)

# SDL3 configuration
set(SDL_TEST_LIBRARY OFF CACHE BOOL "Build SDL test library" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "Build SDL examples" FORCE)
set(SDL_SHARED ON CACHE BOOL "Build SDL shared library" FORCE)
set(SDL_STATIC OFF CACHE BOOL "Build SDL static library" FORCE)

# Box2D configuration
set(BOX2D_UNIT_TESTS OFF CACHE BOOL "Build Box2D unit tests" FORCE)
set(BOX2D_SAMPLES OFF CACHE BOOL "Build Box2D samples" FORCE)

# spdlog configuration
set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "Build spdlog examples" FORCE)
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "Build spdlog tests" FORCE)

# miniaudio configuration
set(MINIAUDIO_BUILD_EXAMPLES OFF CACHE BOOL "Build miniaudio examples" FORCE)
set(MINIAUDIO_BUILD_TESTS OFF CACHE BOOL "Build miniaudio tests" FORCE)
set(MINIAUDIO_BUILD_TOOLS OFF CACHE BOOL "Build miniaudio tools" FORCE)

# EnTT configuration
set(ENTT_BUILD_TESTING OFF CACHE BOOL "Build EnTT tests" FORCE)
set(ENTT_BUILD_DOCS OFF CACHE BOOL "Build EnTT docs" FORCE)

# -------------------------------------------------------------
# Add External Dependencies
# -------------------------------------------------------------
message(STATUS "[MiniEngine] Adding external libraries...")

# bgfx.cmake handles bx, bimg, bgfx and tools (shaderc, bin2c)
add_subdirectory(external/bgfx.cmake)

# SDL3
add_subdirectory(external/SDL)

# Box2D
add_subdirectory(external/box2d)

# miniaudio
add_subdirectory(external/miniaudio)

# spdlog
add_subdirectory(external/spdlog)

# EnTT (ECS)
add_subdirectory(external/entt)

# -------------------------------------------------------------
# bgfx Shader Compiler Integration
# -------------------------------------------------------------
set(BGFX_SHADER_INCLUDE_PATH "${BGFX_DIR}/src")
include(${CMAKE_CURRENT_SOURCE_DIR}/external/bgfx.cmake/cmake/bgfxToolUtils.cmake)

set(SHADER_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated/shaders")
file(MAKE_DIRECTORY "${SHADER_OUTPUT_DIR}")

# Find all vertex and fragment shaders in shader/
file(GLOB_RECURSE VERTEX_SHADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/shader/vs_*.sc")
file(GLOB_RECURSE FRAGMENT_SHADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/shader/fs_*.sc")

set(ALL_GENERATED_SHADER_HEADERS "")

if(VERTEX_SHADERS)
    bgfx_compile_shaders(
        SHADERS ${VERTEX_SHADERS}
        TYPE VERTEX
        VARYING_DEF "${CMAKE_CURRENT_SOURCE_DIR}/shader/varying.def.sc"
        OUTPUT_DIR "${SHADER_OUTPUT_DIR}"
        OUT_FILES_VAR COMPILED_VERTEX_HEADERS
        AS_HEADERS
    )
    list(APPEND ALL_GENERATED_SHADER_HEADERS ${COMPILED_VERTEX_HEADERS})
endif()

if(FRAGMENT_SHADERS)
    bgfx_compile_shaders(
        SHADERS ${FRAGMENT_SHADERS}
        TYPE FRAGMENT
        VARYING_DEF "${CMAKE_CURRENT_SOURCE_DIR}/shader/varying.def.sc"
        OUTPUT_DIR "${SHADER_OUTPUT_DIR}"
        OUT_FILES_VAR COMPILED_FRAGMENT_HEADERS
        AS_HEADERS
    )
    list(APPEND ALL_GENERATED_SHADER_HEADERS ${COMPILED_FRAGMENT_HEADERS})
endif()

# Generate shader wrapper headers so each shader can be included simply by its basename
foreach(SHADER_FILE ${VERTEX_SHADERS} ${FRAGMENT_SHADERS})
    get_filename_component(SHADER_NAME_WE ${SHADER_FILE} NAME_WE)
    get_filename_component(SHADER_NAME ${SHADER_FILE} NAME)
    set(WRAPPER_HEADER "${SHADER_OUTPUT_DIR}/${SHADER_NAME_WE}.bin.h")
    file(WRITE "${WRAPPER_HEADER}"
"// Auto-generated shader profile includes for ${SHADER_NAME_WE}
#pragma once
#include <bx/platform.h>
#include \"spirv/${SHADER_NAME}.bin.h\"
#include \"glsl/${SHADER_NAME}.bin.h\"
#include \"essl/${SHADER_NAME}.bin.h\"
")
endforeach()

# Generate global master shader header: miniengine_shaders.hpp
set(MASTER_SHADER_CONTENT
"// Auto-generated master shader definitions for MiniEngine
#pragma once

#include <bgfx/bgfx.h>

#ifndef BGFX_PLATFORM_SUPPORTS_DXBC
#define BGFX_PLATFORM_SUPPORTS_DXBC 0
#endif
#ifndef BGFX_PLATFORM_SUPPORTS_DXIL
#define BGFX_PLATFORM_SUPPORTS_DXIL 0
#endif
#ifndef BGFX_PLATFORM_SUPPORTS_METAL
#define BGFX_PLATFORM_SUPPORTS_METAL 0
#endif
#ifndef BGFX_PLATFORM_SUPPORTS_NVN
#define BGFX_PLATFORM_SUPPORTS_NVN 0
#endif
#ifndef BGFX_PLATFORM_SUPPORTS_PSSL
#define BGFX_PLATFORM_SUPPORTS_PSSL 0
#endif
#ifndef BGFX_PLATFORM_SUPPORTS_WGSL
#define BGFX_PLATFORM_SUPPORTS_WGSL 0
#endif

#include <bgfx/embedded_shader.h>

")

foreach(SHADER_FILE ${VERTEX_SHADERS} ${FRAGMENT_SHADERS})
    get_filename_component(SHADER_NAME_WE ${SHADER_FILE} NAME_WE)
    string(APPEND MASTER_SHADER_CONTENT "#include \"${SHADER_NAME_WE}.bin.h\"\n")
endforeach()

string(APPEND MASTER_SHADER_CONTENT "
namespace miniengine::Shaders {
    static const bgfx::EmbeddedShader s_embeddedShaders[] = {
")

foreach(SHADER_FILE ${VERTEX_SHADERS} ${FRAGMENT_SHADERS})
    get_filename_component(SHADER_NAME_WE ${SHADER_FILE} NAME_WE)
    string(APPEND MASTER_SHADER_CONTENT "        BGFX_EMBEDDED_SHADER(${SHADER_NAME_WE}),\n")
endforeach()

string(APPEND MASTER_SHADER_CONTENT "        BGFX_EMBEDDED_SHADER_END()
    };

    inline bgfx::ShaderHandle createShader(const char* name) {
        return bgfx::createEmbeddedShader(s_embeddedShaders, bgfx::getRendererType(), name);
    }

    inline bgfx::ProgramHandle createProgram(const char* vsName, const char* fsName, bool destroyShaders = true) {
        bgfx::ShaderHandle vsh = createShader(vsName);
        bgfx::ShaderHandle fsh = createShader(fsName);
        return bgfx::createProgram(vsh, fsh, destroyShaders);
    }
} // namespace miniengine::Shaders
")

file(WRITE "${SHADER_OUTPUT_DIR}/miniengine_shaders.hpp" "${MASTER_SHADER_CONTENT}")

# Custom target to trigger shader compilation
add_custom_target(miniengine_shaders
    DEPENDS ${ALL_GENERATED_SHADER_HEADERS}
    COMMENT "[MiniEngine] Compiling bgfx shaders to headers..."
)

# -------------------------------------------------------------
# MiniEngine Target
# -------------------------------------------------------------
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/miniengine/*/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/miniengine/*/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/miniengine/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/miniengine/*.h"
)

add_executable(miniengine
    ${ENGINE_SOURCES}
    ${ALL_GENERATED_SHADER_HEADERS}
)

add_dependencies(miniengine miniengine_shaders)

# Include directories
target_include_directories(miniengine PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${SHADER_OUTPUT_DIR}"
    "${BX_DIR}/include"
    "${BIMG_DIR}/include"
    "${BGFX_DIR}/include"
)

# Link all requested libraries
target_link_libraries(miniengine PRIVATE
    bgfx
    bimg
    bx
    SDL3::SDL3
    box2d
    miniaudio
    spdlog::spdlog
    EnTT::EnTT
)
