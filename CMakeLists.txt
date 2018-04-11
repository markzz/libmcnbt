cmake_minimum_required(VERSION 2.8)
project(libmcnbt C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build/cmake")
if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${libmcnbt_BINARY_DIR}/bin)
endif()

#
# Version - read from 'version' file.
#
file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/build/version _version)
string(REGEX REPLACE
        "^([0-9])[0-9][0-9][0-9][0-9][0-9][0-9][a-z]*$" "\\1" _major ${_version})
string(REGEX REPLACE
        "^[0-9]([0-9][0-9][0-9])[0-9][0-9][0-9][a-z]*$" "\\1" _minor ${_version})
string(REGEX REPLACE
        "^[0-9][0-9][0-9][0-9]([0-9][0-9][0-9])[a-z]*$" "\\1" _revision ${_version})
string(REGEX REPLACE
        "^[0-9][0-9][0-9][0-9][0-9][0-9][0-9]([a-z]*)$" "\\1" _quality ${_version})
set(_version_number ${_major}${_minor}${_revision})
string(REGEX REPLACE "[0]*([^0]*[0-9])$" "\\1" _trimmed_minor ${_minor})
string(REGEX REPLACE "[0]*([^0]*[0-9])$" "\\1" _trimmed_revision ${_revision})
#
set(VERSION "${_major}.${_trimmed_minor}.${_trimmed_revision}${_quality}")
set(LIBMCNBT_VERSION_NUMBER "${_version_number}")
set(LIBMCNBT_VERSION_STRING "${VERSION}")

option(ENABLE_INSTALL "Enable installing of libraries" ON)

find_package(LibArchive 3.0 REQUIRED)

set(ADDITIONAL_LIBS ${LibArchive_LIBRARIES})
include(CreatePkgConfigFile)

add_library(mcnbt SHARED src/mcnbt.c src/mcnbt.h src/tree.c src/tree.h src/util.c src/util.h src/parser.c src/walker.c src/serializer.c)
target_link_libraries(mcnbt ${LibArchive_LIBRARIES})

install(FILES src/mcnbt.h DESTINATION include)
install(TARGETS mcnbt LIBRARY DESTINATION lib)