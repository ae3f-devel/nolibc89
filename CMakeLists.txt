cmake_minimum_required(VERSION 3.25)
enable_testing()

set(ae2f_ProjRoot ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Tree root for project")
set(ae2f_BinRoot ${CMAKE_CURRENT_BINARY_DIR} CACHE STRING "Tree root for project")
set(ae2f_submod  .submod CACHE STRING "Relative path to submodule (subdirectory)")
set(ae2f_Preproc_Wh ${ae2f_ProjRoot}/${ae2f_submod}/ae2f/Preproc)
option(ae2f_TEST	ON ON)

project(nolibc89 VERSION 0)

if(TARGET ae3f-nolibc89-common)
	return()
endif()

# Execute git to fetch CoreScript when repository does not exist
if(NOT EXISTS ${ae2f_Preproc_Wh})
	execute_process(
		COMMAND git clone https://talkischeap.work.gd/ae2f/Preproc
		${ae2f_Preproc_Wh} --depth 1
		)
endif()

# add subdirectory
if(NOT TARGET ae2f::Preproc)
	add_subdirectory(${ae2f_Preproc_Wh} ${ae2f_submod}/ae2f/Preproc)
endif()

if(NOT EXISTS ${ae2f_ProjRoot}/${ae2f_submod}/torvalds/linux)
	execute_process(
		COMMAND git clone https://github.com/torvalds/linux
		${ae2f_ProjRoot}/${ae2f_submod}/torvalds/linux --depth 1
		)
endif()

ae2f_CoreLibFetch(ae2f Core main)

ae2f_CoreLibTentConfigCustom(
	nolibc89-common INTERFACE inc ae3f
	${PROJECT_SOURCE_DIR}/cfg/syscall.cmake.in
	)

add_library(ae3f::nolibc89 ALIAS ae3f-nolibc89-common)

target_include_directories(
	ae3f-nolibc89-common
	INTERFACE	${ae2f_ProjRoot}/${ae2f_submod}/torvalds/linux/tools/include
	INTERFACE	${ae2f_ProjRoot}/${ae2f_submod}/torvalds/linux/tools/include/nolibc
	)

target_link_libraries(
	ae3f-nolibc89-common
	INTERFACE	ae2f::Core
	)


macro(mknolibc89 name)
	if(LINUX)
		file(GLOB_RECURSE source ${PROJECT_SOURCE_DIR}/lib/${name}/*)

		ae2f_CoreLibTentConfigCustom(
			${name} STATIC inc nolibc89
			${PROJECT_SOURCE_DIR}/cfg/syscall.cmake.in
			lib/${name}.c
			${source}
			)
		target_link_libraries(nolibc89-${name} PRIVATE ae3f::nolibc89-common)
		target_compile_options(nolibc89-${name}	BEFORE PRIVATE "-ffreestanding" "-nostdlib" "-fno-stack-protector" "-Wno-error")
		target_link_options(nolibc89-${name}	PRIVATE "-ffreestanding" "-nostdlib" "-fno-stack-protector" "-Wno-error")
		if(OFF)
			set_target_properties(nolibc89-${name} PROPERTIES
				C_STANDARD 11
				C_STANDARD_REQUIRED ON
				C_EXTENSIONS ON
				)
		endif()
	else()
		ae2f_CoreLibTentConfigCustom(
			${name} INTERFACE inc nolibc89
			${PROJECT_SOURCE_DIR}/cfg/syscall.cmake.in
			)
	endif()
endmacro()

# list of supports
mknolibc89(syscall)
mknolibc89(crt)
mknolibc89(clone)
