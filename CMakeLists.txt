cmake_minimum_required(VERSION 3.9)
project(libmcnbt C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS -larchive)

add_library(mcnbt SHARED src/mcnbt.c src/mcnbt.h src/tree.c src/tree.h src/util.c src/util.h src/parser.c src/walker.c src/serializer.c)

install(FILES src/mcnbt.h DESTINATION include)
install(TARGETS mcnbt LIBRARY DESTINATION lib)