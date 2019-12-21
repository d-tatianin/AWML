#include <string>
#include <clocale>
#include <cstring>
#include <stdexcept>

#include <X11/XKBlib.h>

#include "XWindow.h"

namespace awml {

    XOpenGLContext::XOpenGLContext()
        : m_Parent(nullptr),
        m_VisualInfo(),
        m_ColorMap(),
        m_Attribs(),
        m_OpenGLContext(),
        m_WinAttribs(),
        m_Props()
    {
    }

    bool XOpenGLContext::Setup(Window* self)
    {
        m_Parent = static_cast<XWindow*>(self);

        m_Props[0] = GLX_RGBA;
        m_Props[1] = GLX_DEPTH_SIZE;
        m_Props[2] = 24;
        m_Props[3] = GLX_DOUBLEBUFFER;
        m_Props[4] = None;

        m_VisualInfo = glXChooseVisual(
            m_Parent->m_Connection,
            0,
            m_Props
        );

        m_ColorMap = XCreateColormap(
            m_Parent->m_Connection,
            DefaultRootWindow(m_Parent->m_Connection),
            m_VisualInfo->visual,
            AllocNone
        );

        m_OpenGLContext =
            glXCreateContext(
                m_Parent->m_Connection,
                m_VisualInfo,
                NULL,
                GL_TRUE
            );

        if (!m_OpenGLContext)
        {
            m_Parent->NotifyError(error::CONTEXT, "Failed to create an OpenGL context!");
            return false;
        }

        return true;
    }

    bool XOpenGLContext::Activate()
    {
        if (!m_OpenGLContext)
        {
            m_Parent->NotifyError(error::CONTEXT, "Cannot activate a null context!");
            return false;
        }

        glXMakeCurrent(
            m_Parent->m_Connection,
            m_Parent->m_Window,
            m_OpenGLContext
        );

        return true;
    }

    void XOpenGLContext::SwapBuffers()
    {
        if (!m_OpenGLContext)
        {
            m_Parent->NotifyError(error::CONTEXT, "Cannot swap buffers for null context!");
            return;
        }

        glXSwapBuffers(
            m_Parent->m_Connection,
            m_Parent->m_Window
        );
    }

    bool XOpenGLContext::EnsureSetup()
    {
        return m_Parent;
    }

    void XOpenGLContext::MakeCurrent()
    {
        if (!EnsureSetup())
            return;

        if (!m_OpenGLContext)
        {
            m_Parent->NotifyError(error::CONTEXT, "Cannot make null context current!");
            return;
        }

        glXMakeCurrent(
            m_Parent->m_Connection,
            m_Parent->m_Window,
            m_OpenGLContext
        );
    }

    XOpenGLContext::~XOpenGLContext()
    {
        if (m_OpenGLContext)
        {
            glXMakeCurrent(
                m_Parent->m_Connection,
                None,
                NULL
            );

            glXDestroyContext(
                m_Parent->m_Connection,
                m_OpenGLContext
            );
        }

        m_Parent->Close();
    }

    XWindow::XWindow(
        const std::wstring& title,
        uint16_t width,
        uint16_t height,
        Context context,
        WindowMode window_mode,
        CursorMode cursor_mode,
        bool resizable
    ) : m_Title(title),
        m_Width(width),
        m_Height(height),
        m_Context(nullptr),
        m_ContextType(context),
        m_WindowMode(window_mode),
        m_CursorMode(cursor_mode),
        m_ShouldClose(false)
    {
        setlocale(LC_ALL, "en_US.utf8");
    }

    bool XWindow::EnsureAlive()
    {
        if (!m_Window || !m_Connection)
        {
            NotifyError(error::WINDOW, "Window has not yet been launched!");
            return false;
        }

        return true;
    }

