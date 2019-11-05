#ifdef _WIN32
    #include "awml_windows.h"
#elif defined(__linux__)
    #include "awml_xwindow.h"
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
      #ifdef _WIN32
        return
            std::make_shared<WindowsWindow>(
                title,
                width,
                height,
                context,
                window_mode,
                cursor_mode,
                resizable
            );
      #elif defined(__linux__)
        return
	    std::make_shared<XWindow>(
		title,
                width,
                height,
                context,
                window_mode,
                cursor_mode,
                resizable
	   );
      #else
        #error Sorry, your platform is currently not supported!	
      #endif
    }
}
