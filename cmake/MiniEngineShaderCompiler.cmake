# cmake/MiniEngineShaderCompiler.cmake
# Compilador customizado de shaders para a MiniEngine sem a flag --Werror embutida.

function(miniengine_compile_shaders)
    set(options AS_HEADERS NO_SOURCE_GROUP)
    set(oneValueArgs TYPE VARYING_DEF OUTPUT_DIR OUT_FILES_VAR)
    set(multiValueArgs SHADERS INCLUDE_DIRS DEFINES PROFILES OPTIONS)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" "${ARGN}")

    if(ARGS_PROFILES)
        set(PROFILES ${ARGS_PROFILES})
    else()
        set(PROFILES spirv 430 300_es)
        if(WIN32 OR MINGW OR MSYS OR CYGWIN)
            list(APPEND PROFILES s_5_0)
        elseif(APPLE)
            list(APPEND PROFILES metal)
        endif()
    endif()

    if(WIN32 OR MINGW OR MSYS OR CYGWIN)
        set(PLATFORM WINDOWS)
    elseif(APPLE)
        set(PLATFORM OSX)
    else()
        set(PLATFORM LINUX)
    endif()

    set(ALL_OUTPUTS "")
    foreach(SHADER_FILE ${ARGS_SHADERS})
        if(NOT ARGS_NO_SOURCE_GROUP)
            source_group("Shaders" FILES "${SHADER_FILE}")
        endif()
        get_filename_component(SHADER_FILE_BASENAME ${SHADER_FILE} NAME)
        get_filename_component(SHADER_FILE_NAME_WE ${SHADER_FILE} NAME_WE)
        get_filename_component(SHADER_FILE_ABSOLUTE ${SHADER_FILE} ABSOLUTE)

        set(OUTPUTS "")
        set(COMMANDS "")
        set(MKDIR_COMMANDS "")
        foreach(PROFILE ${PROFILES})
            _bgfx_get_profile_path_ext(${PROFILE} PROFILE_PATH_EXT)
            _bgfx_get_profile_ext(${PROFILE} PROFILE_EXT)
            if(ARGS_AS_HEADERS)
                set(HEADER_PREFIX .h)
            endif()
            set(OUTPUT ${ARGS_OUTPUT_DIR}/${PROFILE_PATH_EXT}/${SHADER_FILE_BASENAME}.bin${HEADER_PREFIX})
            set(PLATFORM_I ${PLATFORM})
            if(PROFILE STREQUAL "spirv")
                set(PLATFORM_I LINUX)
            endif()
            set(BIN2C_PART "")
            if(ARGS_AS_HEADERS)
                set(BIN2C_PART BIN2C ${SHADER_FILE_NAME_WE}_${PROFILE_EXT})
            endif()

            # Chama _bgfx_shaderc_parse SEM a flag WERROR:
            _bgfx_shaderc_parse(
                CLI
                ${BIN2C_PART}
                ${ARGS_TYPE} ${PLATFORM_I} "$<$<CONFIG:Debug,RelWithDebInfo>:DEBUG>"
                FILE ${SHADER_FILE_ABSOLUTE}
                OUTPUT ${OUTPUT}
                PROFILE ${PROFILE}
                O "$<IF:$<CONFIG:Debug>,0,3>"
                VARYINGDEF ${ARGS_VARYING_DEF}
                INCLUDES ${BGFX_SHADER_INCLUDE_PATH} ${ARGS_INCLUDE_DIRS}
                DEFINES ${ARGS_DEFINES}
            )

            list(APPEND OUTPUTS ${OUTPUT})
            list(APPEND ALL_OUTPUTS ${OUTPUT})
            list(
                APPEND
                MKDIR_COMMANDS
                COMMAND
                ${CMAKE_COMMAND}
                -E
                make_directory
                ${ARGS_OUTPUT_DIR}/${PROFILE_PATH_EXT}
            )
            list(APPEND COMMANDS COMMAND bgfx::shaderc ${CLI})
        endforeach()

        add_custom_command(
            OUTPUT ${OUTPUTS}
            COMMAND ${MKDIR_COMMANDS} ${COMMANDS}
            MAIN_DEPENDENCY ${SHADER_FILE_ABSOLUTE}
            DEPENDS ${ARGS_VARYING_DEF}
        )
    endforeach()

    if(DEFINED ARGS_OUT_FILES_VAR)
        set(${ARGS_OUT_FILES_VAR} ${ALL_OUTPUTS} PARENT_SCOPE)
    endif()
endfunction()
