cmake_minimum_required(VERSION 3.6.2)
project(dslink-cpp-system)

# add sources
set(SOURCES main.cc
		        cmdline.cc
				utils/linenoise.c
		        command_factory.cc
		        commands/core/command.cc
		        commands/core/command_data.cc
		        commands/command_help.cc
		        commands/command_list.cc
		        commands/command_cd.cc
		        commands/command_set.cc
		        commands/command_timeout.cc
		        commands/command_invoke.cc
		        commands/command_subscribe.cc
		        commands/command_quit.cc)

add_executable(${TARGET_NAME} ${SOURCES})

target_link_libraries(${TARGET_NAME} ${DSA_SDK_LIBRARY})
target_link_libraries(${TARGET_NAME} ${Boost_LIBRARIES})
target_link_libraries(${TARGET_NAME} ${OPENSSL_LIBRARIES})
if (UNIX)
	target_link_libraries(${TARGET_NAME} dl pthread)
endif ()

if (NOT PREBUILT_SDK_PATH)
	add_dependencies(${TARGET_NAME} ${DSA_SDK_LIBRARY})
endif ()
