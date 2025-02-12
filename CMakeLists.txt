cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(servo_control C CXX ASM)
pico_sdk_init()

add_executable(servo_control
    servo_control.c
)

pico_enable_stdio_usb(servo_control 1)
pico_enable_stdio_uart(servo_control 1)

target_link_libraries(servo_control pico_stdlib hardware_pwm)
pico_add_extra_outputs(servo_control)
