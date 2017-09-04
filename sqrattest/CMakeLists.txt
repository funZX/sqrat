# Include all build code for external projects.
INCLUDE(${COMP_MODULE_PATH}/external.cmake)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

# This is needed for gtest.
FIND_PACKAGE(Threads REQUIRED)

SET(SQRAT_SRCS
    Main.cpp
    Vector.cpp
)

SET(SQRAT_TESTS
    ArrayBinding
    ClassBinding
    ClassInstances
    ClassProperties
    ConstBindings
    FuncInputArgumentType
    FunctionOverload
    FunctionParams
    NullPointerReturn
    RunStackHandling
    ScriptLoading
    SquirrelFunctions
    SuspendVM
    TableBinding
    UniqueObject
)

FOREACH(test ${SQRAT_TESTS})
    # Create a project for the test.
    PROJECT(${test})

    # TODO: Remove this and always require C++11
    ADD_DEFINITIONS(-DSCRAT_USE_CXX11_OPTIMIZATIONS=1)

    # Generate the test executable.
    ADD_EXECUTABLE(${test} ${SQRAT_SRCS} ${test}.cpp)

    # Add this to the project folder.
    SET_TARGET_PROPERTIES(${test} PROPERTIES FOLDER
        "Tests/${test}")

    # Add the sqrat include directory.
    TARGET_INCLUDE_DIRECTORIES(${test} PRIVATE ../include)

    # Add debug to the tests.
    TARGET_COMPILE_OPTIONS(${test} PRIVATE -g)

    # Link the libraries to the test executable.
    TARGET_LINK_LIBRARIES(${test} gtest squirrel sqstdlib
        ${CMAKE_THREAD_LIBS_INIT})

    # Add the test to CTest.
    ADD_TEST(NAME ${test} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${test}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
ENDFOREACH()
