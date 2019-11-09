#pragma once

#include <cstdint>
#include <iostream>
#include <string>

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

        // ---- Arrow keys ----
        LEFT         = VK_LEFT,
        UP           = VK_UP,
        RIGHT        = VK_RIGHT,
        DOWN         = VK_DOWN,

        // ---- Keyboard numbers ----
        _0           = 0x30,
        _1           = 0x31,
        _2           = 0x32,
        _3           = 0x33,
        _4           = 0x34,
        _5           = 0x35,
        _6           = 0x36,
        _7           = 0x37,
        _8           = 0x38,
        _9           = 0x39,

        // ---- Keyboard letters ----
        A            = 0x41,
        B            = 0x42,
        C            = 0x43,
        D            = 0x44,
        E            = 0x45,
        F            = 0x46,
        G            = 0x47,
        H            = 0x48,
        I            = 0x49,
        J            = 0x4A,
        K            = 0x4B,
        L            = 0x4C,
        M            = 0x4D,
        N            = 0x4E,
        O            = 0x4F,
        P            = 0x50,
        Q            = 0x51,
        R            = 0x52,
        S            = 0x53,
        T            = 0x54,
        U            = 0x55,
        V            = 0x56,
        W            = 0x57,
        X            = 0x58,
        Y            = 0x59,
        Z            = 0x5A,

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

        // ---- Shift/Ctrl/Alt keys ----
        LEFT_SHIFT   = VK_LSHIFT,
        RIGHT_SHIFT  = VK_RSHIFT,
        LEFT_CTRL    = VK_LCONTROL,
        RIGHT_CTRL   = VK_RCONTROL,
        LEFT_ALT     = VK_LMENU,
        RIGHT_ALT    = VK_RMENU,

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
        PRINTSCREEN  = VK_SNAPSHOT,
        INSERT       = VK_INSERT,
        DEL          = VK_DELETE,
        NUMLOCK      = VK_NUMLOCK,
        SCRLLOCK     = VK_SCROLL,

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

        // ---- SUPER (system-specific) ----
        SUPER_LEFT   = VK_LWIN,
        SUPER_RIGHT  = VK_RWIN,
    };
#elif defined (__linux__)
    #include <X11/keysym.h>

    enum class awml_key : uint16_t
    {
        // ---- Mouse buttons ----
        MOUSE_LEFT   = 0x01,
        MOUSE_RIGHT  = 0x03,
        MOUSE_MIDDLE = 0x02,
        MOUSE_X1     = 0x08,
        MOUSE_X2     = 0x09,

        // ---- Arrow keys ----
        LEFT         = XK_Left,
        UP           = XK_Up,
        RIGHT        = XK_Right,
        DOWN         = XK_Down,

        // ---- Keyboard numbers ----
        _0           = XK_0,
        _1           = XK_1,
        _2           = XK_2,
        _3           = XK_3,
        _4           = XK_4,
        _5           = XK_5,
        _6           = XK_6,
        _7           = XK_7,
        _8           = XK_8,
        _9           = XK_9,

        // ---- Keyboard letters ----
        A            = XK_a,
        B            = XK_b,
        C            = XK_c,
        D            = XK_d,
        E            = XK_e,
        F            = XK_f,
        G            = XK_g,
        H            = XK_h,
        I            = XK_i,
        J            = XK_j,
        K            = XK_k,
        L            = XK_l,
        M            = XK_m,
        N            = XK_n,
        O            = XK_o,
        P            = XK_p,
        Q            = XK_q,
        R            = XK_r,
        S            = XK_s,
        T            = XK_t,
        U            = XK_u,
        V            = XK_v,
        W            = XK_w,
        X            = XK_x,
        Y            = XK_y,
        Z            = XK_z,

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

        // ---- Shift/Ñtrl/Alt keys ----
        LEFT_SHIFT   = XK_Shift_L,
        RIGHT_SHIFT  = XK_Shift_R,
        LEFT_CTRL    = XK_Control_L,
        RIGHT_CTRL   = XK_Control_R,
        LEFT_ALT     = XK_Alt_L,
        RIGHT_ALT    = XK_Alt_R,

        // ---- Misc keyboard keys ----
        BACKSPACE    = XK_BackSpace,
        TAB          = XK_Tab,
        ENTER        = XK_Return,
        PAUSE        = XK_Pause,
        CAPSLOCK     = XK_Caps_Lock,
        ESCAPE       = XK_Escape,
        SPACEBAR     = XK_space,
        PAGEUP       = XK_Page_Up,
        PAGEDOWN     = XK_Page_Down,
        END          = XK_End,
        HOME         = XK_Home,
        PRINTSCREEN  = XK_Print,
        INSERT       = XK_Insert,
        DEL          = XK_Delete,
        NUMLOCK      = XK_Num_Lock,
        SCRLLOCK     = XK_Scroll_Lock,

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

        // ---- SUPER (system-specific) ----
        SUPER_LEFT   = XK_Super_L,
        SUPER_RIGHT  = XK_Super_R
    };

#else
    #error Sorry, your platform is currently not supported!
#endif

namespace awml {

