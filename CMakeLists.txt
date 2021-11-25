cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(dict)

set(CMAKE_C_STANDARD 99)
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif (NOT CMAKE_BUILD_TYPE)

include_directories(${PROJECT_SOURCE_DIR}/lib)
link_directories(${PROJECT_SOURCE_DIR}/lib/blib)

include_directories(${PROJECT_SOURCE_DIR}/cmake-btree/inc)
link_directories(${PROJECT_SOURCE_DIR}/cmake-btree/)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER} -rdynamic)

message(STATUS "Include: ${GTK3_INCLUDE_DIRS}\n LIB_DIR: ${GTK3_LIBRARY_DIRS} \n CFLAGS_OTHERS: ${GTK3_CFLAGS_OTHER} \n LIBS: ${GTK3_LIBRARIES}")

add_executable(init_db ${PROJECT_SOURCE_DIR}/src/data/init_db.c)
target_link_libraries(init_db bt)

add_executable(server ${PROJECT_SOURCE_DIR}/src/server.c)
target_link_libraries(server ${GTK3_LIBRARIES} fdr bt)

add_executable(client ${PROJECT_SOURCE_DIR}/src/client.c ${PROJECT_SOURCE_DIR}/src/dict.c)
target_link_libraries(client ${GTK3_LIBRARIES} fdr bt)