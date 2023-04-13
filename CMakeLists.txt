#include the macros
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Macros.cmake)

# Set a default build type
mighter2d_set_option(CMAKE_BUILD_TYPE Release STRING "Choose the type of build (Debug or Release)")

# Set cmake version
cmake_minimum_required(VERSION 3.17)

# Project name, version and description

project(MIGHTER2D VERSION 0.1.0 DESCRIPTION "A 2D grid-based game engine using C++17")

# MIGHTER2D uses the paths from the cmake GNUInstallDirs module as defaults
# (https://cmake.org/cmake/help/v3.0/module/GNUInstallDirs.html)
include(GNUInstallDirs)

# Install VC++ runtime binaries
set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ${CMAKE_INSTALL_BINDIR}/Runtime)
set(CMAKE_INSTALL_SYSTEM_RUNTIME_COMPONENT MIGHTER2D_Runtime)
include(InstallRequiredSystemLibraries)

# Include the configuration file
include(${PROJECT_SOURCE_DIR}/cmake/Config.cmake)

# Add an option for choosing the build type (shared or static)
if(MIGHTER2D_OS_WINDOWS)
    if(DEFINED BUILD_SHARED_LIBS)
        mighter2d_set_option(MIGHTER2D_SHARED_LIBS ${BUILD_SHARED_LIBS} BOOL "TRUE to build MIGHTER2D as a shared library, FALSE to build it as a static library")
    else()
        mighter2d_set_option(MIGHTER2D_SHARED_LIBS TRUE BOOL "TRUE to build MIGHTER2D as a shared library, FALSE to build it as a static library")
    endif()
endif()

# Add option to build the examples
mighter2d_set_option(MIGHTER2D_BUILD_EXAMPLES FALSE BOOL "TRUE to build the MIGHTER2D examples, FALSE to ignore them")

# Add options to build the tests
mighter2d_set_option(MIGHTER2D_BUILD_TESTS FALSE BOOL "TRUE to build the MIGHTER2D tests")

# Add option to build the documentation
mighter2d_set_option(MIGHTER2D_BUILD_DOC FALSE BOOL "TRUE to generate the API documentation, FALSE to ignore it")

# add an option to let the user specify a custom directory for external libraries installation
mighter2d_set_option(MIGHTER2D_DEPENDENCIES_INSTALL_PREFIX "." PATH "External libraries (SFML, TGUI, Box2D, ...) installation directory")

# Set CXX Standard
mighter2d_set_option(MIGHTER2D_CXX_STANDARD "17" STRING "C++ standard version to build MIGHTER2D with. Possible values: 17 or 20. Projects using MIGHTER2D must use a version equal or higher to this")

# At least C++17 has to be used
if(MIGHTER2D_CXX_STANDARD LESS "17")
    message(FATAL_ERROR "MIGHTER2D_CXX_STANDARD has to be at least 17")
endif()

# Define the install directory for miscellaneous files
if(MIGHTER2D_OS_WINDOWS)
    set(DEFAULT_INSTALL_MISC_DIR .)
endif()

# add an option to let the user specify a custom directory for doc, examples, licence, readme and other miscellaneous files
mighter2d_set_option(MIGHTER2D_MISC_INSTALL_PREFIX "${DEFAULT_INSTALL_MISC_DIR}" PATH "Prefix installation path for miscellaneous files")
mark_as_advanced(MIGHTER2D_MISC_INSTALL_PREFIX)

# Don't Install pkg-config files by default
mighter2d_set_option(MIGHTER2D_INSTALL_PKGCONFIG_FILES FALSE BOOL "TRUE to automatically install pkg-config files so other projects can find MIGHTER2D")

