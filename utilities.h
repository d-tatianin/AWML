#pragma once

#include <winuser.h>

#ifdef _WIN32
    #define AWML_REPEATED_BIT      0x40000000
    #define AWML_KEY_PRESSED_BIT   0x8000
    #define AWML_REPEAT_COUNT_MASK 0xffff
    typedef int32_t awml_keycode;
#endif
