#ifdef _WIN32
    #include "awml_windows.h"
#endif

namespace awml {

    Window::SharedWindow Window::Create(
        const std::wstring& title,
        uint16_t width,
        uint16_t height
    )
    {
      #ifdef _WIN32
        return std::make_shared<WindowsWindow>(title, width, height);
      #endif
    }
}
