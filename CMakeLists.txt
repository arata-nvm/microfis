cmake_minimum_required(VERSION 3.22)
project(microfis C ASM)

file(GLOB_RECURSE SRC *.c *.s)
include_directories(include)

set(TARGET kernel)

set(CMAKE_C_FLAGS "-g -m32 -masm=intel -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra -Werror")
set(CMAKE_ASM_FLAGS "-m32 -masm=intel")
set(CMAKE_EXE_LINKER_FLAGS "-T${PROJECT_SOURCE_DIR}/link.ld")

add_executable(${TARGET} ${SRC})
