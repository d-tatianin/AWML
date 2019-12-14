#include <iostream>
#include <string>
#include <functional>

#include "WindowsGL.h"
#include "WindowsWindow.h"

#include "utilities.h"

namespace awml {

    WindowsOpenGLContext::WindowsOpenGLContext()
        : m_Context(),
        m_OpenGLContext(),
        m_Format(),
        m_Window()
    {
    }

    bool WindowsOpenGLContext::Setup(Window* self)
    {
        m_Window = static_cast<WindowsWindow*>(self);

        PIXELFORMATDESCRIPTOR pfd =
        {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            32,
            0, 0, 0, 0, 0, 0,
            0,
            0,
            0,
            0, 0, 0, 0,
            24,
            8,
            0,
            PFD_MAIN_PLANE,
            0,
            0, 0, 0
        };

        m_Context = GetDC(m_Window->m_Window);

        m_Format = ChoosePixelFormat(m_Context, &pfd);

        SetPixelFormat(m_Context, m_Format, &pfd);

        m_OpenGLContext = wglCreateContext(m_Context);

        return true;
    }

    bool WindowsOpenGLContext::Activate()
    {
        int attriblist[] = 
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB,
            0,
            WGL_CONTEXT_MINOR_VERSION_ARB,
            0,
            WGL_CONTEXT_PROFILE_MASK_ARB,
            WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0,
            0
        };

        wglMakeCurrent(m_Context, m_OpenGLContext);
        if (!glLoader::Init())
        {
            m_Window->OnError(error::GENERIC, "Failed to initialize OpenGL");
            return false;
        }
        auto glversion = glGetString(GL_VERSION);

        attriblist[1] = glversion[0] - '0';
        attriblist[3] = glversion[2] - '0';

        if (!glLoader::LoadVersion(attriblist[1], attriblist[3]))
        {
            m_Window->OnError(error::GENERIC, "Failed to load OpenGL functions!");
        }

        wglMakeCurrent(m_Context, NULL);
        wglDeleteContext(m_OpenGLContext);

        m_OpenGLContext = wglCreateContextAttribsARB(m_Context, 0, attriblist);
        wglMakeCurrent(m_Context, m_OpenGLContext);

