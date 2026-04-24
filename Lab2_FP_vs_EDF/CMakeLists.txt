# Generated CMake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()

# ====================================================================================
set(PICO_BOARD pico_w)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Pull in Raspberry Pi Pico SDK
include(pico_sdk_import.cmake)

# Pull in your custom FreeRTOS Kernel
include("$ENV{FREERTOS_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake")

# set project name (must match your main file)
set(PROJECTNAME lab2)
project(${PROJECTNAME} C CXX ASM)

include_directories(include)
pico_sdk_init()

# Add executable
add_executable(${PROJECTNAME}
    src/main.c
    src/utils/traces/traces.c
    src/utils/highPrioTask.c
    src/utils/delay.c
    src/utils/tiebreak.c
)

# Include directories
target_include_directories(${PROJECTNAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(${PROJECTNAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/config)

# Link libraries
target_link_libraries(${PROJECTNAME} 
    pico_stdlib 
    pico_cyw43_arch_none
    FreeRTOS-Kernel 
    FreeRTOS-Kernel-Heap4
)

# Extra link for Pico W
if (PICO_CYW43_SUPPORTED)
    target_link_libraries(${PROJECTNAME} 
        pico_cyw43_arch_none
        FreeRTOS-Kernel 
        FreeRTOS-Kernel-Heap4
    )
endif()

# Enable stdio over USB, disable UART
pico_enable_stdio_usb(${PROJECTNAME} 1)
pico_enable_stdio_uart(${PROJECTNAME} 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECTNAME})
