cmake_minimum_required(VERSION 3.25)
project(one_armed_bandit C)

set(CMAKE_C_STANDARD 11)

include_directories(lib/button_library)
include_directories(lib/buzzer_library)
include_directories(lib/display_library)
include_directories(lib/led_library)
include_directories(lib/timer_library)
include_directories(lib/usart)
include_directories(lib/utils)

add_executable(one_armed_bandit
        lib/button_library/button.c
        lib/button_library/button.h
        lib/buzzer_library/buzzer.c
        lib/buzzer_library/buzzer.h
        lib/display_library/display.c
        lib/display_library/display.h
        lib/led_library/led.c
        lib/led_library/led.h
        lib/timer_library/timer.c
        lib/timer_library/timer.h
        lib/usart/usart.c
        lib/usart/usart.h
        lib/utils/LEDlibrary.c
        lib/utils/LEDlibrary.h
        lib/utils/utils.c
        lib/utils/utils.h
        src/main.c)