        return true;
    }

    void WindowsOpenGLContext::SwapBuffers()
    {
        ::SwapBuffers(m_Context);
    }

    WindowsOpenGLContext::~WindowsOpenGLContext()
    {
        wglMakeCurrent(m_Context, NULL);
        wglDeleteContext(m_OpenGLContext);
        ReleaseDC(m_Window->m_Window, m_Context);
    }

    WindowsWindow::WindowsWindow(
        const std::wstring& title,
        uint16_t width,
        uint16_t height,
        Context context,
        WindowMode window_mode,
        CursorMode cursor_mode,
        bool resizable
    ) : m_ClassName(title),
        m_WindowTitle(title),
        m_WinProps(),
        m_Window(),
        m_WindowStyle(0),
        m_OriginalWidth(width),
        m_OriginalHeight(height),
        m_RunningWidth(width),
        m_RunningHeight(height),
        m_TrueWidth(0),
        m_TrueHeight(0),
        m_NativeWidth(0),
        m_NativeHeight(0),
        m_MouseX(0),
        m_MouseY(0),
        m_ContextType(context),
        m_WindowMode(window_mode),
        m_CursorMode(cursor_mode),
        m_ShouldClose(false)
    {
        m_ClassName += std::to_wstring(s_WindowID++);

        // this can return an error, add a handle later
        m_WinProps.lpfnWndProc = WindowEventHandler;
        m_WinProps.hInstance = s_ThisInstance;
        m_WinProps.lpszClassName = m_ClassName.c_str();
        m_WinProps.cbWndExtra = sizeof(WindowsWindow*);
        RegisterClassW(&m_WinProps);

        if (resizable)
            m_WindowStyle = WS_OVERLAPPEDWINDOW;
        else
            m_WindowStyle = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME;

        RECT rect;
        rect.left = rect.top = 0;
        rect.right = width;
        rect.bottom = height;
        AdjustWindowRect(&rect, m_WindowStyle, false);

        m_TrueWidth =  static_cast<uint16_t>(rect.right - rect.left);
        m_TrueHeight = static_cast<uint16_t>(rect.bottom - rect.top);

        RecalculateNative();
    }

    void WindowsWindow::Launch()
    {
        static bool launched = false;

        if (!launched)
        {
            launched = true;

            m_Window = CreateWindowExW(
                0,
                m_ClassName.c_str(),
                m_WindowTitle.c_str(),
                m_WindowStyle,
                CW_USEDEFAULT, CW_USEDEFAULT,
                m_TrueWidth,
                m_TrueHeight,
                NULL,
                NULL,
                s_ThisInstance,
                NULL
            );

            if (m_Window == NULL)
            {
                OnError(error::NULL_WINDOW, "Could not create the window!");
                return;
            }

            SetWindowLongPtrW(m_Window, 0, reinterpret_cast<LONG_PTR>(this));

            // this is a bit messy, fix later
            WindowMode wm = m_WindowMode;
            m_WindowMode = WindowMode::UNSPECIFIED;
            SetWindowMode(wm);

            switch (m_ContextType)
            {
            case Context::OpenGL:
                SetContext(
                    std::make_unique<WindowsOpenGLContext>()
                );
                break;
            default:
                break;
            }

            ShowWindow(m_Window, SW_NORMAL);
        }
        else
        {
            OnError(error::GENERIC, "Launch has already been called earlier!");
        }
    }

    void WindowsWindow::SetWindowMode(WindowMode window_mode)
    {
        static WINDOWPLACEMENT last_placement = { sizeof(last_placement) };

        DWORD style = GetWindowLongW(m_Window, GWL_STYLE);

        static bool resolution_changed = false;

        if ((window_mode == WindowMode::FULLSCREEN) &&
            (m_WindowMode != WindowMode::FULLSCREEN)
        )
        {
            m_WindowMode = WindowMode::FULLSCREEN;

            MONITORINFO monitor_info = { sizeof(monitor_info) };

            GetMonitorInfoW(
                MonitorFromWindow(
                    m_Window,
                    MONITOR_DEFAULTTOPRIMARY),
                &monitor_info
            );

            if (m_OriginalWidth < m_NativeWidth || m_OriginalHeight < m_NativeHeight)
            {
                DEVMODE dm = {};
                dm.dmPelsWidth = m_OriginalWidth;
                dm.dmPelsHeight = m_OriginalHeight;
                dm.dmBitsPerPel = 32;
                dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
                dm.dmSize = sizeof(dm);

                ChangeDisplaySettings(&dm, 0);

                resolution_changed = true;
            }

            GetWindowPlacement(m_Window, &last_placement);

            GetMonitorInfoW(
                MonitorFromWindow(
                    m_Window,
                    MONITOR_DEFAULTTOPRIMARY),
                &monitor_info
            );

            SetWindowLongW(
                m_Window,
                GWL_STYLE,
                style & ~WS_OVERLAPPEDWINDOW
            );

            SetWindowPos(
                m_Window,
                HWND_TOP,
                monitor_info.rcMonitor.left, monitor_info.rcMonitor.top,
                monitor_info.rcMonitor.right - monitor_info.rcMonitor.left,
                monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top,
                SWP_NOOWNERZORDER | SWP_FRAMECHANGED
            );

            SetCursorMode(m_CursorMode);
        }

        if ((window_mode == WindowMode::WINDOWED) &&
            (m_WindowMode == WindowMode::FULLSCREEN)
        )
        {
            m_WindowMode = WindowMode::WINDOWED;

            if (resolution_changed)
            {
                DEVMODE dm = {};
                dm.dmPelsWidth = m_NativeWidth;
                dm.dmPelsHeight = m_NativeHeight;
                dm.dmBitsPerPel = 32;
                dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
                dm.dmSize = sizeof(dm);

                ChangeDisplaySettings(&dm, 0);

                resolution_changed = false;
            }

            SetWindowLongW(
                m_Window,
                GWL_STYLE,
                style | m_WindowStyle
            );
            SetWindowPlacement(m_Window, &last_placement);
            SetWindowPos(
                m_Window,
                NULL,
                0, 0, 0, 0,
                SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                SWP_NOOWNERZORDER | SWP_FRAMECHANGED
            );

            SetCursorMode(m_CursorMode);
        }
    }

    void WindowsWindow::SetContext(window_context wc)
    {
        m_Context.reset(wc.release());

        m_Context->Setup(this);

        m_Context->Activate();
    }

    void WindowsWindow::SetTitle(const std::wstring& title)
    {
        m_WindowTitle = title;
        SetWindowTextW(m_Window, m_WindowTitle.data());
    }

    bool WindowsWindow::EnsureAlive()
    {
        if (!m_Window)
        {
            OnError(error::NULL_WINDOW, "Cannot update a NULL window!");
            return false;
        }

        return true;
    }

    void WindowsWindow::PollEvents()
    {
        if (!EnsureAlive()) return;

        auto message = MSG();
        while (PeekMessageW(&message, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }
    }

    void WindowsWindow::SwapBuffers()
    {
        if (!EnsureAlive()) return;

        if (m_Context)
            m_Context->SwapBuffers();
    }

    void WindowsWindow::Update()
    {
        if (!EnsureAlive()) return;

        PollEvents();
        SwapBuffers();
    }

    bool WindowsWindow::ShouldClose()
    {
        return m_ShouldClose;
    }

    void WindowsWindow::Close()
    {
        if (m_Window)
        {
            DestroyWindow(m_Window);
            m_Window = nullptr;
        }
    }

    uint16_t WindowsWindow::GetWidth()
    {
        return m_RunningWidth;
    }

    uint16_t WindowsWindow::GetHeight()
    {
        return m_RunningHeight;
    }

    uint16_t WindowsWindow::GetMouseX()
    {
        return m_MouseX;
    }
    
    uint16_t WindowsWindow::GetMouseY()
    {
        return m_MouseY;
    }

    std::pair<uint16_t, uint16_t> WindowsWindow::GetMouseCoords()
    {
        return { m_MouseX, m_MouseY };
    }

    void WindowsWindow::OnError(error_callback cb)
    {
        m_ErrorCB = cb;
    }

    void WindowsWindow::OnKeyPressed(key_pressed_callback cb)
    {
        m_KeyPressedCB = cb;
    }

    void WindowsWindow::OnKeyReleased(key_released_callback cb)
    {
        m_KeyReleasedCB = cb;
    }

    void WindowsWindow::OnWindowResized(window_resized_callback cb)
    {
        m_WindowResizedCB = cb;
    }

    void WindowsWindow::OnWindowClosed(window_closed_callback cb)
    {
        m_WindowClosedCB = cb;
    }

    void WindowsWindow::OnMouseMoved(mouse_moved_callback cb)
    {
        m_MouseMovedCB = cb;
    }

    void WindowsWindow::OnMousePressed(mouse_pressed_callback cb)
    {
        m_MousePressedCB = cb;
    }

    void WindowsWindow::OnMouseReleased(mouse_released_callback cb)
    {
        m_MouseReleasedCB = cb;
    }

    void WindowsWindow::OnMouseScrolled(mouse_scrolled_callback cb)
    {
        m_MouseScrolledCB = cb;
    }

    void WindowsWindow::OnCharTyped(char_typed_callback cb)
    {
        m_CharTypedCB = cb;
    }

    bool WindowsWindow::IsKeyPressed(awml_key key_code)
    {
        return AWML_KEY_PRESSED_BIT &
            GetKeyState(static_cast<int>(key_code));
    }

    bool WindowsWindow::Minimized()
    {
        return !m_RunningWidth && !m_RunningHeight;
    }

    void WindowsWindow::SetCursorMode(CursorMode cursor_mode)
    {
        // TODO: change visible flag to m_CursorMode
        // TODO: make the new mode |= m_CursorMode while
        // XORing the old one.

        if (CursorMode::CAPTURED & cursor_mode)
        {
            RECT rect;
            GetClientRect(m_Window, &rect);

            POINT ul;
            ul.x = rect.left;
            ul.y = rect.top;

            POINT lr;
            lr.x = rect.right;
            lr.y = rect.bottom;

            MapWindowPoints(m_Window, nullptr, &ul, 1);
            MapWindowPoints(m_Window, nullptr, &lr, 1);

            rect.left = ul.x;
            rect.top = ul.y;

            rect.right = lr.x;
            rect.bottom = lr.y;

            ClipCursor(&rect);
        }
        else if (CursorMode::FREE & cursor_mode)
        {
            ClipCursor(NULL);
        }

        static bool hidden = false;

        if (!hidden && (CursorMode::HIDDEN & cursor_mode))
        {
            ShowCursor(false);
            hidden = true;
        }
        else if (hidden && (CursorMode::VISIBLE & cursor_mode))
        {
            ShowCursor(true);
            hidden = false;
        }
    }

    WindowsWindow::~WindowsWindow()
    {
        Close();
    }

    void WindowsWindow::RecalculateNative()
    {
        MONITORINFO monitor_info = { sizeof(monitor_info) };

        GetMonitorInfoW(
            MonitorFromWindow(
            m_Window,
            MONITOR_DEFAULTTOPRIMARY
            ), &monitor_info
        );

        m_NativeWidth = static_cast<uint16_t>(
            monitor_info.rcMonitor.right
            - monitor_info.rcMonitor.left
            );

        m_NativeHeight = static_cast<uint16_t>(
            monitor_info.rcMonitor.bottom
            - monitor_info.rcMonitor.top
        );
    }

    void WindowsWindow::OnError(error code, const std::string& msg)
    {
        if (m_ErrorCB)
            m_ErrorCB(code, msg);
    }

    void WindowsWindow::OnWindowResized(WORD width, WORD height)
    {
        // We have to check whether resizing
        // was set to true before because
        // making the window fullscreen
        // changes the resolution twice,
        // but we only want to get one event.
        static bool resizing = false;

        if (m_RunningWidth  &&
            m_RunningHeight &&
            width           &&
            height
        ) AWML_LIKELY
        {
            if (m_WindowMode == WindowMode::FULLSCREEN && !resizing)
            {
                resizing = true;
                return;
            }

            if (m_WindowMode == WindowMode::FULLSCREEN &&
                ((m_RunningWidth == width) &&
                (m_RunningHeight == height)))
            {
                resizing = false;
                return;
            }

            if ((m_RunningWidth == width) &&
                (m_RunningHeight == height))
                return;
        }

        m_RunningWidth = width;
        m_RunningHeight = height;

        if (m_WindowResizedCB)
            m_WindowResizedCB(width, height);

        resizing = false;
    }

    void WindowsWindow::OnWindowClosed()
    {
        if (m_WindowMode == WindowMode::FULLSCREEN)
        {
            SetResolution(m_NativeWidth, m_NativeHeight);
        }

        if (m_WindowClosedCB)
            m_WindowClosedCB();
    }

    void WindowsWindow::OnMouseMoved(WORD xpos, WORD ypos)
    {
        // The window can generate spurious mouse moved events,
        // so we have to handle that case here.
        if (xpos != m_MouseX || ypos != m_MouseY)
        {
            if (xpos > m_RunningWidth)
                xpos = 0;
            if (ypos > m_RunningHeight)
                ypos = 0;
            
            m_MouseX = xpos;
            m_MouseY = ypos;

            if (m_MouseMovedCB)
                m_MouseMovedCB(m_MouseX, m_MouseY);
        }
    }

    void WindowsWindow::OnMousePressed(awml_key code)
    {
        if (m_MousePressedCB)
            m_MousePressedCB(code);
    }

    void WindowsWindow::OnMouseReleased(awml_key code)
    {
        if (m_MouseReleasedCB)
            m_MouseReleasedCB(code);
    }

    void WindowsWindow::OnMouseScrolled(int16_t rotation, bool vertical)
    {
        rotation /= 12;

        // make sure we're not out of bounds
        if      (rotation >  10) rotation =  10;
        else if (rotation < -10) rotation = -10;

        if (m_MouseScrolledCB)
            m_MouseScrolledCB(rotation, vertical);
    }

    void WindowsWindow::OnKeyPressed(WPARAM key_code, bool repeated, uint16_t repeat_count)
    {
        if (m_KeyPressedCB)
            m_KeyPressedCB(
                static_cast<awml_key>(key_code),
                repeated, repeat_count
            );
    }

    void WindowsWindow::OnKeyReleased(WPARAM key_code)
    {
        if (m_KeyReleasedCB)
            m_KeyReleasedCB(
                static_cast<awml_key>(key_code)
            );
    }

    void WindowsWindow::OnCharTyped(wchar_t typed_char)
    {
        if (m_CharTypedCB)
            m_CharTypedCB(typed_char);
    }

    void WindowsWindow::SetResolution(uint16_t width, uint16_t height)
    {
        DEVMODE dm = {};
        dm.dmPelsWidth = width;
        dm.dmPelsHeight = height;
        dm.dmBitsPerPel = 32;
        dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
        dm.dmSize = sizeof(dm);

        ChangeDisplaySettings(&dm, 0);
    }

    void WindowsWindow::Resize(uint16_t width, uint16_t height)
    {
        if (!m_Window)
        {
            OnError(error::NULL_WINDOW, "Cannot resize a null window!");
            return;
        }

        if (m_WindowMode == WindowMode::FULLSCREEN)
        {
            OnError(error::GENERIC, "Cannot resize a fullscreen window!");
            return;
        }

        SetWindowPos(
            m_Window,
            NULL,
            0, 0, width, height,
            SWP_NOMOVE | SWP_NOZORDER |
            SWP_NOOWNERZORDER | SWP_FRAMECHANGED
        );
    }

    void* WindowsWindow::GetNativeHandle()
    {
        return m_Window;
    }

    LRESULT CALLBACK WindowsWindow::WindowEventHandler(
        HWND window,
        UINT message,
        WPARAM param_1,
        LPARAM param_2
    )
    {
        WindowsWindow* owner =
            reinterpret_cast<WindowsWindow*>(
                GetWindowLongPtrW(window, 0)
                );

        if (!owner)
            return DefWindowProcW(window, message, param_1, param_2);

        switch (message)
        {
        case WM_MOUSEMOVE:
            owner->OnMouseMoved(
                GET_X_LPARAM(param_2),
                GET_Y_LPARAM(param_2)
            );
            break;
        case WM_LBUTTONDOWN:
            owner->OnMousePressed(awml_key::MOUSE_LEFT);
            break;
        case WM_MBUTTONDOWN:
            owner->OnMousePressed(awml_key::MOUSE_MIDDLE);
            break;
        case WM_RBUTTONDOWN:
            owner->OnMousePressed(awml_key::MOUSE_RIGHT);
            break;
        case WM_XBUTTONDOWN:
            if (HIWORD(param_1) & AWML_MOUSE_X1_BIT)
                owner->OnMousePressed(awml_key::MOUSE_X1);
            else if (HIWORD(param_1) & AWML_MOUSE_X2_BIT)
                owner->OnMousePressed(awml_key::MOUSE_X2);
            break;
        case WM_LBUTTONUP:
            owner->OnMouseReleased(awml_key::MOUSE_LEFT);
            break;
        case WM_MBUTTONUP:
            owner->OnMouseReleased(awml_key::MOUSE_MIDDLE);
            break;
        case WM_RBUTTONUP:
            owner->OnMouseReleased(awml_key::MOUSE_RIGHT);
            break;
        case WM_XBUTTONUP:
            if (HIWORD(param_1) & AWML_MOUSE_X1_BIT)
                owner->OnMouseReleased(awml_key::MOUSE_X1);
            else if (HIWORD(param_1) & AWML_MOUSE_X2_BIT)
                owner->OnMouseReleased(awml_key::MOUSE_X2);
            break;
        case WM_MOUSEWHEEL:
            owner->OnMouseScrolled(
                GET_WHEEL_DELTA_WPARAM(param_1),
                true
            );
            break;
        case WM_MOUSEHWHEEL:
            owner->OnMouseScrolled(
                GET_WHEEL_DELTA_WPARAM(param_1),
                false
            );
            break;
        case WM_KEYDOWN:
            owner->OnKeyPressed(
                param_1,
                param_2 & AWML_REPEATED_BIT,
                param_2 & AWML_REPEAT_COUNT_MASK
            );
            break;
        case WM_KEYUP:
            owner->OnKeyReleased(param_1);
            break;
        case WM_CHAR:
            owner->OnCharTyped(static_cast<wchar_t>(param_1));
            break;
        case WM_SIZE:
            owner->OnWindowResized(
                LOWORD(param_2),
                HIWORD(param_2)
            );
            break;
        case WM_KILLFOCUS:
            if (owner->m_WindowMode == WindowMode::FULLSCREEN)
            {
                owner->SetResolution(
                    owner->m_NativeWidth,
                    owner->m_NativeHeight
                );

                ShowWindow(
                    owner->m_Window,
                    SW_MINIMIZE
                );
            }
            break;
        case WM_SETFOCUS:
            if (owner->m_WindowMode == WindowMode::FULLSCREEN)
            {
                owner->SetResolution(
                    owner->m_OriginalWidth,
                    owner->m_OriginalHeight
                );

                ShowWindow(
                    owner->m_Window,
                    SW_RESTORE
                );

                owner->SetCursorMode(
                    owner->m_CursorMode
                );
            }
            break;
        case WM_MOVE:
            if(!(owner->m_WindowMode == WindowMode::FULLSCREEN))
                owner->RecalculateNative();
            break;
        AWML_UNLIKELY case WM_DESTROY:
            owner->OnWindowClosed();
            break;
        AWML_UNLIKELY case WM_CLOSE:
            owner->m_ShouldClose = true;
            break;
        AWML_LIKELY default:
            return DefWindowProcW(window, message, param_1, param_2);
        }

        return 0;
    }

    HINSTANCE WindowsWindow::s_ThisInstance = GetModuleHandleW(NULL);
    uint16_t  WindowsWindow::s_WindowID = 1;
}
