add_subdirectory(libs/ebench/ebench)


#TODO: make an enum for SELECTED_engine
if(SELECTED_engine STREQUAL "aunteater" )
    add_subdirectory(libs/aunteaterAdapter/aunteaterAdapter)
    add_subdirectory(ecs/aunteaterv1/aunteaterv1)
    #add_subdirectory(apps/tests)
endif()
if(SELECTED_engine STREQUAL "entity" )
    add_subdirectory(libs/entityAdapter/entityAdapter)
    add_subdirectory(ecs/entity/entity)
    #add_subdirectory(apps/tests)
endif()
