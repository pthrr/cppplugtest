cmake_minimum_required(VERSION 3.25)
project(LinearAmplifierPlugin VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_minimum_required(VERSION 3.25)
project(LinearAmplifierPlugin VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(USE_SCCACHE "Use sccache" ON)

if(USE_SCCACHE)
    find_program(SCCACHE_PROGRAM sccache)
    if(SCCACHE_PROGRAM)
        set(CMAKE_C_COMPILER_LAUNCHER "${SCCACHE_PROGRAM}")
        set(CMAKE_CXX_COMPILER_LAUNCHER "${SCCACHE_PROGRAM}")
        if(NOT DEFINED ENV{SCCACHE_ENDPOINT})
            set(ENV{SCCACHE_ENDPOINT} "nwv-srv:4226")
        endif()
        if(NOT DEFINED ENV{SCCACHE_CACHE})
            set(ENV{SCCACHE_CACHE} "redis")
        endif()
        if(NOT DEFINED ENV{SCCACHE_LOG_LEVEL})
            set(ENV{SCCACHE_LOG_LEVEL} "info")
        endif()
        message(STATUS "sccache endpoint: $ENV{SCCACHE_ENDPOINT}")
        message(STATUS "sccache cache type: $ENV{SCCACHE_CACHE}")
        message(STATUS "sccache log level: $ENV{SCCACHE_LOG_LEVEL}")
    endif()
endif()

include(FetchContent)

FetchContent_Declare(dpf
    GIT_REPOSITORY https://github.com/DISTRHO/DPF.git
    GIT_TAG main
    GIT_SHALLOW TRUE
)
set(DPF_BUILD_EXAMPLES OFF CACHE BOOL "")
set(DPF_BUILD_TESTS OFF CACHE BOOL "")
FetchContent_MakeAvailable(dpf)

option(BUILD_WITH_LOGGING "Build with logging" ON)

if(BUILD_WITH_LOGGING)
    FetchContent_Declare(spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.13.0
        GIT_SHALLOW TRUE
    )
    set(SPDLOG_INSTALL OFF CACHE BOOL "")
    set(SPDLOG_COMPILED_LIB OFF CACHE BOOL "Use header-only version")
    FetchContent_MakeAvailable(spdlog)
endif()

option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
    FetchContent_Declare(Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.2
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(Catch2)
endif()

set(PLUGIN_SOURCES
    src/LinearAmplifierPlugin.cpp
    ${dpf_SOURCE_DIR}/distrho/src/DistrhoPlugin.cpp
)

add_library(LinearAmplifierPlugin-vst3 MODULE ${PLUGIN_SOURCES})
target_include_directories(LinearAmplifierPlugin-vst3 PRIVATE
    src
    ${dpf_SOURCE_DIR}/distrho
)
target_compile_definitions(LinearAmplifierPlugin-vst3 PRIVATE DISTRHO_PLUGIN_TARGET_VST3)
set_target_properties(LinearAmplifierPlugin-vst3 PROPERTIES
    OUTPUT_NAME "LinearAmplifierPlugin"
    SUFFIX ".vst3"
    PREFIX ""
)

add_library(LinearAmplifierPlugin-clap MODULE ${PLUGIN_SOURCES})
target_include_directories(LinearAmplifierPlugin-clap PRIVATE
    src
    ${dpf_SOURCE_DIR}/distrho
)
target_compile_definitions(LinearAmplifierPlugin-clap PRIVATE DISTRHO_PLUGIN_TARGET_CLAP)
set_target_properties(LinearAmplifierPlugin-clap PROPERTIES
    OUTPUT_NAME "LinearAmplifierPlugin"
    SUFFIX ".clap"
    PREFIX ""
)

add_library(LinearAmplifierPluginStatic STATIC ${PLUGIN_SOURCES})
target_include_directories(LinearAmplifierPluginStatic PUBLIC
    src
    ${dpf_SOURCE_DIR}/distrho
)

if(BUILD_WITH_LOGGING)
    target_link_libraries(LinearAmplifierPluginStatic PUBLIC spdlog::spdlog_header_only)
    target_compile_definitions(LinearAmplifierPluginStatic PUBLIC PLUGIN_HAS_LOGGING=1)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_executable(plugin_tests test/test_plugin.cpp)
    target_link_libraries(plugin_tests PRIVATE
        Catch2::Catch2WithMain
        LinearAmplifierPluginStatic
    )

    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    include(CTest)
    include(Catch)
    catch_discover_tests(plugin_tests)
endif()

set(CPACK_PACKAGE_NAME "LinearAmplifierPlugin")
set(CPACK_GENERATOR "ZIP")

install(TARGETS LinearAmplifierPlugin-vst3 LinearAmplifierPlugin-clap
    LIBRARY DESTINATION .
)

include(CPack)
