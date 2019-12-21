#include <string>
#include <clocale>
#include <cstring>
#include <stdexcept>

#include <X11/XKBlib.h>

#include "XWindow.h"
#include "XGL.h"

namespace awml {

    XOpenGLContext::XOpenGLContext()
        : m_Parent(nullptr),
        m_VisualInfo(),
        m_ColorMap(),
        m_Attribs(),
        m_OpenGLContext(),
        m_WinAttribs(),
        m_BestFBC()
    {
    }

    bool XOpenGLContext::Setup(Window* self)
    {
        m_Parent = static_cast<XWindow*>(self);

        GLint visual_attribs[] =
        {
            GLX_X_RENDERABLE,  True,
            GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
            GLX_RENDER_TYPE,   GLX_RGBA_BIT,
            GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
            GLX_RED_SIZE,      8,
            GLX_GREEN_SIZE,    8,
            GLX_BLUE_SIZE,     8,
            GLX_ALPHA_SIZE,    8,
            GLX_DEPTH_SIZE,    24,
            GLX_STENCIL_SIZE,  8,
            GLX_DOUBLEBUFFER,  True,
            None
        };

        int glxmajor;
        int glxminor;

        if (!glXQueryVersion(m_Parent->m_Connection, &glxmajor, &glxminor) ||
            ((glxmajor == 1) && (glxminor < 3)) || (glxmajor < 1))
        {
            m_Parent->NotifyError(error::CONTEXT, "Invalid OpenGL version");
            return false;
        }

        GLint fb_count;
        GLXFBConfig* fbc =
            glXChooseFBConfig(
                m_Parent->m_Connection,
                DefaultScreen(m_Parent->m_Connection),
                visual_attribs,
                &fb_count
            );

        if (!fbc)
        {
            m_Parent->NotifyError(error::CONTEXT, "Failed to setup OpenGL context");
            return false;
        }

        int best_fbc       = -1,
            worst_fbc      = -1,
            best_num_samp  = -1,
            worst_num_samp = 999;

        int i;
        for (i = 0; i < fb_count; ++i)
        {
            XVisualInfo* vi = glXGetVisualFromFBConfig(m_Parent->m_Connection, fbc[i]);
            if (vi)
            {
                int samp_buf, samples;
                glXGetFBConfigAttrib(m_Parent->m_Connection, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf);
                glXGetFBConfigAttrib(m_Parent->m_Connection, fbc[i], GLX_SAMPLES, &samples);

                if (best_fbc < 0 || samp_buf && samples > best_num_samp)
                {
                    best_fbc = i;
                    best_num_samp = samples;
                }

                if (worst_fbc < 0 || !samp_buf || samples < worst_num_samp)
                {
                    worst_fbc = i;
                    worst_num_samp = samples;
                }
            }

            XFree(vi);
        }

        m_BestFBC = fbc[best_fbc];

        XFree(fbc);

        m_VisualInfo = 
            glXGetVisualFromFBConfig(
                m_Parent->m_Connection,
                m_BestFBC
            );

        m_ColorMap = XCreateColormap(
            m_Parent->m_Connection,
            RootWindow(m_Parent->m_Connection, m_VisualInfo->screen),
            m_VisualInfo->visual,
            AllocNone
        );

        m_Attribs.colormap = m_ColorMap;
        m_Attribs.background_pixmap = None;
        m_Attribs.border_pixel = 0;
        m_Attribs.event_mask = StructureNotifyMask;

        return true;
    }

    XVisualInfo* XOpenGLContext::GetVisualInfo()
    {
        if (!EnsureSetup())
            return nullptr;

        if (!m_VisualInfo)
        {
            m_Parent->NotifyError(error::CONTEXT, "Failed to create an OpenGL context!");
            return nullptr;
        }

        return m_VisualInfo;
    }

    XSetWindowAttributes* XOpenGLContext::GetAttribsPtr()
    {
        if (!EnsureSetup())
            return nullptr;

        if (!m_Attribs.colormap)
        {
            m_Parent->NotifyError(error::CONTEXT, "Failed to create an OpenGL context!");
            return nullptr;
        }

        return &m_Attribs;
    }
    
    bool XOpenGLContext::Activate()
    {
        if (!EnsureSetup())
            return false;

        int context_attribs[] =
        {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 1,
            GLX_CONTEXT_MINOR_VERSION_ARB, 0,
            None
        };

        if (!glLoader::Init())
        {
            m_Parent->NotifyError(error::CONTEXT, "Failed to create an OpenGL context!");
            return false;
        }

        m_OpenGLContext = 
            glXCreateContextAttribsARB(
                m_Parent->m_Connection,
                m_BestFBC, 0,
                True,
                context_attribs
            );

        if (!m_OpenGLContext)
        {
            m_Parent->NotifyError(error::CONTEXT, "Failed to create an OpenGL context!");
            return false;
        }

        glXMakeCurrent(
            m_Parent->m_Connection,
            m_Parent->m_Window,
            m_OpenGLContext
        );

        auto glversion = glGetString(GL_VERSION);
        char major = glversion[0] - '0';
        char minor = glversion[2] - '0';

        if (!glLoader::LoadVersion(major, minor))
            return false;

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

        if (m_ContextType == Context::NONE)
        {
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
        }
        else if (m_ContextType == Context::OpenGL)
        {
            m_Context = std::make_unique<XOpenGLContext>();
            m_Context->Setup(this);

            auto visual_info = ((XOpenGLContext*)m_Context.get())->GetVisualInfo();
            auto attribs_ptr = ((XOpenGLContext*)m_Context.get())->GetAttribsPtr();

            if (!visual_info || !attribs_ptr)
                return false;

            m_Window = XCreateWindow(
                m_Connection,
                RootWindow(m_Connection, visual_info->screen),
                0, 0,
                m_Width,
                m_Height,
                0, visual_info->depth,
                InputOutput, visual_info->visual,
                CWBorderPixel | CWColormap | CWEventMask,
                attribs_ptr
            );
        }

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

        if (!m_Context->Activate())
            return false;

        return true;
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
        // Dynamically switching contexts is currently
        // not supported. Hopefully later?
        // m_Context.reset(wc.release());

        

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
