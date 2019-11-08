#include <string>

#include "awml_xwindow.h"

namespace awml {

    XWindow::XWindow(
        const std::wstring& title,
        uint16_t width,
        uint16_t height,
        Context context,
        WindowMode window_mode,
        CursorMode cursor_mode,
        bool resizable
    ) : m_Width(width),
        m_Height(height),
        m_Context(context),
        m_WindowMode(window_mode),
        m_CursorMode(cursor_mode),
        m_ShouldClose(false)
    {
    }

    void XWindow::Launch() 
    {
        m_Connection = XOpenDisplay(NULL);

        if (!m_Connection)
            throw "xxx";

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

        XSelectInput(m_Connection, m_Window, KeyPressMask | ButtonPressMask | KeyReleaseMask | ButtonReleaseMask | StructureNotifyMask);

        XMapWindow(m_Connection, m_Window);

        Atom WM_DELETE_WINDOW = XInternAtom(m_Connection, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(m_Connection, m_Window, &WM_DELETE_WINDOW, 1);
    }

    void XWindow::SetContext(window_context wc) 
    {

    }

    void XWindow::Update() 
    {
        XPending(m_Connection);

        while (XQLength(m_Connection))
        {
            XNextEvent(m_Connection, &m_Event);

            switch (m_Event.type)
            {
            case ConfigureNotify:
                m_Width = m_Event.xconfigure.width;
                m_Height = m_Event.xconfigure.height;
                if (m_WindowResizedCB)
                    m_WindowResizedCB(m_Width, m_Height);
                break;
            case ButtonPress:
                if (m_MousePressedCB)
                    m_MousePressedCB(static_cast<awml_key>(m_Event.xbutton.button));
                break;
            case ButtonRelease:
                if (m_MouseReleasedCB)
                    m_MouseReleasedCB(static_cast<awml_key>(m_Event.xbutton.button));
                    break;
            case KeyPress:
                if (m_KeyPressedCB)
                    m_KeyPressedCB(static_cast<awml_key>(XKeycodeToKeysym(m_Connection, m_Event.xkey.keycode, 0)), true, 0);
		break;
            case KeyRelease:
                if (m_KeyReleasedCB)
                    m_KeyReleasedCB(static_cast<awml_key>(XKeycodeToKeysym(m_Connection, m_Event.xkey.keycode, 0)));
		break;
            case ClientMessage:
                m_ShouldClose = true;
            default:
                break;
            }
        }
    }

    bool XWindow::ShouldClose()
    {
        return m_ShouldClose;
    }

    void XWindow::Close()
    {

    }

    uint16_t XWindow::Width()
    {
        return m_Width;
    }

    uint16_t XWindow::Height()
    {
        return m_Height;
    }

    uint16_t XWindow::MouseX()
    {
        return m_MouseX;
    }

    uint16_t XWindow::MouseY()
    {
        return m_MouseY;
    }

    void XWindow::OnErrorFunc(
        error_callback cb
    )
    {

    }

    void XWindow::OnKeyPressedFunc(
        key_pressed_callback cb
    )
    {
        m_KeyPressedCB = cb;
    }

    void XWindow::OnKeyReleasedFunc(
        key_released_callback cb
    )
    {
        m_KeyReleasedCB = cb;
    }

    void XWindow::OnWindowResizedFunc(
        window_resized_callback cb
    )
    {
        m_WindowResizedCB = cb;
    }

    void XWindow::OnWindowClosedFunc(
        window_closed_callback cb
    )
    {

    }

    void XWindow::OnMouseMovedFunc(
        mouse_moved_callback cb
    )
    {

    }

    void XWindow::OnMousePressedFunc(
        mouse_pressed_callback cb
    )
    {
        m_MousePressedCB = cb;
    }

    void XWindow::OnMouseReleasedFunc(
        mouse_released_callback cb
    )
    {
       m_MouseReleasedCB = cb;
    }

    void XWindow::OnMouseScrolledFunc(
        mouse_scrolled_callback cb
    )
    {

    }

    void XWindow::OnCharTypedFunc(
        char_typed_callback cb
    )
    {

    }

    bool XWindow::Minimized()
    {
        return m_Width == 0 && m_Height == 0;
    }

    bool XWindow:: KeyPressed(awml_key key_code)
    {

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

    XWindow::~XWindow() 
    {
        XCloseDisplay(m_Connection);
    }
}
