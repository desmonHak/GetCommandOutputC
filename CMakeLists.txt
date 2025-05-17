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

include_directories(
        include
        lib/SimpleOfuscator/include
        lib/SimpleOfuscator/LZ77_c/include
)

add_executable(code
        example/code.c
        include/GetCommandOutputC.h
        lib/SimpleOfuscator/include/SimpleOfuscator.h
        lib/SimpleOfuscator/LZ77_c/include/lz77.h
        lib/SimpleOfuscator/LZ77_c/src/lz77.c
        lib/SimpleOfuscator/src/SimpleOfuscator.c
        src/GetCommandOutputC.c
)

add_executable(example_inline
        example_inline.c
        include/GetCommandOutputC.h
        lib/SimpleOfuscator/include/SimpleOfuscator.h
        lib/SimpleOfuscator/LZ77_c/include/lz77.h
        lib/SimpleOfuscator/LZ77_c/src/lz77.c
        lib/SimpleOfuscator/src/SimpleOfuscator.c
        src/GetCommandOutputC.c
)