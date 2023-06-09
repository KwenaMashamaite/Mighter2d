# Macro that helps defining an option
macro(mighter2d_set_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()
    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

# Set the compile options used by all targets
function(mighter2d_set_global_compile_flags target)
    if(MIGHTER2D_COMPILER_MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target}
            PRIVATE
            -Wall
            -Wextra
            -Wshadow
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Wcast-align
            -Wunused
            -Woverloaded-virtual
            -Wpedantic
            -Wdouble-promotion
            -Wformat=2
            )
    endif()

    set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)
    target_compile_features(${target} PUBLIC cxx_std_${MIGHTER2D_CXX_STANDARD})
endfunction()

# Set the appropriate standard library on each platform for the given target
function(mighter2d_set_stdlib target)

    # Apply the MIGHTER2D_USE_STATIC_STD_LIBS option on windows when using GCC (already done earlier when using VC++ which requires a global change)
    if(MIGHTER2D_OS_WINDOWS AND MIGHTER2D_COMPILER_GCC)
        if(MIGHTER2D_USE_STATIC_STD_LIBS AND NOT MIGHTER2D_COMPILER_GCC_TDM)
            target_link_libraries(${target} PRIVATE "-static-libgcc" "-static-libstdc++")
        elseif(NOT MIGHTER2D_USE_STATIC_STD_LIBS AND MIGHTER2D_COMPILER_GCC_TDM)
            target_link_libraries(${target} PRIVATE "-shared-libgcc" "-shared-libstdc++")
        endif()
    endif()
endfunction()

# Generate a MIGHTER2DConfig.cmake file (and associated files)
function(mighter2d_export_target export_name)
    include(CMakePackageConfigHelpers)
    if(CMAKE_VERSION VERSION_LESS 3.11)
        set(CVF_VERSION ${Mighter2d_VERSION_MAJOR}.${Mighter2d_VERSION_MINOR}.${Mighter2d_VERSION_PATCH})
        configure_file("${PROJECT_SOURCE_DIR}/cmake/MIGHTER2DConfigVersion.cmake.in" "${PROJECT_BINARY_DIR}/MIGHTER2DConfigVersion.cmake" @ONLY)
    else()
        write_basic_package_version_file("${PROJECT_BINARY_DIR}/MIGHTER2DConfigVersion.cmake"
        VERSION ${Mighter2d_VERSION_MAJOR}.${Mighter2d_VERSION_MINOR}.${Mighter2d_VERSION_PATCH}
        COMPATIBILITY SameMinorVersion)
    endif()

    if (MIGHTER2D_SHARED_LIBS)
        set(targets_config_filename MIGHTER2DSharedTargets.cmake)
    else()
        set(targets_config_filename MIGHTER2DStaticTargets.cmake)
    endif()

    export(EXPORT ${export_name}
            FILE "${PROJECT_BINARY_DIR}/${targets_config_filename}")

    set(config_package_location ${CMAKE_INSTALL_LIBDIR}/cmake/MIGHTER2D)

    configure_package_config_file("${PROJECT_SOURCE_DIR}/cmake/MIGHTER2DConfig.cmake.in" "${PROJECT_BINARY_DIR}/MIGHTER2DConfig.cmake"
        INSTALL_DESTINATION "${config_package_location}")

    install(EXPORT ${export_name}
            FILE ${targets_config_filename}
            DESTINATION ${config_package_location})

    install(FILES "${PROJECT_BINARY_DIR}/MIGHTER2DConfig.cmake"
            "${PROJECT_BINARY_DIR}/MIGHTER2DConfigVersion.cmake"
            DESTINATION ${config_package_location}
            COMPONENT devel)
endfunction()
