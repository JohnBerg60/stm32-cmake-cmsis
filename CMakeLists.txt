cmake_minimum_required(VERSION 3.16)
project(firmware C CXX ASM)

add_definitions(-DSTM32F103xB)

# Set the C/C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(MCU "cortex-m3")
set(CFCOMMON "-mcpu=${MCU} -mthumb --specs=nano.specs --specs=nosys.specs -Wall -fmessage-length=0 -ffunction-sections -fdata-sections")
set(CFRELEASE "-Os")
set(CFDEBUG "-O0 -g")

include(cmake/cmsis.cmake)

file(GLOB SOURCE_FILES CONFIGURE_DEPENDS "src/*.c" "src/*.cpp" "*.s")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CFDEBUG} ${CFCOMMON} -fno-exceptions")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CFDEBUG} ${CFCOMMON}")
set(CMAKE_ASM_FLAGS "${COMMON_COMPILE_FLAGS} -x assembler-with-cpp")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_SOURCE_DIR}/STM32F103C8Tx_FLASH.ld -Wl,--gc-sections")

add_executable(${PROJECT_NAME}.elf ${SOURCE_FILES})

add_custom_command(TARGET "${PROJECT_NAME}.elf" POST_BUILD
        # Build .hex and .bin files
        COMMAND ${OBJCOPY} -Obinary "${PROJECT_NAME}.elf" "${PROJECT_NAME}.bin"
        COMMAND ${OBJCOPY} -Oihex  "${PROJECT_NAME}.elf" "${PROJECT_NAME}.hex"
        COMMENT "Building ${PROJECT_NAME}.bin and ${PROJECT_NAME}.hex"
        
        # Display sizes
        COMMAND ${SIZE} --format=berkeley ${PROJECT_NAME}.elf 
        COMMENT "Invoking: Cross ARM GNU Print Size" )