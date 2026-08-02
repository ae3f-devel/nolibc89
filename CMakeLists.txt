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

ae2f_CoreLibFetch(ae2f Core main)

ae2f_CoreLibTentConfigCustom(
	nolibc89-common INTERFACE inc ae3f
	${PROJECT_SOURCE_DIR}/cfg/syscall.cmake.in
	)

add_library(ae3f::nolibc89 ALIAS ae3f-nolibc89-common)

if(LINUX)
	if(NOT EXISTS ${ae2f_ProjRoot}/${ae2f_submod}/torvalds/linux)
		execute_process(
			COMMAND git clone https://github.com/torvalds/linux
			${ae2f_ProjRoot}/${ae2f_submod}/torvalds/linux --depth 1
			)
	endif()

	target_include_directories(
		ae3f-nolibc89-common
		INTERFACE	$<BUILD_INTERFACE:${ae2f_ProjRoot}/${ae2f_submod}/torvalds/linux/tools/include>
		INTERFACE	$<BUILD_INTERFACE:${ae2f_ProjRoot}/${ae2f_submod}/torvalds/linux/tools/include/nolibc>
		)
endif()

target_link_libraries(
	ae3f-nolibc89-common
	INTERFACE	ae2f::Core
	)


macro(mknolibc89 name os)
	if(${os})
		file(GLOB_RECURSE source ${PROJECT_SOURCE_DIR}/lib/${os}/${name}/*)
		file(GLOB_RECURSE source_c ${PROJECT_SOURCE_DIR}/lib/${os}/${name}.c)

		message(STATUS "\n"
			"nolibc89::${os}::${name}\n"
			"${source}\n"
			"${source_c}"
			)

		ae2f_CoreLibTentConfigCustom(
			${name} STATIC inc2/${os}/ nolibc89-${os}
			${PROJECT_SOURCE_DIR}/cfg/syscall.cmake.in
			${source}
			${source_c}
			)

		target_link_libraries(nolibc89-${os}-${name}	PRIVATE
			ae3f::nolibc89-common)
		target_compile_options(nolibc89-${os}-${name}	BEFORE PRIVATE
			"-ffreestanding" "-nostdlib" "-fno-stack-protector" "-Wno-error")
		target_link_options(nolibc89-${os}-${name}	PRIVATE
			"-ffreestanding" "-nostdlib" "-fno-stack-protector" "-Wno-error")
		if(OFF)
			set_target_properties(nolibc89-${os}-${name} PROPERTIES
				C_STANDARD 11
				C_STANDARD_REQUIRED ON
				C_EXTENSIONS ON
				)
		endif()

		add_library(nolibc89::${os}::${name}	ALIAS nolibc89-${os}-${name})
		add_library(nolibc89::${name}		ALIAS nolibc89-${os}-${name})

		target_link_libraries(nolibc89-${os}-${name} INTERFACE ae3f-nolibc89-common)
	endif()
endmacro()

macro(fallnolibc89 name)
	if(NOT TARGET nolibc89::${name})
		file(GLOB_RECURSE source ${PROJECT_SOURCE_DIR}/lib/POSIX/${name}/*)
		file(GLOB_RECURSE source_c ${PROJECT_SOURCE_DIR}/lib/POSIX/${name}.c)

		ae2f_CoreLibTentConfigCustom(
			${name} INTERFACE inc2/POSIX nolibc89
			${PROJECT_SOURCE_DIR}/cfg/syscall.cmake.in
			${source} ${source_c}
			)

		target_link_libraries(nolibc89-${name} INTERFACE ae3f-nolibc89-common)
	endif()
endmacro()

macro(cfgnolibc89 name)
	foreach(arg ${ARGN})
		mknolibc89(${name} ${arg})
	endforeach()

	fallnolibc89(${name})
endmacro()

# list of supports
cfgnolibc89(syscall	LINUX)
cfgnolibc89(crt		WIN32 LINUX)
cfgnolibc89(clone	LINUX)
cfgnolibc89(futex)
