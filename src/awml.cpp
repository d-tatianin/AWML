#ifdef _WIN32
    #include "awml_windows.h"
    #define AWML_NATIVE_WINDOW WindowsWindow
#elif defined(__linux__)
    #include "awml_xwindow.h"
    #define AWML_NATIVE_WINDOW XWindow
#else
    #error Sorry, your platform is currently not supported!
    #define AWML_NATIVE_WINDOW
#endif

namespace awml {

    Window::SharedWindow Window::Create(
        const std::wstring& title,
        uint16_t width,
        uint16_t height,
        Context context,
        WindowMode window_mode,
        CursorMode cursor_mode,
        bool resizable
    )
    {
        return
            std::make_shared<AWML_NATIVE_WINDOW>(
                title,
                width,
                height,
                context,
                window_mode,
                cursor_mode,
                resizable
            );
    }
}
