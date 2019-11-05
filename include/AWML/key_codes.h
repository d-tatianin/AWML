#pragma once

#include <cstdint>

#ifdef _WIN32

    #include <winuser.h>

    enum class awml_key : uint16_t
    {
        // ---- Mouse buttons ----
        MOUSE_LEFT   = VK_LBUTTON,
        MOUSE_RIGHT  = VK_RBUTTON,
        MOUSE_MIDDLE = VK_MBUTTON,
        MOUSE_X1     = VK_XBUTTON1,
        MOUSE_X2     = VK_XBUTTON2,
        
        // ---- Misc keyboard keys ----
        BACKSPACE    = VK_BACK,
        TAB          = VK_TAB,
        ENTER        = VK_RETURN,
        SHIFT        = VK_SHIFT,
        CONTROL      = VK_CONTROL,
        ALT          = VK_MENU,
        PAUSE        = VK_PAUSE,
        CAPSLOCK     = VK_CAPITAL,
        ESCAPE       = VK_ESCAPE,
        SPACEBAR     = VK_SPACE,
        PAGEUP       = VK_PRIOR,
        PAGEDOWN     = VK_NEXT,
        END          = VK_END,
        HOME         = VK_HOME,
        
        // ---- Arrow keys ----
        LEFT         = VK_LEFT,
        UP           = VK_UP,
        RIGHT        = VK_RIGHT,
        DOWN         = VK_DOWN,
        
        // ---- Misc keyboard ----
        PRINTSCREEN  = VK_SNAPSHOT,
        INSERT       = VK_INSERT,
        DEL          = VK_DELETE,

        // ---- Keyboard numbers ----
        _0        = 0x30,
        _1        = 0x31,
        _2        = 0x32,
        _3        = 0x33,
        _4        = 0x34,
        _5        = 0x35,
        _6        = 0x36,
        _7        = 0x37,
        _8        = 0x38,
        _9        = 0x39,
        
        // ---- Keyboard letters ----
        A        = 0x41,
        B        = 0x42,
        C        = 0x43,
        D        = 0x44,
        E        = 0x45,
        F        = 0x46,
        G        = 0x47,
        H        = 0x48,
        I        = 0x49,
        J        = 0x4A,
        K        = 0x4B,
        L        = 0x4C,
        M        = 0x4D,
        N        = 0x4E,
        O        = 0x4F,
        P        = 0x50,
        Q        = 0x51,
        R        = 0x52,
        S        = 0x53,
        T        = 0x54,
        U        = 0x55,
        V        = 0x56,
        W        = 0x57,
        X        = 0x58,
        Y        = 0x59,
        Z        = 0x5A,

        // ---- SUPER (system-specific) ----
        SUPER_LEFT  = VK_LWIN,
        SUPER_RIGHT = VK_RWIN,
        
        // ---- Numpad keys ----
        NP_0         = VK_NUMPAD0,
        NP_1         = VK_NUMPAD1,
        NP_2         = VK_NUMPAD2,
        NP_3         = VK_NUMPAD3,
        NP_4         = VK_NUMPAD4,
        NP_5         = VK_NUMPAD5,
        NP_6         = VK_NUMPAD6,
        NP_7         = VK_NUMPAD7,
        NP_8         = VK_NUMPAD8,
        NP_9         = VK_NUMPAD9,
        NP_MULTIPLY  = VK_MULTIPLY,
        NP_ADD       = VK_ADD,
        NP_SEPARATOR = VK_SEPARATOR,
        NP_SUBTRACT  = VK_SUBTRACT,
        NP_DECIMAL   = VK_DECIMAL,
        NP_DIVIDE    = VK_DIVIDE,
        
        // ---- Function keys ----
        F1           = VK_F1,
        F2           = VK_F2,
        F3           = VK_F3,
        F4           = VK_F4,
        F5           = VK_F5,
        F6           = VK_F6,
        F7           = VK_F7,
        F8           = VK_F8,
        F9           = VK_F9,
        F10          = VK_F10,
        F11          = VK_F11,
        F12          = VK_F12,
        
        // ---- Shift/ctrl/etc keys ----
        NUMLOCK      = VK_NUMLOCK,
        SCRLLOCK     = VK_SCROLL,
        LEFT_SHIFT   = VK_LSHIFT,
        RIGHT_SHIFT  = VK_RSHIFT,
        LEFT_CTRL    = VK_LCONTROL,
        RIGHT_CTRL   = VK_RCONTROL
    };
#elif defined (__linux__)
    typedef uint32_t awml_key;
#else
    #error Sorry, your platform is currently not supported!
#endif
