cmake_minimum_required(VERSION 3.16)
project(dsa_c C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(red_black_tree src/red_black_tree.c)
add_executable(treap src/treap.c)
add_executable(hash_table src/hash_table.c)
add_executable(bdd src/bdd.c)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    foreach(target red_black_tree treap hash_table bdd)
        target_compile_options(${target} PRIVATE -Wall -Wextra)
    endforeach()
endif()
