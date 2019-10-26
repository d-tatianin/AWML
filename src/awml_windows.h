#pragma once

#include <windows.h>
#include <windowsx.h>
#include <winuser.h>

#include "key_codes.h"

#include "awml.h"

namespace awml {

    class WindowsWindow;

    class WindowsOpenGLContext : public GraphicsContext
    {
    private:
        HDC m_Context;
        HGLRC m_OpenGLContext;
        WindowsWindow* m_Window;
        int m_Format;
    public:
        WindowsOpenGLContext();

        bool Setup(Window* self) override;
        bool Activate() override;
        void SwapBuffers() override;

        ~WindowsOpenGLContext();
    };

    class WindowsWindow : public Window
    {
    private:
        friend class WindowsOpenGLContext;
    private:
        static HINSTANCE s_ThisInstance;
        static uint16_t  s_WindowID;

        std::wstring m_ClassName;
        std::wstring m_WindowTitle;
        WNDCLASSW m_WinProps;
        HWND m_Window;

        uint32_t m_WindowStyle;

        window_context m_Context;

        uint16_t m_OriginalWidth;
        uint16_t m_OriginalHeight;

        uint16_t m_RunningWidth;
        uint16_t m_RunningHeight;

        uint16_t m_TrueWidth;
        uint16_t m_TrueHeight;

        uint16_t m_NativeWidth;
        uint16_t m_NativeHeight;

        uint16_t m_MouseX;
        uint16_t m_MouseY;

        bool m_FullScreen;
        bool m_ShouldClose;

        key_pressed_callback    m_KeyPressedCB;
        key_released_callback   m_KeyReleasedCB;
        window_resized_callback m_WindowResizedCB;
        window_closed_callback  m_WindowClosedCB;
        mouse_moved_callback    m_MouseMovedCB;
        mouse_pressed_callback  m_MousePressedCB;
        mouse_released_callback m_MouseReleasedCB;
        mouse_scrolled_callback m_MouseScrolledCB;
        char_typed_callback     m_CharTypedCB;
    public:
        WindowsWindow(
            const std::wstring& title,
            uint16_t width,
            uint16_t height,
            bool resizable,
            bool fullscreen
        );

        void SetContext(window_context wc) override;

        void Update() override;

        bool ShouldClose() override;

        void Close() override;

        uint16_t Width() override;

        uint16_t Height() override;

        uint16_t MouseX() override;

        uint16_t MouseY() override;

        void OnKeyPressedFunc(
            key_pressed_callback cb
        ) override;

        void OnKeyReleasedFunc(
            key_released_callback cb
        ) override;

        void OnWindowResizedFunc(
            window_resized_callback cb
        ) override;

        void OnWindowClosedFunc(
            window_closed_callback cb
        ) override;

        void OnMouseMovedFunc(
            mouse_moved_callback cb
        ) override;

        void OnMousePressedFunc(
            mouse_pressed_callback cb
        ) override;

        void OnMouseReleasedFunc(
            mouse_released_callback cb
        ) override;

        void OnMouseScrolledFunc(
            mouse_scrolled_callback cb
        ) override;

        void OnCharTypedFunc(
            char_typed_callback cb
        ) override;

        bool KeyPressed(awml_keycode key_code) override;

        void SetFullscreen(bool mode) override;

        ~WindowsWindow();
    private:
        void SetResolution(uint16_t width, uint16_t height);

        void RecalculateNative();

        void OnWindowResized(WORD width, WORD height);

        void OnWindowClosed();

        void OnMouseMoved(WORD xpos, WORD ypos);

        void OnMousePressed(UINT code);

        void OnMouseReleased(UINT code);

        void OnMouseScrolled(int16_t rotation, bool vertical);

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
