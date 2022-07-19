add_subdirectory(libs/aunteaterAdapter/aunteaterAdapter)
add_subdirectory(libs/ebench/ebench)

add_subdirectory(ecs/aunteaterv1/aunteaterv1)

option (BUILD_tests "Build 'tests' application" true)
if(BUILD_tests)
    #add_subdirectory(apps/tests)
endif()
