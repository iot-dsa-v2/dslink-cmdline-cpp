#cmake_minimum_required(VERSION 3.6.2)
#project(cmdline_dslink)
#
#if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
#    set(MACOSX TRUE)
#endif ()
#
### OpenSSL v1.0.2n
#if (MACOSX)
#    set(OPENSSL_ROOT_DIR "/usr/local/Cellar/openssl/1.0.2n")
#    set(OPENSSL_INCLUDE_DIR "/usr/local/Cellar/openssl/1.0.2n/include")
#elseif (UNIX)
#    set(OPENSSL_ROOT_DIR "/usr/local/ssl")
#endif ()
#
#find_package(OpenSSL REQUIRED)
#include_directories(${OPENSSL_INCLUDE_DIR})
#
#if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 5.1)
#    set(CMAKE_CXX_STANDARD 14)
#else ()
#    set(CMAKE_CXX_STANDARD 11)
#endif ()
#
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DSA_DEBUG")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
#
#if (CMAKE_COMPILER_IS_GNUCC AND NOT CYGWIN)
#    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
#    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fstack-protector-all")
#
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-type")
#endif()
#
#add_subdirectory("${CMAKE_SOURCE_DIR}/deps/sdk")
#
#find_package(Boost 1.66 REQUIRED COMPONENTS system thread date_time program_options filesystem)
#include_directories(${Boost_INCLUDE_DIRS})
#set(EXAMPLE_LIBS ${EXAMPLE_LIBS} ${Boost_LIBRARIES})
#
#
#set(EXAMPLE_LIBS ${EXAMPLE_LIBS} ${OPENSSL_LIBRARIES})
#
#include_directories("./deps/sdk/src/sdk")
#include_directories("./deps/sdk/src/broker")
#include_directories("./deps/sdk/include")
#
##
#if (UNIX)
#    set(EXAMPLE_LIBS dsa_sdk ${EXAMPLE_LIBS} dl)
#else ()
#    set(EXAMPLE_LIBS dsa_sdk ${EXAMPLE_LIBS})
#endif ()
#
#include_directories("src")
#include_directories("cmake-build-debug/deps/sdk/deps/jansson-build/include/")
#
#add_executable(cmdline_dslink
#        src/main.cc
#        src/cmdline.cc
#        src/command_factory.cc
#        src/commands/core/command.cc
#        src/commands/core/command_data.cc
#        src/commands/command_help.cc
#        src/commands/command_list.cc
#        src/commands/command_cd.cc
#        src/commands/command_set.cc
#        src/commands/command_timeout.cc
#        src/commands/command_file.cc
#        src/commands/command_filelist.cc
#        src/commands/command_invoke.cc
#        src/commands/command_subscribe.cc
#        src/commands/command_quit.cc)
#
#target_link_libraries(cmdline_dslink ${EXAMPLE_LIBS} dsa_sdk)

cmake_minimum_required(VERSION 3.6.2)

set(TARGET_NAME cmdline-dslink)

# uncomment and write if you want to use prebuilt sdk
#set (PREBUILT_SDK_PATH ${PROJECT_BINARY_DIR}/deps/sdk-dslink-cpp)

if (PREBUILT_SDK_PATH)
    # you can change sourcepaths
    set(PREBUILT_SDK_SOURCE_PATH ${PROJECT_SOURCE_DIR}/deps/sdk)
    set(PREBUILT_SDK_BINARY_PATH ${PREBUILT_SDK_PATH})
endif ()

if (PREBUILT_SDK_SOURCE_PATH)
    set(SDK_PROJECT_SOURCE_DIR ${PREBUILT_SDK_SOURCE_PATH})
else ()
    set(SDK_PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/deps/sdk)
endif ()


#TODO: this path recognition should be improved
if (PREBUILT_SDK_BINARY_PATH)
    set(SDK_PROJECT_BINARY_DIR ${PREBUILT_SDK_BINARY_PATH})

    if (UNIX OR MACOSX)
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            find_library(DSA_SDK_LIBRARY dsa_sdkd ${SDK_PROJECT_BINARY_DIR}/src/sdk)
        elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
            find_library(DSA_SDK_LIBRARY dsa_sdk ${SDK_PROJECT_BINARY_DIR}/src/sdk)
        endif()
    endif(UNIX OR MACOSX)

    if (WIN32)
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            find_library(DSA_SDK_LIBRARY dsa_sdk ${SDK_PROJECT_BINARY_DIR}/src/sdk/Debug)
        elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
            find_library(DSA_SDK_LIBRARY dsa_sdk ${SDK_PROJECT_BINARY_DIR}/src/sdk/Release)
        endif()
    endif (WIN32)

else ()
    set(SDK_PROJECT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/deps/sdk)
    set(DSA_SDK_LIBRARY dsa_sdk)
endif ()

set(DSA_SDK_SOURCE ${SDK_PROJECT_SOURCE_DIR}/src)
set(DSA_SDK_INCLUDE ${SDK_PROJECT_SOURCE_DIR}/include)
set(DSA_SDK_DEPS ${SDK_PROJECT_SOURCE_DIR}/deps)

set(SDK_PROJECT_INCLUDE_DIR ${SDK_PROJECT_SOURCE_DIR}/include)
include(${SDK_PROJECT_SOURCE_DIR}/CMakeListsConfig.txt)

include_directories(SYSTEM ${OPENSSL_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIR})
include_directories(${DSA_SDK_INCLUDE})
include_directories(${DSA_SDK_DEPS}/msgpack/include)
include_directories(${DSA_SDK_DEPS}/jansson-build/include)
include_directories("${DSA_SDK_SOURCE}/sdk")
include_directories("${CMAKE_SOURCE_DIR}/src")
include_directories("${CMAKE_SOURCE_DIR}/include")

add_subdirectory("./src")
if (NOT PREBUILT_SDK_PATH)
    add_subdirectory(${SDK_PROJECT_SOURCE_DIR})
endif ()

if(DSA_INCLUDE_TEST)
    add_subdirectory("./test")
endif()

