cmake_minimum_required(VERSION 3.9)
project(libmcnbt C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build/cmake")
if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${libmcnbt_BINARY_DIR}/bin)
endif()

OPTION(ENABLE_INSTALL "Enable installing of libraries" ON)

find_package(LibArchive 3.0 REQUIRED)

include(CreatePkgConfigFile)

add_library(mcnbt SHARED src/mcnbt.c src/mcnbt.h src/tree.c src/tree.h src/util.c src/util.h src/parser.c src/walker.c src/serializer.c)
target_link_libraries(mcnbt ${LibArchive_LIBRARIES})

install(FILES src/mcnbt.h DESTINATION include)
install(TARGETS mcnbt LIBRARY DESTINATION lib)