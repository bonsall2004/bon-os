cmake_minimum_required(VERSION 3.20)
project(kclib LANGUAGES C)

add_library(kclib STATIC
        src/memory/memcpy.c
        src/memory/memset.c
        src/memory/memmove.c
        src/memory/memcmp.c

        src/string/strlen.c
        src/string/strcpy.c
        src/string/strcpy.c
)

set_source_files_properties(
        src/memory/memcpy.c
        src/memory/memset.c
        src/memory/memmove.c
        src/memory/memcmp.c
        src/string/strlen.c
        src/string/strcpy.c
        PROPERTIES LANGUAGE C
)

target_include_directories(kclib PUBLIC include)

target_compile_options(kclib PRIVATE
        -ffreestanding
        -fno-builtin
        -Wall -Wextra -Werror
)

target_link_options(kclib PRIVATE
        -nostdlib
        -nostartfiles
)

target_compile_definitions(kclib PUBLIC KERNEL_LIBC=1)
