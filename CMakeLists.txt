cmake_minimum_required(VERSION 3.10)
project(psd LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type")
endif()

option(USE_OPENMP "Enable OpenMP support" OFF)

file(GLOB ALL_CPP CONFIGURE_DEPENDS "src/*.cpp")

find_package(Boost REQUIRED COMPONENTS filesystem system)

find_package(NLopt REQUIRED)

if(TARGET NLopt::nlopt)
    set(NLOPT_TARGET NLopt::nlopt)
elseif(TARGET nlopt::nlopt)
    set(NLOPT_TARGET nlopt::nlopt)
elseif(TARGET nlopt)
    set(NLOPT_TARGET nlopt)
else()
    message(FATAL_ERROR "Could not find NLopt target")
endif()

add_executable(${PROJECT_NAME}
    ${ALL_CPP}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<CONFIG:Debug>:-g -O0>
    $<$<CONFIG:Release>:-O3>
)

if(USE_OPENMP)
    find_package(OpenMP REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE USE_OPENMP)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
    Boost::filesystem
    Boost::system
    ${NLOPT_TARGET}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Libraries)
