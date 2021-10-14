cmake_minimum_required(VERSION 3.15)
project(hello-world-input-plugin)

option(BUILD_TEST "builds the tests" ON)

find_package(mapnik CONFIG REQUIRED)

add_library(hello-world-input-plugin SHARED 
    hello_datasource.cpp
    hello_featureset.cpp
)

target_link_libraries(hello-world-input-plugin PRIVATE mapnik::mapnik)

set_target_properties(hello-world-input-plugin PROPERTIES 
    PREFIX ""
    OUTPUT_NAME "hello"
    SUFFIX ".input"
)

if(BUILD_TEST)
    enable_testing()
    add_subdirectory(test)
endif()

install(TARGETS hello-world-input-plugin RUNTIME DESTINATION bin/plugins)
