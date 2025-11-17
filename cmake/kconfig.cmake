
# Create configuration directory
set(CONFIG_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/config)
file(MAKE_DIRECTORY ${CONFIG_OUTPUT_DIR})
#file(GLOB_RECURSE KCONFIG_INPUT Kconfig)
set(KCONFIG_INPUT  ${CMAKE_SOURCE_DIR}/Kconfig)
set(CONFIG_PROJECT ${CMAKE_SOURCE_DIR}/.config)
set(CONFIG_DEFAULT ${CMAKE_SOURCE_DIR}/sdkconfig.defaults)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${CONFIG_PROJECT})
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${CONFIG_DEFAULT})

set(kconfiggen_basecommand
        ${PYTHON_EXECUTABLE}
        ${SCRIPTS_DIR}/kconfiggen.py
        --kconfig ${KCONFIG_INPUT}
        --config ${CONFIG_PROJECT}
        --defaults ${CONFIG_DEFAULT}
)

add_custom_target(menuconfig
        COMMAND ${kconfiggen_basecommand} --menuconfig
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        VERBATIM
        USES_TERMINAL
)

execute_process(
        COMMAND
        ${kconfiggen_basecommand}
        --output cmake ${CONFIG_OUTPUT_DIR}/sdkconfig.cmake
        --output header ${CONFIG_OUTPUT_DIR}/sdkconfig.h
        --output config ${CONFIG_PROJECT}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE config_result
)

if(config_result)
    message(FATAL_ERROR "Failed to run kconfgen. Error ${config_result}")
endif()

# include auto generated sdkconfig cmake
include(${CONFIG_OUTPUT_DIR}/sdkconfig.cmake)
include_directories(${CONFIG_OUTPUT_DIR})