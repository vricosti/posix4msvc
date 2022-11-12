cmake_minimum_required(VERSION 3.1)

#Configuration du projet
project(posix4msvc)

#set(EXECUTABLE_OUTPUT_PATH bin${CMAKE_BUILD_TYPE})

if (NOT DEFINED BUILD_WINDEVBLK)
set(BUILD_WINDEVBLK FALSE CACHE BOOL "Do you build windevblk ?" FORCE)
endif()

file(GLOB_RECURSE lib_src src/*.def src/*.c src/*.h include/*.h)
list(REMOVE_ITEM lib_src ${CMAKE_CURRENT_SOURCE_DIR}/src/_tdirent.c)

if (BUILD_WINDEVBLK)
	include_directories(../windevblk/include)
	add_library(posix4msvc SHARED ${lib_src})
endif()

add_library(posix4msvc STATIC ${lib_src})


add_definitions(-DDLLEXPORTS -DTEST -D__STDC__=1)

target_include_directories(posix4msvc PUBLIC
    include
	include/bsd
	include/sys/arch
	src
)
if (BUILD_WINDEVBLK)
	target_include_directories(posix4msvc PUBLIC
		3rdparty/easyhook/include
	)
endif()

#../3rdparty/easyhook/lib/EasyHook32.lib
#find_library(Shlwapi_LIBPATH Shlwapi)

target_link_libraries(posix4msvc ${Shlwapi_LIBPATH})
	if (BUILD_WINDEVBLK)
		target_link_libraries(posix4msvc windevblk)
endif()

