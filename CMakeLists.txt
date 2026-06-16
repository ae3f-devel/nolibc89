cmake_minimum_required(VERSION 3.10)
enable_testing()

set(ae2f_ProjRoot ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Tree root for project")
set(ae2f_BinRoot ${CMAKE_CURRENT_BINARY_DIR} CACHE STRING "Tree root for project")
set(ae2f_submod  .submod CACHE STRING "Relative path to submodule (subdirectory)")
set(ae2f_Preproc_Wh ${ae2f_ProjRoot}/${ae2f_submod}/ae2f/Preproc)
option(ae2f_TEST	ON ON)

project(nolibc89-syscall VERSION 0)

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
	nolibc89-syscall OBJECT inc ae3f
	${PROJECT_SOURCE_DIR}/cfg/syscall.cmake.in
	lib/syscall.c
	)

target_include_directories(
	ae3f-nolibc89-syscall 
	PRIVATE 
	${ae2f_ProjRoot}/${ae2f_submod}/torvalds/linux/tools/include
	)

target_link_libraries(
	ae3f-nolibc89-syscall
	PUBLIC	ae2f::Core
	)

target_compile_options(ae3f-nolibc89-syscall	BEFORE PRIVATE "-ffreestanding" "-nostdlib" "-fno-stack-protector")
