file(GLOB TEST_SRC_FILES "*.cpp")

add_executable(PowerliftingTests
        ${TEST_SRC_FILES}
        ../src/Meet.cpp
        ../src/Lifter.cpp
        ../src/Serialization.cpp
)

target_include_directories(PowerliftingTests
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(PowerliftingTests
        PRIVATE
        GTest::gtest_main
)

# Discover and add all GoogleTest tests to CTest
include(GoogleTest)
gtest_discover_tests(PowerliftingTests)