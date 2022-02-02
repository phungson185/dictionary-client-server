include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/btree-lib/inc ${PROJECT_SOURCE_DIR}/lib)
link_directories(${PROJECT_SOURCE_DIR}/btree-lib/ ${PROJECT_SOURCE_DIR}/lib/blib/)

include_directories(${PROJECT_SOURCE_DIR})
link_directories(${PROJECT_SOURCE_DIR}/lib/blib)

file(GLOB SOURCES_SERVER *.c ) 

add_executable(server ${SOURCES_SERVER})
target_link_libraries(server fdr bt)