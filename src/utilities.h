#pragma once

#include <cstddef>

#if _MSVC_LANG > 201703L || __cplusplus > 201703L
    #define AWML_LIKELY   [[likely]]
    #define AWML_UNLIKELY [[unlikely]]
#else
    #define AWML_LIKELY
    #define AWML_UNLIKELY
#endif

#ifdef _WIN32
    #define AWML_REPEATED_BIT      0x40000000
    #define AWML_KEY_PRESSED_BIT   0x8000
    #define AWML_REPEAT_COUNT_MASK 0xffff
    #define AWML_MOUSE_X1_BIT      0x0001
    #define AWML_MOUSE_X2_BIT      0x0002
#endif
