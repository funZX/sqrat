CMAKE_MINIMUM_REQUIRED(VERSION 3.2.3)

# Only show Debug and Release configurations in Visual Studio.
IF(CMAKE_CONFIGURATION_TYPES)
    SET(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo" CACHE STRING "" FORCE)
ENDIF()

# Use folders to organize the projects and files.
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

PROJECT(sqrat)

# Print the CMake version for debugging.
MESSAGE("-- CMake version: ${CMAKE_VERSION}")

# Our custom cmake modules.
SET(COMP_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Include our custom cmake modules.
SET(CMAKE_MODULE_PATH
    ${COMP_MODULE_PATH}
)

INSTALL(DIRECTORY include/ DESTINATION include)

OPTION(BUILD_TESTS "Build the unit tests." OFF)

IF(BUILD_TESTS)
    # Enable testing.
    ENABLE_TESTING()

    ADD_SUBDIRECTORY(sqrattest)
ENDIF(BUILD_TESTS)
