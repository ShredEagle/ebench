add_subdirectory(libs/aunteater/aunteater)
add_subdirectory(libs/ebench/ebench)

add_subdirectory(apps/benchmark)

option (BUILD_tests "Build 'tests' application" true)
if(BUILD_tests)
    #add_subdirectory(apps/tests)
endif()