if (MIGHTER2D_INSTALL_PKGCONFIG_FILES)
    mighter2d_set_option(MIGHTER2D_PKGCONFIG_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/${MIGHTER2D_PKGCONFIG_DIR}" PATH "Install directory for MIGHTER2D's pkg-config .pc files")

    configure_file("pkgconfig/mighter2d.pc.in" "pkgconfig/mighter2d.pc" @ONLY)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/pkgconfig/mighter2d.pc" DESTINATION "${MIGHTER2D_PKGCONFIG_INSTALL_PREFIX}")
endif()

# Add an option to choose whether PDB debug symbols should be generated for Visual Studio
if(MIGHTER2D_COMPILER_MSVC)
    mighter2d_set_option(MIGHTER2D_GENERATE_PDB TRUE BOOL "True to generate PDB debug symbols, FALSE otherwise.")
endif()

# Define an option for choosing between static and dynamic C runtime
if(MIGHTER2D_OS_WINDOWS)
    mighter2d_set_option(MIGHTER2D_USE_STATIC_STD_LIBS FALSE BOOL "TRUE to statically link to the standard libraries, FALSE to use them as DLLs. This option has to match with the one from SFML.")

    # The following combination of flags is not valid
    if(MIGHTER2D_SHARED_LIBS AND MIGHTER2D_USE_STATIC_STD_LIBS)
        message(FATAL_ERROR "MIGHTER2D_SHARED_LIBS and MIGHTER2D_USE_STATIC_STD_LIBS cannot be used together")
    endif()

    # for VC++, we can apply it globally by modifying the compiler flags
    if(MIGHTER2D_COMPILER_MSVC AND MIGHTER2D_USE_STATIC_STD_LIBS)
        foreach(flag
                CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
                CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
            if(${flag} MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
            endif()
        endforeach()
    endif()
endif()

# enable project folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

# Disable static linking
if(NOT MIGHTER2D_SHARED_LIBS AND MIGHTER2D_VERSION_MAJOR VERSION_EQUAL 0 AND MIGHTER2D_VERSION_MINOR VERSION_GREATER_EQUAL 1)
    message(FATAL_ERROR "MIGHTER2D 0.1.0 and higher do not support static builds")
endif()

# Link SFML in the same way as MIGHTER2D, unless SFML_STATIC_LIBRARIES is manually specified
if(NOT DEFINED SFML_STATIC_LIBRARIES)
    if(MIGHTER2D_SHARED_LIBS)
        set(SFML_STATIC_LIBRARIES FALSE)
    else()
        set(SFML_STATIC_LIBRARIES TRUE)
    endif()
endif()

# Link TGUI in the same way as MIGHTER2D, unless TGUI_STATIC_LIBRARIES is manually specified
if(NOT DEFINED TGUI_STATIC_LIBRARIES)
    if(MIGHTER2D_SHARED_LIBS)
        set(TGUI_STATIC_LIBRARIES FALSE)
    else()
        set(TGUI_STATIC_LIBRARIES TRUE)
    endif()
endif()

# Add source directory
add_subdirectory(src/Mighter2d)

# Generate the Config.hpp header
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/MIGHTER2D/Config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/MIGHTER2D/Config.h @ONLY)

# Build the documentation when requested
if(MIGHTER2D_BUILD_DOC)
    add_subdirectory(docs)
endif()

# Build the tests if requested
if(MIGHTER2D_BUILD_TESTS)
    if(NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        message(WARNING "MIGHTER2D_BUILD_TESTS is ON but CMAKE_BUILD_TYPE isn't Debug")
    endif()

    add_subdirectory(tests)
endif()

## Set up install rules

# Add version information to folder name
set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}-${PROJECT_VERSION}")

# Install include files
install(DIRECTORY include
        DESTINATION .
        COMPONENT devel
        FILES_MATCHING PATTERN "*.h" PATTERN "*.inl")

if(MIGHTER2D_GENERATE_PDB)
        install(DIRECTORY "${PROJECT_BINARY_DIR}/lib"
        DESTINATION .
        COMPONENT devel
        FILES_MATCHING PATTERN "*.pdb")
endif()

# Install miscellaneous files
install(FILES LICENSE DESTINATION ${MIGHTER2D_MISC_INSTALL_PREFIX})
install(FILES README.md DESTINATION ${MIGHTER2D_MISC_INSTALL_PREFIX})

# Install 3rd-party binaries
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    # Install SFML binaries
    install(DIRECTORY "${SFML_DIR}/../../../bin/"
            DESTINATION ${MIGHTER2D_DEPENDENCIES_INSTALL_PREFIX}/bin/${CMAKE_BUILD_TYPE}
            COMPONENT devel
            FILES_MATCHING PATTERN "*-d-2.dll"
            )

    # Install openAl.dll (Needed in both dynamic and static configurations)
    install(FILES "${SFML_DIR}/../../../bin/openal32.dll" DESTINATION ${MIGHTER2D_DEPENDENCIES_INSTALL_PREFIX}/bin/${CMAKE_BUILD_TYPE})

    # Install TGUI binaries
    install(FILES "${TGUI_DIR}/../../../bin/tgui-d.dll" DESTINATION ${MIGHTER2D_DEPENDENCIES_INSTALL_PREFIX}/bin/${CMAKE_BUILD_TYPE})
else()
    # Install SFML binaries
    install(DIRECTORY "${SFML_DIR}/../../../bin/"
            DESTINATION ${MIGHTER2D_DEPENDENCIES_INSTALL_PREFIX}/bin/${CMAKE_BUILD_TYPE}
            COMPONENT devel
            FILES_MATCHING PATTERN "*.dll"
            )

    # Install TGUI binaries
    install(FILES "${TGUI_DIR}/../../../bin/tgui.dll" DESTINATION ${MIGHTER2D_DEPENDENCIES_INSTALL_PREFIX}/bin/${CMAKE_BUILD_TYPE})
endif()

# Remove unwanted binaries from folder (only release binaries in "Release" and debug binaries in "Debug")
install(CODE [[
    file(GLOB DEBUG_BINARIES_IN_RELEASE_FOLDER "${CMAKE_INSTALL_PREFIX}/bin/Release/*-d-2.dll")
    if (DEBUG_BINARIES_IN_RELEASE_FOLDER)
        file(REMOVE_RECURSE ${DEBUG_BINARIES_IN_RELEASE_FOLDER})
    endif()

    # MIGHTER2D does not use the sfml-network module
    file(REMOVE "${CMAKE_INSTALL_PREFIX}/bin/Release/sfml-network-2.dll")
    file(REMOVE "${CMAKE_INSTALL_PREFIX}/bin/Debug/sfml-network-d-2.dll")
]])