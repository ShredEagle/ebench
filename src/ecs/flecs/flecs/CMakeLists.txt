set(TARGET_NAME flecs_bench)

set(${TARGET_NAME}_HEADERS
)

set(${TARGET_NAME}_SOURCES
)

add_executable(${TARGET_NAME}
    main.cpp
    ${${TARGET_NAME}_SOURCES}
    ${${TARGET_NAME}_HEADERS})


find_package(flecs REQUIRED CONFIG)
find_package(benchmark REQUIRED CONFIG)

# target_compile_options(${TARGET_NAME}
#     PRIVATE "-fno-omit-frame-pointer" "-g" "-fsanitize=leak" "-Wall" "-Wextra" "-fsanitize=address" "-fsanitize=undefined")
# target_link_options(${TARGET_NAME}
#     PRIVATE "-fsanitize=address" "-fsanitize=leak" "-fsanitize=undefined")

target_link_libraries(${TARGET_NAME}
    PRIVATE
        flecs::flecs_static
        ad::flecsAdapter
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

