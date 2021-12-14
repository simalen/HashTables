cmake_minimum_required(VERSION 3.0)
project(HashTables C)

set(CMAKE_C_STANDARD 99)

add_executable(HashTables HashTable.c HashTable.h Person.c Person.h Bucket.h test_HashTable.c)
