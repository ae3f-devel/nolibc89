cmake_minimum_required(VERSION 3.25)
enable_testing()

set(ae2f_ProjRoot ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Tree root for project")
set(ae2f_BinRoot ${CMAKE_CURRENT_BINARY_DIR} CACHE STRING "Tree root for project")
set(ae2f_submod  .submod CACHE STRING "Relative path to submodule (subdirectory)")
option(ae2f_TEST	ON ON)

project(nolibc89 VERSION 0 LANGUAGES C)

if(TARGET nolibc89::common)
	return()
endif()

set(ae2f_CoreScript_Wh ${ae2f_ProjRoot}/${ae2f_submod}/ae2f/CoreScript)
if(NOT EXISTS ${ae2f_CoreScript_Wh})
	execute_process(
		COMMAND git clone https://talkischeap.work.gd/ae2f/CoreScript
		${ae2f_CoreScript_Wh} --depth 1
		)
endif()

# add subdirectory
if(NOT TARGET ae2f::CoreScript)
	add_subdirectory(${ae2f_CoreScript_Wh} ${ae2f_submod}/ae2f/CoreScript)
endif()
ae2f_CoreLibFetch(ae2f Core main)


file(GLOB_RECURSE source ${PROJECT_SOURCE_DIR}/lib/${TAR_OS}/${TAR_NAME}/*)
file(GLOB_RECURSE source_c ${PROJECT_SOURCE_DIR}/lib/${TAR_OS}/${TAR_NAME}.c)

add_library(tar ${source} ${source_c})
target_include_directories(tar PRIVATE ../inc inc/${TAR_OS})
target_link_libraries(tar PRIVATE ae2f::Core)

set_target_properties(tar PROPERTIES 
    PREFIX ""
    SUFFIX ""
    OUTPUT_NAME tar
)

if(LINUX)
	target_include_directories(tar PRIVATE ${ae2f_ProjRoot}/${ae2f_submod}/torvalds/linux/tools/include/)
endif()
