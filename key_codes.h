#pragma once

#ifdef _WIN32
    // ---- Mouse buttons ----
    #define AWML_MB_LEFT      0x01
    #define AWML_MB_RIGHT     0x02
    #define AWML_MB_MIDDLE    0x04

    // ---- Misc keyboard keys ----
    #define AWML_BACKSPACE    0x08
    #define AWML_TAB          0x09
    #define AWML_ENTER        0x0D
    #define AWML_SHIFT        0x10
    #define AWML_CONTROL      0x11
    #define AWML_ALT          0x12
    #define AWML_PAUSE        0x13
    #define AWML_CAPSLOCK     0x14
    #define AWML_ESCAPE       0x1B
    #define AWML_SPACEBAR     0x20
    #define AWML_PAGEUP       0x21
    #define AWML_PAGEDOWN     0x22
    #define AWML_END          0x23
    #define AWML_HOME         0x24

    // ---- Arrow keys ----
    #define AWML_LEFT         0x25
    #define AWML_UP           0x26
    #define AWML_RIGHT        0x27
    #define AWML_DOWN         0x28

    // ---- Misc keyboard ----
    #define AWML_PRINTSCREEN  0x2C
    #define AWML_INSERT       0x2D
    #define AWML_DELETE       0x2E

    // ---- Keyboard numbers ----
    #define AWML_KEY_0        0x30
    #define AWML_KEY_1        0x31
    #define AWML_KEY_2        0x32
    #define AWML_KEY_3        0x33
    #define AWML_KEY_4        0x34
    #define AWML_KEY_5        0x35
    #define AWML_KEY_6        0x36
    #define AWML_KEY_7        0x37
    #define AWML_KEY_8        0x38
    #define AWML_KEY_9        0x39

    // ---- Keyboard letters ----
    #define AWML_KEY_A        0x41
    #define AWML_KEY_B        0x42
    #define AWML_KEY_C        0x43
    #define AWML_KEY_D        0x44
    #define AWML_KEY_E        0x45
    #define AWML_KEY_F        0x46
    #define AWML_KEY_G        0x47
    #define AWML_KEY_H        0x48
    #define AWML_KEY_I        0x49
    #define AWML_KEY_J        0x4A
    #define AWML_KEY_K        0x4B
    #define AWML_KEY_L        0x4C
    #define AWML_KEY_M        0x4D
    #define AWML_KEY_N        0x4E
    #define AWML_KEY_O        0x4F
    #define AWML_KEY_P        0x50
    #define AWML_KEY_Q        0x51
    #define AWML_KEY_R        0x52
    #define AWML_KEY_S        0x53
    #define AWML_KEY_T        0x54
    #define AWML_KEY_U        0x55
    #define AWML_KEY_V        0x56
    #define AWML_KEY_W        0x57
    #define AWML_KEY_X        0x58
    #define AWML_KEY_Y        0x59
    #define AWML_KEY_Z        0x5A

    // ---- Numpad keys ----
    #define AWML_NP_0         0x60
    #define AWML_NP_1         0x61
    #define AWML_NP_2         0x62
    #define AWML_NP_3         0x63
    #define AWML_NP_4         0x64
    #define AWML_NP_5         0x65
    #define AWML_NP_6         0x66
    #define AWML_NP_7         0x67
    #define AWML_NP_8         0x68
    #define AWML_NP_9         0x69
    #define AWML_NP_MULTIPLY  0x6A
    #define AWML_NP_ADD       0x6B
    #define AWML_NP_SEPARATOR 0x6C
    #define AWML_NP_SUBTRACT  0x6D
    #define AWML_NP_DECIMAL   0x6E
    #define AWML_NP_DIVIDE    0x6F

    // ---- Function keys ----
    #define AWML_F1           0x70
    #define AWML_F2           0x71
    #define AWML_F3           0x72
    #define AWML_F4           0x73 
    #define AWML_F5           0x74
    #define AWML_F6           0x75
    #define AWML_F7           0x76
    #define AWML_F8           0x77
    #define AWML_F9           0x78
    #define AWML_F10          0x79
    #define AWML_F11          0x7A
    #define AWML_F12          0x7B

    // ---- Shift/ctrl/etc keys ----
    #define AWML_NUMLOCK      0x90
    #define AWML_SCRLLOCK     0x91
    #define AWML_LEFT_SHIFT   0xA0
    #define AWML_RIGHT_SHIFT  0xA1
    #define AWML_LEFT_CTRL    0xA2
    #define AWML_RIGHT_CTRL   0xA3
#else
    #error Sorry, your platform is currently not supported!
#endif
