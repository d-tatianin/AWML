#include <Windows.h>
#include <iostream>
#include <string>
#include <functional>

class Window
{
private:
    static HINSTANCE s_ThisInstance;
    static uint16_t  s_WindowID;

    std::string m_ClassName;
    std::string m_WindowTitle;
    WNDCLASS m_WinProps = {};
    HWND m_Window = {};

    bool m_ShouldClose;
public:
    Window(const std::string& title, uint16_t width, uint16_t height)
        : m_ClassName(title), m_WindowTitle(title), m_ShouldClose(false)
    {
        m_ClassName += std::to_string(s_WindowID++);

        m_WinProps.lpfnWndProc = WindowEventHandler;
        m_WinProps.hInstance = s_ThisInstance;
        m_WinProps.lpszClassName = m_ClassName.c_str();
        m_WinProps.cbWndExtra = sizeof(Window*);
        RegisterClass(&m_WinProps);

        m_Window = CreateWindowEx(
            0,
            m_ClassName.c_str(),
            m_WindowTitle.c_str(),
            WS_OVERLAPPEDWINDOW,
            10, 10, width, height,
            NULL,
            NULL,
            s_ThisInstance,
            NULL
        );

        if (m_Window == NULL)
            throw std::exception("Could not create a window!");

        SetWindowLongPtrW(m_Window, 0, reinterpret_cast<LONG_PTR>(this));

        ShowWindow(m_Window, SW_NORMAL);
    }

    void PollEvents()
    {
        MSG msg = { };
        while (PeekMessage(&msg, m_Window, 0, 0, PM_REMOVE) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    bool ShouldClose()
    {
        return m_ShouldClose;
    }
private:
    static LRESULT CALLBACK WindowEventHandler(HWND window, UINT message, WPARAM param_1, LPARAM param_2)
    {
        Window* creator =
            reinterpret_cast<Window*>(GetWindowLongPtrW(window, 0));

        switch (message)
        {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_CLOSE:
            if (MessageBox(window, "Application", "Close the window?", MB_YESNO) == IDYES)
            {
                DestroyWindow(window);
            }
            creator->m_ShouldClose = true;
            return 0;
            break;
        case WM_SIZE:
            int width = LOWORD(param_2);
            int height = HIWORD(param_2);

            std::cout << "Resized the window, New size: " << width << "x" << height << std::endl;
            break;
        }

        return DefWindowProc(window, message, param_1, param_2);
    }
};

HINSTANCE Window::s_ThisInstance = GetModuleHandle(NULL);
uint16_t  Window::s_WindowID = 1;

int main()
{
    auto win = Window("Test Window", 1280, 720);

    while (!win.ShouldClose())
    {
        win.PollEvents();
    }

    return 0;
}