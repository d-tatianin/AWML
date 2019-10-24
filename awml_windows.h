#pragma once

#include <windows.h>
#include <windowsx.h>
#include <winuser.h>

#include "key_codes.h"

#include "awml.h"

namespace awml {

    class WindowsWindow : public Window
    {
    private:
        static HINSTANCE s_ThisInstance;
        static uint16_t  s_WindowID;

        std::wstring m_ClassName;
        std::wstring m_WindowTitle;
        WNDCLASSW m_WinProps;
        HWND m_Window;

        uint16_t m_Width;
        uint16_t m_Height;

        uint16_t m_MouseX;
        uint16_t m_MouseY;

        bool m_ShouldClose;

    public:
        WindowsWindow(
            const std::wstring& title,
            uint16_t width,
            uint16_t height
        );

        void PollEvents() override;

        bool ShouldClose() override;

        void Close() override;

        uint16_t Width() override;

        uint16_t Height() override;

        uint16_t MouseX() override;

        uint16_t MouseY() override;

        bool KeyPressed(awml_keycode key_code) override;

        ~WindowsWindow();
    private:
        void OnWindowResized(WORD width, WORD height);

        void OnWindowClosed();

        void OnMouseMoved(WORD xpos, WORD ypos);

        void OnMousePressed(UINT code);

        void OnMouseReleased(UINT code);

        void OnMouseScrolled(int16_t rotation);

        void OnKeyPressed(WPARAM key_code, bool repeated, uint16_t repeat_count);

        void OnKeyReleased(WPARAM key_code);

        void OnCharTyped(wchar_t typed_char);

        static LRESULT CALLBACK WindowEventHandler(
            HWND window,
            UINT message,
            WPARAM param_1,
            LPARAM param_2
        );
    };
}
