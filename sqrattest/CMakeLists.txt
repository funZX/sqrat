# Include all build code for external projects.
INCLUDE(${COMP_MODULE_PATH}/external.cmake)

# This is needed for gtest.
FIND_PACKAGE(Threads REQUIRED)

SET(SQRAT_SRCS
    Main.cpp
    Vector.cpp
)

SET(SQRAT_TESTS
    ClassBinding
    ClassInstances
    ClassProperties
    ConstBindings
    FunctionOverload
    ScriptLoading
    SquirrelFunctions
    TableBinding
    FunctionParams
    RunStackHandling
    SuspendVM
    NullPointerReturn
    FuncInputArgumentType
    ArrayBinding
    UniqueObject
)

FOREACH(test ${SQRAT_TESTS})
    # Create a project for the test.
    PROJECT(${test})

    # Generate the test executable.
    ADD_EXECUTABLE(${test} ${SQRAT_SRCS} ${test}.cpp)

    # Add this to the project folder.
    SET_TARGET_PROPERTIES(${ttest} PROPERTIES FOLDER
        "Tests/${test}")

    # Add the sqrat include directory.
    TARGET_INCLUDE_DIRECTORIES(${test} PRIVATE ../include)

    # Link the libraries to the test executable.
    TARGET_LINK_LIBRARIES(${test} gtest squirrel sqstdlib
        ${CMAKE_THREAD_LIBS_INIT})

    # Add the test to CTest.
    ADD_TEST(NAME ${test} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${test}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
ENDFOREACH()
