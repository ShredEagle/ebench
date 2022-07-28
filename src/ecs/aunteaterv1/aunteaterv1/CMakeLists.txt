set(TARGET_NAME aunteaterv1)

set(${TARGET_NAME}_HEADERS
)

set(${TARGET_NAME}_SOURCES
)

add_executable(${TARGET_NAME}
    main.cpp
    ${${TARGET_NAME}_SOURCES}
    ${${TARGET_NAME}_HEADERS})


find_package(Aunteater REQUIRED CONFIG COMPONENTS aunteater)
find_package(benchmark REQUIRED CONFIG)

target_compile_options(${TARGET_NAME}
    PRIVATE "-fno-omit-frame-pointer")
target_link_libraries(${TARGET_NAME}
    PRIVATE
        ad::aunteater
        ad::aunteaterAdapter
        ad::ebench
        benchmark::benchmark
)

set_target_properties(${TARGET_NAME} PROPERTIES
                      VERSION "${${PROJECT_NAME}_VERSION}"
)


##
## Install
##

install(TARGETS ${TARGET_NAME})