    bool XWindow::Launch()
    {
        m_Connection = XOpenDisplay(NULL);
        XkbSetDetectableAutoRepeat(m_Connection, true, NULL);

        if (!m_Connection)
        {
            NotifyError(
                error::WINDOW,
                "Could not establish connection with the X server!"
            );

            return false;
        }

        uint32_t screen_num = DefaultScreen(m_Connection);
        uint32_t background_color = BlackPixel(m_Connection, screen_num);
        uint32_t border_color = WhitePixel(m_Connection, screen_num);

        m_Window = XCreateSimpleWindow(
            m_Connection,
            DefaultRootWindow(m_Connection),
            0, 0,
            m_Width,
            m_Height,
            2, border_color,
            background_color
        );

        if (!m_Window)
            NotifyError(
                error::WINDOW,
                "Failed to create a window!"
            );

        UpdateWindowTitle();

        XSelectInput(
            m_Connection,
            m_Window,
            KeyPressMask      |
            ButtonPressMask   |
            KeyReleaseMask    |
            ButtonReleaseMask |
            PointerMotionMask |
            StructureNotifyMask
        );

        XMapWindow(m_Connection, m_Window);

        Atom WM_DELETE_WINDOW = XInternAtom(m_Connection, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(m_Connection, m_Window, &WM_DELETE_WINDOW, 1);

        switch (m_ContextType)
        {
            case Context::OpenGL:
                if (!SetContext(std::make_unique<XOpenGLContext>()))
                    return false;
                break;
            default:
                break;
        }
    }

    void XWindow::UpdateWindowTitle()
    {
        size_t title_size = m_Title.size() * 2;
        char* title = static_cast<char*>(alloca(title_size));
        auto final_size = wcstombs(title, m_Title.data(), title_size);
        if (final_size == SIZE_MAX)
        {
            NotifyError(
                error::GENERIC,
                "Failed to convert the window title to UTF-8, title will not be set."
            );
            return;
        }
        title[final_size] = '\0';

        XStoreName(m_Connection, m_Window, title);

        Atom _net_wm_name = XInternAtom(m_Connection, "_NET_WM_NAME", false);
        Atom utf8_string = XInternAtom(m_Connection, "UTF8_STRING", false);
        XChangeProperty(
            m_Connection,
            m_Window,
            _net_wm_name,
            utf8_string,
            8, PropModeReplace,
            reinterpret_cast<unsigned char*>(title),
            title_size
        );
    }

    void XWindow::NotifyError(error code, const std::string& message)
    {
        if (m_ErrorCB)
            m_ErrorCB(code, message);
        else
            // TODO: Replace with an awml specific error class
            throw std::runtime_error(message);
    }

    bool XWindow::SetContext(window_context wc) 
    {
        m_Context.reset(wc.release());

        if (!m_Context->Setup(this))
            return false;

        if (!m_Context->Activate())
            return false;

        return true;
    }

    void XWindow::PollEvents()
    {
        XPending(m_Connection);

        while (XQLength(m_Connection))
        {
            XNextEvent(m_Connection, &m_Event);

            switch (m_Event.type)
            {
            case ConfigureNotify:

                if (m_Event.xconfigure.width == m_Width &&
                    m_Event.xconfigure.height == m_Height)
                    return;

                m_Width = m_Event.xconfigure.width;
                m_Height = m_Event.xconfigure.height;

                if (m_WindowResizedCB)
                    m_WindowResizedCB(m_Width, m_Height);

                break;

            case ButtonPress:
            {
                auto button = m_Event.xbutton.button;

                if (button == 4 || button == 5)
                {
                    if (m_MouseScrolledCB)
                        m_MouseScrolledCB(
                            button == 4 ? 10 : -10, true
                        );
                    break;
                }
                else if (button == 6 || button == 7)
                {
                    if (m_MouseScrolledCB)
                        m_MouseScrolledCB(button == 6 ? 10 : -10, false);
                    break;
                }
                else if (m_MousePressedCB)
                    m_MousePressedCB(
                        static_cast<awml_key>(button)
                    );

                break;
            }
            case ButtonRelease:
            {
                auto button = m_Event.xbutton.button;

                if (button == 4 ||
                    button == 5 ||
                    button == 6 ||
                    button == 7
                    )
                    break;

                if (m_MouseReleasedCB)
                    m_MouseReleasedCB(
                        static_cast<awml_key>(button)
                    );

                break;
            }
            case KeyPress:
            {
                wchar_t typed_char = GetTypedChar();

                if (typed_char && m_CharTypedCB)
                    m_CharTypedCB(
                        typed_char
                    );

                auto key = NormalizeKeyPress();

                if (m_KeyPressedCB)
                    m_KeyPressedCB(
                        key,
                        GetKeyRepeatCount(key),
                        GetKeyRepeatCount(key)
                    );

                IncremetRepeatCount(key);

                break;
            }
            case KeyRelease:
            {
                auto key = NormalizeKeyPress();

                if (m_KeyReleasedCB)
                    m_KeyReleasedCB(key);

                ResetRepeatCount(key);

                break;
            }
            case MotionNotify:
                if (m_MouseMovedCB)
                    m_MouseMovedCB(
                        m_Event.xmotion.x,
                        m_Event.xmotion.y
                    );

                break;

            case ClientMessage:
                m_ShouldClose = true;

                break;

            default:
                break;
            }
        }
    }

    void XWindow::SwapBuffers()
    {
        if (m_Context)
            m_Context->SwapBuffers();
    }

    void XWindow::MakeCurrent()
    {
        if (!EnsureAlive())
            return;

        if (m_Context)
            m_Context->MakeCurrent();
        else
            NotifyError(error::CONTEXT, "Cannot make NULL context current!");
    }

    void XWindow::Update() 
    {
        PollEvents();
        SwapBuffers();
    }

    void XWindow::SetTitle(const std::wstring& title)
    {
        m_Title = title;
        UpdateWindowTitle();
    }

    bool XWindow::ShouldClose()
    {
        return m_ShouldClose;
    }

    void XWindow::Close()
    {
        static bool closed = false;

        // TODO: Change internal state
        // to prevent using the window
        // after manually calling close.
        if (!closed)
        {
            XCloseDisplay(m_Connection);
            closed = true;
        }
    }

    uint16_t XWindow::GetWidth()
    {
        return m_Width;
    }

    uint16_t XWindow::GetHeight()
    {
        return m_Height;
    }

    uint16_t XWindow::GetMouseX()
    {
        return m_MouseX;
    }

    uint16_t XWindow::GetMouseY()
    {
        return m_MouseY;
    }

    std::pair<uint16_t, uint16_t> XWindow::GetMouseCoords()
    {
        return { m_MouseX, m_MouseY };
    }

    void XWindow::OnError(
        error_callback cb
    )
    {
        m_ErrorCB = cb;
    }

    void XWindow::OnKeyPressed(
        key_pressed_callback cb
    )
    {
        m_KeyPressedCB = cb;
    }

    void XWindow::OnKeyReleased(
        key_released_callback cb
    )
    {
        m_KeyReleasedCB = cb;
    }

    void XWindow::OnWindowResized(
        window_resized_callback cb
    )
    {
        m_WindowResizedCB = cb;
    }

    void XWindow::OnWindowClosed(
        window_closed_callback cb
    )
    {
        m_WindowClosedCB = cb;
    }

    void XWindow::OnMouseMoved(
        mouse_moved_callback cb
    )
    {
        m_MouseMovedCB = cb;
    }

    void XWindow::OnMousePressed(
        mouse_pressed_callback cb
    )
    {
        m_MousePressedCB = cb;
    }

    void XWindow::OnMouseReleased(
        mouse_released_callback cb
    )
    {
       m_MouseReleasedCB = cb;
    }

    void XWindow::OnMouseScrolled(
        mouse_scrolled_callback cb
    )
    {
       m_MouseScrolledCB = cb;
    }

    void XWindow::OnCharTyped(
        char_typed_callback cb
    )
    {
        m_CharTypedCB = cb;
    }

    bool XWindow::Minimized()
    {
        return m_Width == 0 && m_Height == 0;
    }

    bool XWindow::IsKeyPressed(awml_key key_code)
    {
        char key_map[32];

        auto xcode = XKeysymToKeycode(
            m_Connection,
            static_cast<KeySym>(key_code)
        );

        XQueryKeymap(m_Connection, key_map);

        return key_map[xcode/8] & (0x1<<(xcode%8));
    }

    void XWindow::SetCursorMode(CursorMode cursor_mode)
    {

    }

    void XWindow::SetWindowMode(WindowMode window_mode)
    {

    }

    void XWindow::Resize(uint16_t width, uint16_t height)
    {

    }

    void* XWindow::GetNativeHandle()
    {
        return m_Connection;
    }

    awml_key XWindow::NormalizeKeyPress()
    {
        auto key_sym = XkbKeycodeToKeysym(
            m_Connection,
            m_Event.xkey.keycode,
            0,
            m_Event.xkey.state & ShiftMask ? 1 : 0
        );

        KeySym lower_sym;
        KeySym upper_sym;

        XConvertCase(key_sym, &lower_sym, &upper_sym);

        return static_cast<awml_key>(lower_sym);
    }

    wchar_t XWindow::GetTypedChar()
    {
        auto key_sym = XkbKeycodeToKeysym(
            m_Connection,
            m_Event.xkey.keycode,
            0,
            m_Event.xkey.state & ShiftMask ? 1 : 0
        );

        // This is not how you actually get the typed char
        // To be fixed later. (its like 2k lines to get this working btw)
        //static_cast<wchar_t>(*XKeysymToString(key_sym))

        return static_cast<wchar_t>(0);
    }

    uint8_t XWindow::GetKeyRepeatCount(awml_key code)
    {
        return m_RepeatCount.count(code) ?
            m_RepeatCount[code] : 0;
    }

    void XWindow::IncremetRepeatCount(awml_key code)
    {
        if (m_RepeatCount.count(code))
        {
            if (m_RepeatCount[code] != UINT8_MAX)
                m_RepeatCount[code]++;
        }
        else
            m_RepeatCount[code] = 1;
    }

    void XWindow::ResetRepeatCount(awml_key code)
    {
        if (m_RepeatCount.count(code))
            m_RepeatCount[code] = 0;
    }

    XWindow::~XWindow()
    {
        if (!m_Context)
            Close();
    }
}
