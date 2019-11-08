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
        DELETE       = VK_DELETE,

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
        RIGHT_CTRL   = VK_RCONTROL,
        LEFT_ALT     = VK_LMENU,
        RIGHT_ALT    = VK_RMENU,
    };
#elif defined (__linux__)
    #include <X11/keysym.h>

    enum class awml_key : uint16_t
    {
        // ---- Mouse buttons ----
        MOUSE_LEFT   = 0x01,
        MOUSE_RIGHT  = 0x03,
        MOUSE_MIDDLE = 0x02,
        MOUSE_X1     = 0x04,
        MOUSE_X2     = 0x05,
        
        // ---- Misc keyboard keys ----
        BACKSPACE    = XK_BackSpace,
        TAB          = XK_Tab,
        ENTER        = XK_Return,
        PAUSE        = XK_Pause,
        ESCAPE       = XK_Escape,
        DELETE       = XK_Delete,
        HOME         = XK_Home,

        // ---- Arrow keys ----
        LEFT         = XK_Left,
        UP           = XK_Up,
        RIGHT        = XK_Right,
        DOWN         = XK_Down,

        // ---- Misc keyboard ----
        PAGEUP       = XK_Page_Up,
        PAGEDOWN     = XK_Page_Down,
        END          = XK_End, 
        PRINTSCREEN  = XK_Print,
        INSERT       = XK_Insert,
        SPACEBAR     = XK_space,

        // ---- Keyboard numbers ----
        _0        = XK_0,
        _1        = XK_1,
        _2        = XK_2,
        _3        = XK_3,
        _4        = XK_4,
        _5        = XK_5,
        _6        = XK_6,
        _7        = XK_7,
        _8        = XK_8,
        _9        = XK_9,
        
        // ---- Keyboard letters ----
        A        = XK_A,
        B        = XK_B,
        C        = XK_C,
        D        = XK_D,
        E        = XK_E,
        F        = XK_F,
        G        = XK_G,
        H        = XK_H,
        I        = XK_I,
        J        = XK_J,
        K        = XK_K,
        L        = XK_L,
        M        = XK_M,
        N        = XK_N,
        O        = XK_O,
        P        = XK_P,
        Q        = XK_Q,
        R        = XK_R,
        S        = XK_S,
        T        = XK_T,
        U        = XK_U,
        V        = XK_V,
        W        = XK_W,
        X        = XK_X,
        Y        = XK_Y,
        Z        = XK_Z,

        // ---- SUPER (system-specific) ----
        SUPER_LEFT  = XK_Super_L,
        SUPER_RIGHT = XK_Super_R,
        
        // ---- Numpad keys ----
        NP_0         = XK_KP_0,
        NP_1         = XK_KP_1,
        NP_2         = XK_KP_2,
        NP_3         = XK_KP_3,
        NP_4         = XK_KP_4,
        NP_5         = XK_KP_5,
        NP_6         = XK_KP_6,
        NP_7         = XK_KP_7,
        NP_8         = XK_KP_8,
        NP_9         = XK_KP_9,
        NP_MULTIPLY  = XK_KP_Multiply,
        NP_ADD       = XK_KP_Add,
        NP_SEPARATOR = XK_KP_Separator,
        NP_SUBTRACT  = XK_KP_Subtract,
        NP_DECIMAL   = XK_KP_Decimal,
        NP_DIVIDE    = XK_KP_Divide,
        
        // ---- Function keys ----
        F1           = XK_F1,
        F2           = XK_F2,
        F3           = XK_F3,
        F4           = XK_F4,
        F5           = XK_F5,
        F6           = XK_F6,
        F7           = XK_F7,
        F8           = XK_F8,
        F9           = XK_F9,
        F10          = XK_F10,
        F11          = XK_F11,
        F12          = XK_F12,
        
        // ---- Shift/ctrl/etc keys ----
        NUMLOCK      = XK_Num_Lock,
        SCRLLOCK     = XK_Scroll_Lock,
        LEFT_SHIFT   = XK_Shift_L,
        RIGHT_SHIFT  = XK_Shift_R,
        LEFT_CTRL    = XK_Control_L,
        RIGHT_CTRL   = XK_Control_R,
        CAPSLOCK     = XK_Caps_Lock,
        LEFT_ALT     = XK_Alt_L,
        RIGHT_ALT    = XK_Alt_R
    };

#else
    #error Sorry, your platform is currently not supported!
#endif
