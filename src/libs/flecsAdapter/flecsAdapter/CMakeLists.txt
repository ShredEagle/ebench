set(TARGET_NAME flecsAdapter)

set(${TARGET_NAME}_HEADERS
    model.h
    world.h
)

set(${TARGET_NAME}_SOURCES
    world.cpp
)

add_library(${TARGET_NAME}
            ${${TARGET_NAME}_SOURCES}
            ${${TARGET_NAME}_HEADERS}
)

add_library(ad::${TARGET_NAME} ALIAS ${TARGET_NAME})

cmc_target_current_include_directory(${TARGET_NAME})

set_target_properties(${TARGET_NAME} PROPERTIES
                      VERSION "${${PROJECT_NAME}_VERSION}"
)


##
## Dependencies
##

cmc_find_dependencies()

# target_compile_options(${TARGET_NAME}
#     PRIVATE "-fno-omit-frame-pointer" "-fsanitize=address")
# target_link_options(${TARGET_NAME}
#     PRIVATE "-fsanitize=address")

target_link_libraries(${TARGET_NAME}
    PUBLIC
        ad::ebench
        flecs::flecs_static
        ad::math
)


##
## Install
##

install(TARGETS ${TARGET_NAME} EXPORT ${TARGET_NAME}Targets)

include(cmc-install)
# Install the headers preserving the directory hierarchy
cmc_install_with_folders(FILES ${${TARGET_NAME}_HEADERS}
                         DESTINATION include/${TARGET_NAME}/${TARGET_NAME}
)
# Prepare CMake package in both build and install trees
cmc_install_packageconfig(${TARGET_NAME} ${TARGET_NAME}Targets ${PROJECT_NAME}
                          FIND_FILE "CMakeFinds.cmake.in"
                          NAMESPACE ad::
)
