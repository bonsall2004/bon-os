set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Cross compilers
set(CMAKE_C_COMPILER /opt/cross/bin/x86_64-elf-gcc)
set(CMAKE_ASM_NASM_COMPILER nasm)

# IMPORTANT: Prevent CMake from linking a test program
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Kernel flags for the try-compile tests
set(CMAKE_C_FLAGS "-ffreestanding -m64 -nostdlib")
set(CMAKE_EXE_LINKER_FLAGS "-nostdlib")
