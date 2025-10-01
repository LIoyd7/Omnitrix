# 适合STM32 HAL库 + FreeRTOS的CMakeLists.txt模板

cmake_minimum_required(VERSION 3.22)

# 指定交叉编译工具链
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(CMAKE_SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# 工程设置
project(Omnitrix C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# 编译选项
add_compile_options(-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

# 优化等级
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    add_compile_options(-Os)
else ()
    add_compile_options(-Og -g)
endif ()

# 宏定义
add_definitions(
    -DUSE_HAL_DRIVER
    -DSTM32F411xE
    -DFREERTOS
)

# 头文件路径
include_directories(
    ./Core/Inc
    ./Drivers/STM32F4xx_HAL_Driver/Inc
    ./Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
    ./Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ./Drivers/CMSIS/Include
    ./Middlewares/Third_Party/FreeRTOS/Source/include
    ./Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS
    ./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F   # <--- 加这一行
    ./Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    ./User
)

# 源文件收集
file(GLOB_RECURSE USER_SOURCES
    "Core/Src/*.c"
    "User/*.c"
    "User/*.cpp"
)
file(GLOB_RECURSE HAL_SOURCES
    "Drivers/STM32F4xx_HAL_Driver/Src/*.c"
)
file(GLOB_RECURSE FREERTOS_SOURCES
    "Middlewares/Third_Party/FreeRTOS/Source/*.c"
    "Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/*.c"
)
file(GLOB STARTUP_SOURCES "*.s")

set(ALL_SOURCES
    ${USER_SOURCES}
    ${HAL_SOURCES}
    ${FREERTOS_SOURCES}
    ${STARTUP_SOURCES}
)

# 链接脚本
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F407XX_FLASH.ld)

add_link_options(
    -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
    -Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map
    -u _printf_float
)

# 生成可执行文件
if(ALL_SOURCES)
    add_executable(${PROJECT_NAME} ${ALL_SOURCES})
else()
    message(FATAL_ERROR "No source files found for target ${PROJECT_NAME}")
endif()

# 生成hex和bin文件
set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}> ${HEX_FILE}
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}> ${BIN_FILE}
    COMMENT "生成HEX和BIN文件"
)
# ...existing code...