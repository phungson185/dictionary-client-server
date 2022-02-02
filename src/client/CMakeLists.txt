find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

add_definitions(${GTK3_CFLAGS_OTHER} -rdynamic)

include_directories(${GTK3_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)

file(GLOB SOURCES_CLIENT ../client/*.c)

add_executable(client ${SOURCES_CLIENT})
target_link_libraries(client ${GTK3_LIBRARIES})
