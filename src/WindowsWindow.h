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

        Context        m_ContextType;
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

        WindowMode m_WindowMode;
        CursorMode m_CursorMode;

        bool m_ShouldClose;

        error_callback          m_ErrorCB;
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
            Context context,
            WindowMode window_mode,
            CursorMode cursor_mode,
            bool resizable
        );

        void Launch() override;

        void SetContext(window_context wc) override;

        void SetTitle(const std::wstring& title) override;

        void PollEvents() override;

        void SwapBuffers() override;

        void Update() override;

        bool ShouldClose() override;

        void Close() override;

        uint16_t GetWidth() override;

        uint16_t GetHeight() override;

        std::pair<uint16_t, uint16_t> GetMouseCoords() override;

        uint16_t GetMouseX() override;

        uint16_t GetMouseY() override;

        void OnError(
            error_callback cb
        ) override;

        void OnKeyPressed(
            key_pressed_callback cb
        ) override;

        void OnKeyReleased(
            key_released_callback cb
        ) override;

        void OnWindowResized(
            window_resized_callback cb
        ) override;

        void OnWindowClosed(
            window_closed_callback cb
        ) override;

        void OnMouseMoved(
            mouse_moved_callback cb
        ) override;

        void OnMousePressed(
            mouse_pressed_callback cb
        ) override;

        void OnMouseReleased(
            mouse_released_callback cb
        ) override;

        void OnMouseScrolled(
            mouse_scrolled_callback cb
        ) override;

        void OnCharTyped(
            char_typed_callback cb
        ) override;

        bool Minimized() override;

        bool IsKeyPressed(awml_key key_code) override;

        void SetCursorMode(CursorMode cursor_mode) override;

        void SetWindowMode(WindowMode window_mode) override;

        void Resize(uint16_t width, uint16_t height) override;

        void* GetNativeHandle() override;

        ~WindowsWindow();
    private:
        bool EnsureAlive();

        void SetResolution(uint16_t width, uint16_t height);

        void RecalculateNative();

        void OnError(error code, const std::string& msg);

        void OnWindowResized(WORD width, WORD height);

        void OnWindowClosed();

        void OnMouseMoved(WORD xpos, WORD ypos);

        void OnMousePressed(awml_key code);

        void OnMouseReleased(awml_key code);

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
