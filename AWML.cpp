#include <Windows.h>
#include <iostream>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_CLOSE:
        if (MessageBox(hwnd, "Application", "Close the window?", MB_YESNO) == IDYES)
        {
            DestroyWindow(hwnd);
        }
        return 0;
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        
        //TextOut(hdc, 1, 1, "UEKK", 4);

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_SIZE:
    {
        int width = LOWORD(lParam);  // Macro to get the low-order word.
        int height = HIWORD(lParam); // Macro to get the high-order word.

        std::cout << "Resized the window, New size: " << width << "x" << height << std::endl;
        break;
    }
    
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

class Window
{
private:
    std::string m_ClassName;
    std::string m_WindowTitle;
    WNDCLASS m_WinProps = {};
    HWND m_Window = {};
public:
    Window(const std::string& title, uint16_t width, uint16_t height)
        : m_ClassName("class_1"), m_WindowTitle(title)
    {
        HINSTANCE h = GetModuleHandle(NULL);;

        m_WinProps.lpfnWndProc = WindowProc;
        m_WinProps.hInstance = h;
        m_WinProps.lpszClassName = m_ClassName.c_str();

        RegisterClass(&m_WinProps);

        auto x = GetLastError();

        m_Window = CreateWindowEx(
            0,
            m_ClassName.c_str(),
            m_WindowTitle.c_str(),
            WS_OVERLAPPEDWINDOW,
            10, 10, width + 16, height + 38,
            NULL,
            NULL,
            h,
            NULL
        );

        x = GetLastError();

        if (m_Window == NULL)
            // handle
            return;

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
};

int main()
{
    auto win = Window("Test Window", 1280, 720);

    while (true)
    {
        std::cout << "Polling..." << std::endl;
        win.PollEvents();
    }

    return 0;
}