set(TARGET_NAME benchmark)

set(${TARGET_NAME}_HEADERS
    aunteater_bench.h
)

set(${TARGET_NAME}_SOURCES
    main.cpp
)

source_group(TREE ${CMAKE_CURRENT_LIST_DIR}
             FILES ${${TARGET_NAME}_HEADERS} ${${TARGET_NAME}_SOURCES}
)

add_executable(${TARGET_NAME}
               ${${TARGET_NAME}_HEADERS}
               ${${TARGET_NAME}_SOURCES}
)

cmc_target_current_include_directory(${TARGET_NAME})

target_link_libraries(${TARGET_NAME}
    PRIVATE
        ad::ebench)

set_target_properties(${TARGET_NAME} PROPERTIES
                      VERSION "${${PROJECT_NAME}_VERSION}"
)

install(TARGETS ${TARGET_NAME})
