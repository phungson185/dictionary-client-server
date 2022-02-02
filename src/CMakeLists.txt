include_directories(${PROJECT_SOURCE_DIR}/btree-lib/inc)
link_directories(${PROJECT_SOURCE_DIR}/btree-lib/)

add_executable(init init.c)
target_link_libraries(init bt)