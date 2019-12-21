#pragma once

#include <unordered_map>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <GL/glx.h>

#include <AWML/key_codes.h>
#include <AWML/awml.h>

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
        GLXFBConfig          m_BestFBC;
    public:
        XOpenGLContext();

        bool Setup(Window* self) override;
        bool Activate() override;
        void MakeCurrent() override;
        void SwapBuffers() override;

        XVisualInfo* GetVisualInfo();
        XSetWindowAttributes* GetAttribsPtr();

        ~XOpenGLContext();
    private:
        bool EnsureSetup();
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

        bool Launch() override;

        bool SetContext(window_context wc) override;

        void PollEvents() override;
        void SwapBuffers() override;

        void Update() override;

        void SetTitle(const std::wstring& title) override;

        bool ShouldClose() override;

        void Close() override;

        uint16_t GetWidth() override;
        uint16_t GetHeight() override;

        uint16_t GetMouseX() override;
        uint16_t GetMouseY() override;
        std::pair<uint16_t, uint16_t> GetMouseCoords() override;

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

        void MakeCurrent() override;

        ~XWindow();
    private:
        bool EnsureAlive();

        void NotifyError(error code, const std::string& message);

        void UpdateWindowTitle();

        awml_key NormalizeKeyPress();

        wchar_t GetTypedChar();

        uint8_t GetKeyRepeatCount(awml_key code);

        void IncremetRepeatCount(awml_key code);
        void ResetRepeatCount(awml_key code);
    };
}
