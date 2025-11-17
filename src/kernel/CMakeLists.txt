cmake_minimum_required(VERSION 3.16)

project(kernel LANGUAGES C ASM_NASM)

# Include directories
include_directories(
        includes
        includes/misc
        src
)

# Source files
set(C_SOURCES
        src/main.c
        src/serial/serial.c
        src/framebuffer/framebuffer.c
        src/framebuffer/logging.c
        src/misc/kformat.c

)

set(C_ARCH_SOURCES
        src/arch/x86/io.c
        src/arch/x86/gdt/gdt.c
        src/arch/x86/idt/idt.c
        src/arch/x86/exceptions/exceptions.c
)

set(ASM_ARCH_SOURCES
        src/arch/x86/gdt/gdt.asm
        src/arch/x86/idt/isr_stubs.asm
)


# Tell NASM to output elf64 objects
set_source_files_properties(${ASM_ARCH_SOURCES} PROPERTIES
        LANGUAGE ASM_NASM
        NASM_OBJ_FORMAT elf64
)

add_executable(kernel ${C_SOURCES} ${C_ARCH_SOURCES} ${ASM_ARCH_SOURCES})

# ----------------------------------------
# Restrict flags to C ONLY — crucial fix
# ----------------------------------------
target_compile_options(kernel PRIVATE
        $<$<COMPILE_LANGUAGE:C,CXX>:-ffreestanding>
        $<$<COMPILE_LANGUAGE:C,CXX>:-m64>
        $<$<COMPILE_LANGUAGE:C,CXX>:-O0>
        $<$<COMPILE_LANGUAGE:C,CXX>:-Wall>
        $<$<COMPILE_LANGUAGE:C,CXX>:-Wextra>
        $<$<COMPILE_LANGUAGE:C,CXX>:-fno-stack-protector>
        $<$<COMPILE_LANGUAGE:C,CXX>:-fno-stack-check>
        $<$<COMPILE_LANGUAGE:C,CXX>:-mcmodel=kernel>
        $<$<COMPILE_LANGUAGE:C,CXX>:-mno-red-zone>
)

# Linker flags
target_link_options(kernel PRIVATE
        -nostdlib
        -Wl,--build-id=none
        -z max-page-size=0x1000
        -T ${CMAKE_SOURCE_DIR}/src/linker_scripts/link-64-kernel.ld
)

target_link_libraries(kernel PRIVATE kclib)