    static inline std::string KeyToString(awml_key code)
    {
        switch (code)
        {
            case awml_key::MOUSE_LEFT:   return "Left Mouse Button";
            case awml_key::MOUSE_RIGHT:  return "Right Mouse Button";
            case awml_key::MOUSE_MIDDLE: return "Middle Mouse Button";
            case awml_key::MOUSE_X1:     return "X1 Mouse Button";
            case awml_key::MOUSE_X2:     return "X2 Mouse Button";

            case awml_key::LEFT:         return "Left Arrow";
            case awml_key::UP:           return "Up Arrow";
            case awml_key::RIGHT:        return "Right Arrow";
            case awml_key::DOWN:         return "Down Arrow";

            case awml_key::_0:           return "0 Key";
            case awml_key::_1:           return "1 Key";
            case awml_key::_2:           return "2 Key";
            case awml_key::_3:           return "3 Key";
            case awml_key::_4:           return "4 Key";
            case awml_key::_5:           return "5 Key";
            case awml_key::_6:           return "6 Key";
            case awml_key::_7:           return "7 Key";
            case awml_key::_8:           return "8 Key";
            case awml_key::_9:           return "9 Key";

            case awml_key::A:            return "A Key";
            case awml_key::B:            return "B Key";
            case awml_key::C:            return "C Key";
            case awml_key::D:            return "D Key";
            case awml_key::E:            return "E Key";
            case awml_key::F:            return "F Key";
            case awml_key::G:            return "G Key";
            case awml_key::H:            return "H Key";
            case awml_key::I:            return "I Key";
            case awml_key::J:            return "J Key";
            case awml_key::K:            return "K Key";
            case awml_key::L:            return "L Key";
            case awml_key::M:            return "M Key";
            case awml_key::N:            return "N Key";
            case awml_key::O:            return "O Key";
            case awml_key::P:            return "P Key";
            case awml_key::Q:            return "Q Key";
            case awml_key::R:            return "R Key";
            case awml_key::S:            return "S Key";
            case awml_key::T:            return "T Key";
            case awml_key::U:            return "U Key";
            case awml_key::V:            return "V Key";
            case awml_key::W:            return "W Key";
            case awml_key::X:            return "X Key";
            case awml_key::Y:            return "Y Key";
            case awml_key::Z:            return "Z Key";

            case awml_key::F1:           return "F1";
            case awml_key::F2:           return "F2";
            case awml_key::F3:           return "F3";
            case awml_key::F4:           return "F4";
            case awml_key::F5:           return "F5";
            case awml_key::F6:           return "F6";
            case awml_key::F7:           return "F7";
            case awml_key::F8:           return "F8";
            case awml_key::F9:           return "F9";
            case awml_key::F10:          return "F10";
            case awml_key::F11:          return "F11";
            case awml_key::F12:          return "F12";

            case awml_key::LEFT_SHIFT:   return "Left Shift";
            case awml_key::RIGHT_SHIFT:  return "Right Shift";
            case awml_key::LEFT_CTRL:    return "Left Ctrl";
            case awml_key::RIGHT_CTRL:   return "Right Ctrl";
            case awml_key::LEFT_ALT:     return "Left Alt";
            case awml_key::RIGHT_ALT:    return "Right Alt";

            case awml_key::BACKSPACE:    return "Backspace";
            case awml_key::TAB:          return "Tab";
            case awml_key::ENTER:        return "Enter";
            case awml_key::PAUSE:        return "Pause";
            case awml_key::CAPSLOCK:     return "Caps Lock";
            case awml_key::ESCAPE:       return "Escape";
            case awml_key::SPACEBAR:     return "Spacebar";
            case awml_key::PAGEUP:       return "Page Up";
            case awml_key::PAGEDOWN:     return "Page Down";
            case awml_key::END:          return "End";
            case awml_key::HOME:         return "Home";
            case awml_key::PRINTSCREEN:  return "Print Screen";
            case awml_key::INSERT:       return "Insert";
            case awml_key::DEL:          return "Delete";
            case awml_key::NUMLOCK:      return "Num Lock";
            case awml_key::SCRLLOCK:     return "Scroll Lock";

            case awml_key::NP_0:         return "Numpad 0";
            case awml_key::NP_1:         return "Numpad 1";
            case awml_key::NP_2:         return "Numpad 2";
            case awml_key::NP_3:         return "Numpad 3";
            case awml_key::NP_4:         return "Numpad 4";
            case awml_key::NP_5:         return "Numpad 5";
            case awml_key::NP_6:         return "Numpad 6";
            case awml_key::NP_7:         return "Numpad 7";
            case awml_key::NP_8:         return "Numpad 8";
            case awml_key::NP_9:         return "Numpad 9";
            case awml_key::NP_MULTIPLY:  return "Numpad *";
            case awml_key::NP_ADD:       return "Numpad +";
            case awml_key::NP_SEPARATOR: return "Numpad ,";
            case awml_key::NP_SUBTRACT:  return "Numpad -";
            case awml_key::NP_DECIMAL:   return "Numpad .";
            case awml_key::NP_DIVIDE:    return "Numpad /";

            case awml_key::SUPER_LEFT:   return "Left Super";
            case awml_key::SUPER_RIGHT:  return "Right Super";

            default: {
                std::string custom_code =
                std::to_string(
                    static_cast<uint16_t>(code)
                );
                custom_code += " Key";

                return custom_code;
            }
        }
    }
}

static inline std::ostream& operator<<(std::ostream& stream, awml_key code)
{
    return stream << awml::KeyToString(code);
}
