cmake_minimum_required(VERSION 3.1)

#Configuration du projet
project(posix4msvc)

#set(EXECUTABLE_OUTPUT_PATH bin${CMAKE_BUILD_TYPE})

#Génération de la liste des fichiers sources
file(GLOB_RECURSE lib_src src/*.def src/*.c src/*.h include/*.h)
list(REMOVE_ITEM lib_src ${CMAKE_CURRENT_SOURCE_DIR}/src/_tdirent.c)

include_directories(../windevblk/include)



add_library(
	posix4msvc SHARED
	${lib_src}
)

add_definitions(-DDLLEXPORTS)

target_include_directories(
	posix4msvc PUBLIC
    include
	include/bsd
	include/sys/arch
	3rdparty/easyhook/include
	src
)

#../3rdparty/easyhook/lib/EasyHook32.lib

target_link_libraries(
        posix4msvc
		windevblk
		C:/Developer/msys64/home/Vincent/posix4win/util-linux/posix4msvc/3rdparty/easyhook/lib/EasyHook32.lib
)
