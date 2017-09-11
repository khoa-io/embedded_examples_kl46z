get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR}/.. ABSOLUTE)
get_filename_component(ProjectId ${ProjectId} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

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
ADD_EXECUTABLE(${PROJECT_NAME}
    "${ProjDirPath}/../platform/devices/MKL46Z4/startup/gcc/startup_MKL46Z4.S"
    "${ProjDirPath}/../main.c"
    "${ProjDirPath}/../board.c"
    "${ProjDirPath}/../board.h"
    "${ProjDirPath}/../gpio.c"
    "${ProjDirPath}/../gpio.h"
    "${ProjDirPath}/../port.c"
    "${ProjDirPath}/../port.h"
    "${ProjDirPath}/../uart.c"
    "${ProjDirPath}/../uart.h"
    "${ProjDirPath}/../queue.c"
    "${ProjDirPath}/../queue.h"
    "${ProjDirPath}/../srec_reader.c"
    "${ProjDirPath}/../srec_reader.h"
    "${ProjDirPath}/../platform/devices/MKL46Z4/startup/system_MKL46Z4.c"
    "${ProjDirPath}/../platform/devices/startup.c"
    "${ProjDirPath}/../platform/devices/startup.h"
)
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")

TARGET_LINK_LIBRARIES(${PROJECT_NAME} -Wl,--start-group)
# LIBRARIES
# IF(CMAKE_BUILD_TYPE MATCHES Debug)
#     TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${ProjDirPath}/../lib/ksdk_platform_lib/armgcc/KL46Z4/debug/libksdk_platform.a)
# ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
#     TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${ProjDirPath}/../lib/ksdk_platform_lib/armgcc/KL46Z4/release/libksdk_platform.a)
# ENDIF()

# SYSTEM LIBRARIES
TARGET_LINK_LIBRARIES(${PROJECT_NAME} m)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} c)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} gcc)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} nosys)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} -Wl,--end-group)

# MAP FILE
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}  -Xlinker -Map=debug/${PROJECT_NAME}.map")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}  -Xlinker -Map=release/${PROJECT_NAME}.map")

# BIN AND HEX
ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY} -Oihex ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.elf ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.hex)
ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY} -Obinary ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.elf ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.bin)

# SREC
ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY} -Osrec ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.elf ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.srec)