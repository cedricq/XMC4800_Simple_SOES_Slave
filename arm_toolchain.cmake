set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          arm)

# Without that flag CMake is not able to pass test compilation check
set(CMAKE_TRY_COMPILE_TARGET_TYPE   STATIC_LIBRARY)

set(CMAKE_EXECUTABLE_SUFFIX "")
set(BAREMETAL_ARM_TOOLCHAIN_PATH  "")
#set(BAREMETAL_ARM_TOOLCHAIN_PATH  "/home/cedric/soe_workspace/Test_SOES/tool/gcc-arm-none-eabi-8/bin/")
#set(BAREMETAL_ARM_TOOLCHAIN_PATH  "/home/cedric/soe_workspace/Test_SOES/tool/gcc-arm-none-eabi-10/bin/")

#set(CC "/home/cedric/soe_workspace/Test_SOES/tool/gcc-arm-none-eabi-10/bin/arm-none-eabi-gcc)

#set(CMAKE_AR                        ${BAREMETAL_ARM_TOOLCHAIN_PATH}arm-none-eabi-ar${CMAKE_EXECUTABLE_SUFFIX})
#set(CMAKE_ASM_COMPILER              ${BAREMETAL_ARM_TOOLCHAIN_PATH}arm-none-eabi-gcc${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_C_COMPILER                ${BAREMETAL_ARM_TOOLCHAIN_PATH}arm-none-eabi-gcc${CMAKE_EXECUTABLE_SUFFIX})
#set(CMAKE_CXX_COMPILER              ${BAREMETAL_ARM_TOOLCHAIN_PATH}arm-none-eabi-g++${CMAKE_EXECUTABLE_SUFFIX})
#set(CMAKE_LINKER                    ${BAREMETAL_ARM_TOOLCHAIN_PATH}arm-none-eabi-ld${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_OBJCOPY                   ${BAREMETAL_ARM_TOOLCHAIN_PATH}arm-none-eabi-objcopy${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
#set(CMAKE_RANLIB                    ${BAREMETAL_ARM_TOOLCHAIN_PATH}arm-none-eabi-ranlib${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
#set(CMAKE_SIZE                      ${BAREMETAL_ARM_TOOLCHAIN_PATH}arm-none-eabi-size${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
#set(CMAKE_STRIP                     ${BAREMETAL_ARM_TOOLCHAIN_PATH}arm-none-eabi-strip${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")

# -MMD -MF $(@:.o=.d)
# -fabi-version=5 
set(DEVICE							"XMC4800")
set(VARIANT							"F144x2048")
set(LINKER_SCRIPT					"${CMAKE_SOURCE_DIR}/xmclib/CMSIS/Infineon/XMC4800_series/Source/GCC/XMC4800x2048.ld")
set(CPPFLAGS						"-D${DEVICE}_${VARIANT} -MP -MD -MT ${CMAKE_BINARY_DIR}/$(@) ")
set(ARCHFLAGS						"-nostartfiles -mfloat-abi=softfp -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb")
set(OBJFLAGS						"-Wall -fmessage-length=0 -Wa,-adhlns=${CMAKE_BINARY_DIR}/@.lst ${ARCHFLAGS}")
set(CFLAGS							"${CPPFLAGS} ${OBJFLAGS} -Os -ffunction-sections -fdata-sections  -std=gnu99 -pipe")
set(ASMFLAGS						"${CPPFLAGS} ${OBJFLAGS} -x assembler-with-cpp")
set(OBJCPYFLAGS						"-I binary -O elf32-littlearm -B arm")
set(LINKERFLAGS						"-nostartfiles -T${LINKER_SCRIPT}  -Xlinker --gc-sections -specs=nano.specs -specs=nosys.specs -Wl,-Map,${CMAKE_BINARY_DIR}/${PROJECT_NAME}.map")

set(CMAKE_CXX_FLAGS                 "${CPPFLAGS}" CACHE INTERNAL "")
set(CMAKE_ASM_FLAGS                 "${ASMFLAGS}" CACHE INTERNAL "")
set(CMAKE_C_FLAGS                   "${CFLAGS}" CACHE INTERNAL "")
set(CMAKE_C_FLAGS_DEBUG             "${CPPFLAGS} ${OBJFLAGS} -Os -ffunction-sections -fdata-sections  -std=c++11 -pipe -g" CACHE INTERNAL "")
set(CMAKE_C_FLAGS_RELEASE           "${CPPFLAGS} ${OBJFLAGS} -Os -ffunction-sections -fdata-sections  -std=c++11 -pipe -DNDEBUG" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_C_FLAGS_DEBUG}" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_RELEASE         "${CMAKE_C_FLAGS_RELEASE}" CACHE INTERNAL "")
set(CMAKE_EXE_LINKER_FLAGS			"${LINKERFLAGS}" CACHE INTERNAL "")


set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# -nostartfiles -Xlinker --gc-sections -specs=nano.specs -specs=nosys.specs -Wl,-Map,${PROJECT_NAME}.map ${ARCHFLAGS}