#pragma once

#include <unordered_map>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <GL/glx.h>

#include "key_codes.h"

#include "awml.h"

namespace awml {

    class XWindow;

    class XOpenGLContext : public GraphicsContext
    {
    private:
        XWindow*             m_Parent;
        XVisualInfo*         m_VisualInfo;
        Colormap             m_ColorMap;
        XSetWindowAttributes m_Attribs;
        GLXContext           m_OpenGLContext;
        XWindowAttributes    m_WinAttribs;
        GLint                m_Props[5];
    public:
        XOpenGLContext();

        bool Setup(Window* self) override;
        bool Activate() override;
        void SwapBuffers() override;

        ~XOpenGLContext();
    };

    class XWindow : public Window
    {
    private:
        friend class XOpenGLContext;
    private:
        Display* m_Connection;
        ::Window m_Window;
        XEvent m_Event;

        std::wstring m_Title;

        uint16_t m_Width;
        uint16_t m_Height;

        uint16_t m_MouseX;
        uint16_t m_MouseY;

        Context m_ContextType;
        window_context m_Context;

        WindowMode m_WindowMode;
        CursorMode m_CursorMode;

        bool m_ShouldClose;

        std::unordered_map<awml_key, uint8_t>
            m_RepeatCount;

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
        XWindow(
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

        void Update() override;

        void SetTitle(const std::wstring& title) override;

        bool ShouldClose() override;

        void Close() override;

        uint16_t Width() override;

        uint16_t Height() override;

        uint16_t MouseX() override;

        uint16_t MouseY() override;

        void OnErrorFunc(
            error_callback cb
        ) override;

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

        bool Minimized() override;

        bool KeyPressed(awml_key key_code) override;

        void SetCursorMode(CursorMode cursor_mode) override;

        void SetWindowMode(WindowMode window_mode) override;

        void Resize(uint16_t width, uint16_t height) override;

        void* GetNativeHandle() override;

        ~XWindow();
    private:
        void SetWindowTitle();

        awml_key NormalizeKeyPress();

        wchar_t GetTypedChar();

        uint8_t GetKeyRepeatCount(awml_key code);

        void IncremetRepeatCount(awml_key code);
        void ResetRepeatCount(awml_key code);
    };
}
