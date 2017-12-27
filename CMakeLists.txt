cmake_minimum_required(VERSION 3.6.2)
project(cmdline_dslink)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif ()

if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 5.1)
    set(CMAKE_CXX_STANDARD 14)
else ()
    set(CMAKE_CXX_STANDARD 11)
endif ()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDSA_DEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")

if (CMAKE_COMPILER_IS_GNUCC AND NOT CYGWIN)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fstack-protector-all")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-type")
endif()

add_subdirectory("${CMAKE_SOURCE_DIR}/deps/sdk")

find_package(Boost 1.66 REQUIRED COMPONENTS system thread date_time program_options filesystem)
include_directories(${Boost_INCLUDE_DIRS})
set(EXAMPLE_LIBS ${EXAMPLE_LIBS} ${Boost_LIBRARIES})

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
set(EXAMPLE_LIBS ${EXAMPLE_LIBS} ${OPENSSL_LIBRARIES})

include_directories("./deps/sdk/src/sdk")
include_directories("./deps/sdk/src/broker")
include_directories("./deps/sdk/include")

#
if (UNIX)
    set(EXAMPLE_LIBS dsa_sdk ${EXAMPLE_LIBS} dl)
else ()
    set(EXAMPLE_LIBS dsa_sdk ${EXAMPLE_LIBS})
endif ()

include_directories("src")
add_executable(cmdline_dslink
        src/main.cc
        src/cmdline.cc
        src/commands/command.cc
        src/commands/command_factory.cc
        src/commands/command_unknown.cc
        src/commands/command_list.cc
        src/commands/command_cd.cc
        src/commands/command_subscribe.cc
        src/commands/command_quit.cc)

target_link_libraries(cmdline_dslink ${EXAMPLE_LIBS} dsa_sdk)

