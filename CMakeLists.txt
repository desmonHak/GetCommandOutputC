cmake_minimum_required(VERSION 3.28)
project(GetCommandOutputC C)

# Permitir establecer modo Debug o Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Configuraciones específicas por tipo de compilación
set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG_ENABLE -O0 -Wall -Wpedantic -D_ExceptionHandler")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG  -D_ExceptionHandler")

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(code
        example/code.c
        include/GetCommandOutputC.h
        src/GetCommandOutputC.c)
