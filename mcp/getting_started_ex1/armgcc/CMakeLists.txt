INCLUDE(CMakeForceCompiler)

# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

# THE VERSION NUMBER
SET (Tutorial_VERSION_MAJOR 1)
SET (Tutorial_VERSION_MINOR 0)

# ENABLE ASM
ENABLE_LANGUAGE(ASM)

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)


# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})

# DEBUG LINK FILE
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/../platform/devices/MKL46Z4/linker/gcc/MKL46Z256xxx4_flash.ld  -static")

# RELEASE LINK FILE
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/../platform/devices/MKL46Z4/linker/gcc/MKL46Z256xxx4_flash.ld  -static")

# DEBUG ASM FLAGS
SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -g  -mcpu=cortex-m0plus  -mthumb  -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -mapcs  -std=gnu99")

# DEBUG C FLAGS
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g  -O0  -mcpu=cortex-m0plus  -mthumb  -MMD  -MP  -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -mapcs  -std=gnu99")

# DEBUG LD FLAGS
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g  --specs=nano.specs  -lm  -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -mthumb  -mapcs  -Xlinker --gc-sections  -Xlinker -static  -Xlinker -z  -Xlinker muldefs  -Xlinker --defsym=__stack_size__=0x2000  -Xlinker --defsym=__heap_size__=0x2000")

# RELEASE ASM FLAGS
SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mcpu=cortex-m0plus  -mthumb  -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -mapcs  -std=gnu99")

# RELEASE C FLAGS
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os  -mcpu=cortex-m0plus  -mthumb  -MMD  -MP  -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -mapcs  -std=gnu99")

# RELEASE LD FLAGS
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --specs=nano.specs  -lm  -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -mthumb  -mapcs  -Xlinker --gc-sections  -Xlinker -static  -Xlinker -z  -Xlinker muldefs  -Xlinker --defsym=__stack_size__=0x2000  -Xlinker --defsym=__heap_size__=0x2000")

# ASM MACRO
SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG}  -DDEBUG")

# C MACRO
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}  -DDEBUG")
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}  -DCPU_MKL46Z256VLL4")
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}  -DFRDM_KL46Z")
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}  -DFREEDOM")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}  -DNDEBUG")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}  -DCPU_MKL46Z256VLL4")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}  -DFRDM_KL46Z")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}  -DFREEDOM")

# CXX MACRO

# INCLUDE_DIRECTORIES
IF(CMAKE_BUILD_TYPE MATCHES Debug)
    include_directories(${ProjDirPath}/../platform/osa/inc)
    include_directories(${ProjDirPath}/../platform/utilities/inc)
    include_directories(${ProjDirPath}/../platform/CMSIS/Include)
    include_directories(${ProjDirPath}/../platform/devices)
    include_directories(${ProjDirPath}/../platform/devices/MKL46Z4/include)
    include_directories(${ProjDirPath}/../platform/devices/MKL46Z4/startup)
    include_directories(${ProjDirPath}/../platform/hal/inc)
    include_directories(${ProjDirPath}/../platform/drivers/inc)
    include_directories(${ProjDirPath}/../platform/system/inc)
    include_directories(${ProjDirPath}/..)
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
    include_directories(${ProjDirPath}/../platform/osa/inc)
    include_directories(${ProjDirPath}/../platform/utilities/inc)
    include_directories(${ProjDirPath}/../platform/CMSIS/Include)
    include_directories(${ProjDirPath}/../platform/devices)
    include_directories(${ProjDirPath}/../platform/devices/MKL46Z4/include)
    include_directories(${ProjDirPath}/../platform/devices/MKL46Z4/startup)
    include_directories(${ProjDirPath}/../platform/hal/inc)
    include_directories(${ProjDirPath}/../platform/drivers/inc)
    include_directories(${ProjDirPath}/../platform/system/inc)
    include_directories(${ProjDirPath}/..)
ENDIF()

# ADD_EXECUTABLE
ADD_EXECUTABLE(exception_model
    "${ProjDirPath}/../platform/utilities/src/fsl_misc_utilities.c"
    "${ProjDirPath}/../platform/devices/MKL46Z4/startup/gcc/startup_MKL46Z4.S"
    "${ProjDirPath}/../main.c"
    "${ProjDirPath}/../board.c"
    "${ProjDirPath}/../board.h"
    "${ProjDirPath}/../gpio.c"
    "${ProjDirPath}/../gpio.h"
    "${ProjDirPath}/../port.c"
    "${ProjDirPath}/../port.h"
    "${ProjDirPath}/../platform/utilities/src/fsl_debug_console.c"
    "${ProjDirPath}/../platform/utilities/inc/fsl_debug_console.h"
    "${ProjDirPath}/../platform/utilities/src/print_scan.c"
    "${ProjDirPath}/../platform/utilities/src/print_scan.h"
    "${ProjDirPath}/../platform/devices/MKL46Z4/startup/system_MKL46Z4.c"
    "${ProjDirPath}/../platform/devices/startup.c"
    "${ProjDirPath}/../platform/devices/startup.h"
)
SET_TARGET_PROPERTIES(exception_model PROPERTIES OUTPUT_NAME "exception_model.elf")

TARGET_LINK_LIBRARIES(exception_model -Wl,--start-group)
# LIBRARIES
# IF(CMAKE_BUILD_TYPE MATCHES Debug)
#     TARGET_LINK_LIBRARIES(exception_model ${ProjDirPath}/../lib/ksdk_platform_lib/armgcc/KL46Z4/debug/libksdk_platform.a)
# ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
#     TARGET_LINK_LIBRARIES(exception_model ${ProjDirPath}/../lib/ksdk_platform_lib/armgcc/KL46Z4/release/libksdk_platform.a)
# ENDIF()

# SYSTEM LIBRARIES
TARGET_LINK_LIBRARIES(exception_model m)
TARGET_LINK_LIBRARIES(exception_model c)
TARGET_LINK_LIBRARIES(exception_model gcc)
TARGET_LINK_LIBRARIES(exception_model nosys)
TARGET_LINK_LIBRARIES(exception_model -Wl,--end-group)

# MAP FILE
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}  -Xlinker -Map=debug/exception_model.map")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}  -Xlinker -Map=release/exception_model.map")

# BIN AND HEX
ADD_CUSTOM_COMMAND(TARGET exception_model POST_BUILD COMMAND ${CMAKE_OBJCOPY} -Oihex ${EXECUTABLE_OUTPUT_PATH}/exception_model.elf ${EXECUTABLE_OUTPUT_PATH}/exception_model.hex)
ADD_CUSTOM_COMMAND(TARGET exception_model POST_BUILD COMMAND ${CMAKE_OBJCOPY} -Obinary ${EXECUTABLE_OUTPUT_PATH}/exception_model.elf ${EXECUTABLE_OUTPUT_PATH}/exception_model.bin)
