cmake_minimum_required(VERSION 3.1)

#Configuration du projet
project(posix4msvc)

#set(EXECUTABLE_OUTPUT_PATH bin${CMAKE_BUILD_TYPE})

#Génération de la liste des fichiers sources
file(GLOB_RECURSE lib_src src/*.def src/*.c src/*.h include/*.h)
list(REMOVE_ITEM lib_src ${CMAKE_CURRENT_SOURCE_DIR}/src/_tdirent.c)


add_library(posix4msvc SHARED
  ${lib_src}
)



target_include_directories(posix4msvc PUBLIC
    include
	src
)



