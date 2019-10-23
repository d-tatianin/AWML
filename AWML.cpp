#include <windows.h>
#include <windowsx.h>

#include <iostream>
#include <string>
#include <functional>

#include "utilities.h"
#include "key_codes.h"

namespace awml {

    class Window
    {
    private:
        static HINSTANCE s_ThisInstance;
        static uint16_t  s_WindowID;

        std::string m_ClassName;
        std::string m_WindowTitle;
        WNDCLASS m_WinProps = {};
        HWND m_Window = {};

        uint16_t m_Width;
        uint16_t m_Height;

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

        void Close()
        {
            if (m_Window)
            {
                DestroyWindow(m_Window);
                m_Window = nullptr;
            }
        }

        WORD Width()
        {
            return m_Width;
        }

        WORD Height()
        {
            return m_Height;
        }

        bool KeyPressed(awml_keycode key_code)
        {
            try
            {

            return AWML_KEY_PRESSED_BIT & GetKeyState(key_code);
            }
            catch (const std::exception& ex)
            {
                std::cout << ex.what();
            }
        }

        ~Window()
        {
            Close();
        }
    private:
        void OnWindowResized(WORD width, WORD height)
        {
            m_Width = width;
            m_Height = height;

            std::cout << "Resized the window, New size: " << m_Width << "x" << m_Height << std::endl;
        }

        void OnWindowClosed()
        {
            std::cout << "Window terminated" << std::endl;
        }

        void OnMouseMoved(WORD xpos, WORD ypos)
        {
            std::cout << "Mouse moved, x:" << xpos << " y:" << ypos << std::endl;
        }

        void OnMousePressed(UINT code)
        {
            std::cout << "Mouse button " << code << " pressed." << std::endl;
        }

        void OnMouseReleased(UINT code)
        {
            std::cout << "Mouse button " << code << " released." << std::endl;
        }

        void OnMouseWheelRotated(int16_t rotation)
        {
            std::cout << "Mouse wheel rotated " << rotation << std::endl;
        }

        void OnKeyPressed(WPARAM key_code, bool repeated)
        {
            std::cout << "Key " << (char)key_code << " pressed. Repeated (" << repeated << ")" << std::endl;
        }

        void OnKeyReleased(WPARAM key_code)
        {
            std::cout << "Key " << key_code << " released." << std::endl;
        }

        static LRESULT CALLBACK WindowEventHandler(HWND window, UINT message, WPARAM param_1, LPARAM param_2)
        {
            Window* owner =
                reinterpret_cast<Window*>(GetWindowLongPtrW(window, 0));

            if (!owner)
                return DefWindowProc(window, message, param_1, param_2);

            switch (message)
            {
            case WM_DESTROY:
                owner->OnWindowClosed();
                return 0;
            case WM_CLOSE:
                owner->m_ShouldClose = true;
                return 0;
            case WM_SIZE:
                owner->OnWindowResized(LOWORD(param_2), HIWORD(param_2));
                return 0;
            case WM_MOUSEMOVE:
                owner->OnMouseMoved(GET_X_LPARAM(param_2), GET_Y_LPARAM(param_2));
                return 0;
            case WM_LBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_RBUTTONDOWN:
                owner->OnMousePressed(message);
                return 0;
            case WM_LBUTTONUP:
            case WM_MBUTTONUP:
            case WM_RBUTTONUP:
                owner->OnMouseReleased(message);
                return 0;
            case WM_MOUSEWHEEL:
                owner->OnMouseWheelRotated(GET_WHEEL_DELTA_WPARAM(param_1));
                return 0;
            case WM_KEYDOWN:
                owner->OnKeyPressed(param_1, param_2 & AWML_REPEATED_BIT);
                return 0;
            case WM_KEYUP:
                owner->OnKeyReleased(param_1);
                return 0;
            default:
                return DefWindowProc(window, message, param_1, param_2);
            }
        }
    };

    HINSTANCE Window::s_ThisInstance = GetModuleHandle(NULL);
    uint16_t  Window::s_WindowID = 1;
}

int main()
{
    auto win = awml::Window("Test Window", 1280, 720);

    while (!win.ShouldClose())
    {
        if (win.KeyPressed(AWML_F10))
            std::cout << "ASD" << std::endl;


        win.PollEvents();
    }

    return 0;
}