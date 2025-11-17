cmake_minimum_required(VERSION 3.10)
project(custom_os)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binaries)

set(SCRIPTS_DIR ${CMAKE_SOURCE_DIR}/scripts)
include(${CMAKE_SOURCE_DIR}/cmake/python.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/kconfig.cmake)

add_compile_definitions(
        KERNEL_VERSION="${KERNEL_VERSION}"
        KERNEL_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        KERNEL_VERSION_MINOR=${PROJECT_VERSION_MINOR}
        KERNEL_VERSION_PATCH=${PROJECT_VERSION_PATCH}
        KERNEL_VERSION_BUILD=${PROJECT_VERSION_TWEAK}
)

include_directories(${CMAKE_SOURCE_DIR}/config)
# Build the libc
add_subdirectory(src/libc)

# Build the kernel
add_subdirectory(src/kernel)


# Clean output directory
add_custom_target(clean-binaries ALL
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMENT "Cleaning binaries directory"
)

# Bootable HDD image target
set(OUTPUT_DISK_NAME bootable)
add_custom_target(bootable_image
        COMMAND rm -f ${CMAKE_SOURCE_DIR}/${OUTPUT_DISK_NAME}.hdd
        COMMAND truncate -s 67108864 ${CMAKE_SOURCE_DIR}/${OUTPUT_DISK_NAME}.hdd
        COMMAND sgdisk ${CMAKE_SOURCE_DIR}/${OUTPUT_DISK_NAME}.hdd -n 1:2048 -t 1:ef00
        COMMAND ${CMAKE_SOURCE_DIR}/deps/limine/limine bios-install ${CMAKE_SOURCE_DIR}/${OUTPUT_DISK_NAME}.hdd
        COMMAND mformat -i ${CMAKE_SOURCE_DIR}/${OUTPUT_DISK_NAME}.hdd@@1M
        COMMAND mmd -i ${CMAKE_SOURCE_DIR}/${OUTPUT_DISK_NAME}.hdd@@1M ::/EFI ::/EFI/BOOT ::/boot ::/boot/limine
        COMMAND mcopy -i ${CMAKE_SOURCE_DIR}/${OUTPUT_DISK_NAME}.hdd@@1M ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/kernel ::/boot
        COMMAND mcopy -i ${CMAKE_SOURCE_DIR}/${OUTPUT_DISK_NAME}.hdd@@1M ${CMAKE_SOURCE_DIR}/deps/limine/limine.cfg ${CMAKE_SOURCE_DIR}/deps/limine/limine-bios.sys ::/boot/limine
        COMMAND mcopy -i ${CMAKE_SOURCE_DIR}/${OUTPUT_DISK_NAME}.hdd@@1M ${CMAKE_SOURCE_DIR}/deps/limine/BOOTX64.EFI ::/EFI/BOOT
        COMMENT "Building bootloader and creating bootable HDD"
        DEPENDS kernel
)

# Bootable ISO target
set(OUTPUT_ISO_NAME os.iso)
add_custom_target(bootable_iso
        COMMAND ${CMAKE_COMMAND} -E remove_directory iso_root
        COMMAND ${CMAKE_COMMAND} -E make_directory iso_root/boot/limine
        COMMAND ${CMAKE_COMMAND} -E make_directory iso_root/EFI/BOOT
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/kernel iso_root/boot/
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/deps/limine/limine.conf iso_root/boot/limine/
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/deps/limine/limine-bios.sys iso_root/boot/limine/
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/deps/limine/limine-bios-cd.bin iso_root/boot/limine/
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/deps/limine/limine-uefi-cd.bin iso_root/boot/limine/
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/deps/limine/BOOTX64.EFI iso_root/EFI/BOOT/
        COMMAND xorriso -as mkisofs -R -r -J
        -b boot/limine/limine-bios-cd.bin
        -no-emul-boot -boot-load-size 4 -boot-info-table
        -hfsplus -apm-block-size 2048
        --efi-boot boot/limine/limine-uefi-cd.bin
        -efi-boot-part --efi-boot-image --protective-msdos-label
        iso_root -o ${CMAKE_SOURCE_DIR}/${OUTPUT_ISO_NAME}
        COMMAND ${CMAKE_SOURCE_DIR}/deps/limine/limine bios-install ${CMAKE_SOURCE_DIR}/${OUTPUT_ISO_NAME}
        COMMENT "Creating bootable ISO with Limine"
        DEPENDS kernel
)
