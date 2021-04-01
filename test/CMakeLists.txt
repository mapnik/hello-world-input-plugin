Include(FetchContent)

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.19.0") 
    # for cmake 3.19+ https://cmake.org/cmake/help/git-stage/policy/CMP0110.html
    # might not be needed if catch updates its cmakes
    cmake_policy(SET CMP0110 OLD)
endif()


FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v2.13.1)

FetchContent_MakeAvailable(Catch2)

add_executable(input-test unit.cpp)
target_link_libraries(input-test PRIVATE 
    Catch2::Catch2
    mapnik::core 
    mapnik::mapnik
)

include(CTest)
include(${catch2_SOURCE_DIR}/contrib/Catch.cmake)
catch_discover_tests(input-test)
