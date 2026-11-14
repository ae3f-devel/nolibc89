cmake_minimum_required(VERSION 3.10)
project(nolibc89 VERSION 0)

set(nolibc89_cc clang CACHE STRING "c compiler")

set(ae2f_ProjRoot ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Tree root for project")
set(ae2f_BinRoot ${CMAKE_CURRENT_BINARY_DIR} CACHE STRING "Tree root for project")
set(ae2f_submod  .submod CACHE STRING "Relative path to submodule (subdirectory)")
option(ae2f_TEST	ON ON)

set(SUB_BIN ${PROJECT_BINARY_DIR}/nolibc89-sub)

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

ae2f_CoreLibTentConfigCustom(
	common INTERFACE inc nolibc89
	${PROJECT_SOURCE_DIR}/cfg/syscall.cmake.in
	)

if(LINUX)
	if(NOT EXISTS ${ae2f_ProjRoot}/${ae2f_submod}/torvalds/linux)
		execute_process(
			COMMAND git clone https://github.com/torvalds/linux
			${ae2f_ProjRoot}/${ae2f_submod}/torvalds/linux --depth 1
			)
	endif()
endif()

include(ExternalProject)
function(mknolibc89 name os)
	if(TARGET nolibc89::${name})
		return()
	endif()

	if(${os})
		file(GLOB_RECURSE source ${PROJECT_SOURCE_DIR}/proj/lib/${os}/${name}/*)

		if(EXISTS ${PROJECT_SOURCE_DIR}/proj/lib/${os}/${name}.c)
			set(source_c ${PROJECT_SOURCE_DIR}/proj/lib/${os}/${name}.c)
		else()
			set(source_c )
		endif()

		set(SUB_BIN ${SUB_BIN}/${name}/${os})


		add_custom_command(
			DEPENDS			${PROJECT_SOURCE_DIR}/proj/CMakeLists.txt
			OUTPUT			${SUB_BIN}/CMakeCache.txt
			WORKING_DIRECTORY	${PROJECT_SOURCE_DIR}/proj
			COMMAND
			${CMAKE_COMMAND}
			-S "."
			-B "${SUB_BIN}"
			-DCMAKE_C_COMPILER=${nolibc89_cc}
			-Dae2f_ProjRoot=${ae2f_ProjRoot}
			-Dae2f_submod=${ae2f_submod}
			-Dae2f_host=${ae2f_host}
			-Dae2f_TEST=${ae2f_TEST}
			-DCMAKE_BUILD_TYPE=Release
			-DTAR_NAME=${name}
			-DTAR_OS=${os}
			-DCMAKE_TOOLCHAIN_FILE=${PROJECT_SOURCE_DIR}/cfg/unify.cmake
			)

		add_custom_target(nolibc89-${name}-${os}-cfg DEPENDS ${SUB_BIN}/CMakeCache.txt)

		add_custom_command(
			OUTPUT			${SUB_BIN}/lib/Release/tar${CMAKE_STATIC_LIBRARY_SUFFIX}
			WORKING_DIRECTORY	${SUB_BIN}
			COMMAND			${CMAKE_COMMAND} --build . --config Release
			DEPENDS			${SUB_BIN}/CMakeCache.txt nolibc89-${name}-${os}-cfg
			)

		add_custom_target(nolibc89-${name}-${os}-build
			DEPENDS
			${SUB_BIN}/lib/Release/tar${CMAKE_STATIC_LIBRARY_SUFFIX}
			nolibc89-${name}-${os}-cfg
			)

		ae2f_CoreLibTentConfigCustomInterface(
			${name} STATIC inc nolibc89
			${PROJECT_SOURCE_DIR}/cfg/syscall.cmake.in
			IMPORTED GLOBAL
			)

		set_target_properties(nolibc89-${name} PROPERTIES
			IMPORTED_LOCATION_RELEASE	${SUB_BIN}/lib/Release/tar${CMAKE_STATIC_LIBRARY_SUFFIX}
			IMPORTED_LOCATION		${SUB_BIN}/lib/Release/tar${CMAKE_STATIC_LIBRARY_SUFFIX}
			IMPORTED_GLOBAL			TRUE
			)

		target_include_directories(
			nolibc89-${name}
			INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc/>
			INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/proj/inc/${os}>
			)

		target_link_libraries(nolibc89-${name}
			INTERFACE ae2f::Core
			INTERFACE nolibc89::common
			)

		add_dependencies(nolibc89-${name} nolibc89-${name}-${os}-build)
	endif()
endfunction()

macro(fallnolibc89 name)
	if(NOT TARGET nolibc89::${name})
		file(GLOB_RECURSE source ${PROJECT_SOURCE_DIR}/proj/lib/POSIX/${name}/*)
		file(GLOB_RECURSE source_c ${PROJECT_SOURCE_DIR}/proj/lib/POSIX/${name}.c)

		ae2f_CoreLibTentConfigCustom(
			${name}-POSIX STATIC proj/inc/POSIX nolibc89
			${PROJECT_SOURCE_DIR}/cfg/syscall.cmake.in
			${source} ${source_c} proj/lib/dummy.c
			)

		target_link_libraries(nolibc89-${name}-POSIX PUBLIC nolibc89::common)
		add_library(nolibc89::${name} ALIAS nolibc89-${name}-POSIX)
	endif()
endmacro()

macro(cfgnolibc89 name)
	foreach(arg ${ARGN})
		mknolibc89(${name} ${arg})
	endforeach()

	fallnolibc89(${name})

	if(NOT TARGET nolibc89::${name})
		message(FATAL_ERROR "nolibc89::${name}")
	else()
		message(STATUS "nolibc89::${name}")
	endif()
endmacro()

# list of supports
cfgnolibc89(syscall	LINUX)
cfgnolibc89(crt		WIN32 LINUX)
cfgnolibc89(clone	LINUX)
cfgnolibc89(futex	WIN32 LINUX)
