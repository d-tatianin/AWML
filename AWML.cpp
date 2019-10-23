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

        std::wstring m_ClassName;
        std::wstring m_WindowTitle;
        WNDCLASSW m_WinProps;
        HWND m_Window;

        uint16_t m_Width;
        uint16_t m_Height;

        uint16_t m_MouseX;
        uint16_t m_MouseY;

        bool m_ShouldClose;

    public:
        Window(
            const std::wstring& title,
            uint16_t width,
            uint16_t height
        ) : m_ClassName(title),
            m_WindowTitle(title),
            m_WinProps(),
            m_Window(),
            m_Width(0),
            m_Height(0),
            m_MouseX(0),
            m_MouseY(0),
            m_ShouldClose(false)
        {
            m_ClassName += std::to_wstring(s_WindowID++);

            m_WinProps.lpfnWndProc = WindowEventHandler;
            m_WinProps.hInstance = s_ThisInstance;
            m_WinProps.lpszClassName = m_ClassName.c_str();
            m_WinProps.cbWndExtra = sizeof(Window*);
            RegisterClassW(&m_WinProps);

            m_Window = CreateWindowExW(
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

        static void PollEvents()
        {
            auto message = MSG();
            while (PeekMessageW(&message, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&message);
                DispatchMessageW(&message);
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

        uint16_t Width()
        {
            return m_Width;
        }

        uint16_t Height()
        {
            return m_Height;
        }

        uint16_t MouseX()
        {
            return m_MouseX;
        }

        uint16_t MouseY()
        {
            return m_MouseY;
        }

        bool KeyPressed(awml_keycode key_code)
        {
            return AWML_KEY_PRESSED_BIT & GetKeyState(key_code);
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
            if (xpos != m_MouseX || ypos != m_MouseY)
            {
                m_MouseX = xpos;
                m_MouseY = ypos;

                std::cout << "Mouse moved, x:" << xpos << " y:" << ypos << std::endl;
            }
        }

        void OnMousePressed(UINT code)
        {
            std::cout << "Mouse button " << code << " pressed." << std::endl;
        }

        void OnMouseReleased(UINT code)
        {
            std::cout << "Mouse button " << code << " released." << std::endl;
        }

        void OnMouseScrolled(int16_t rotation)
        {
            std::cout << "Mouse wheel rotated " << rotation << std::endl;
        }

        void OnKeyPressed(WPARAM key_code, bool repeated, uint16_t repeat_count)
        {
            std::cout << "Key " << (char)key_code << " pressed. Repeated (" << repeated << ") " << "Repeat count (" << repeat_count << ")" << std::endl;
        }

        void OnKeyReleased(WPARAM key_code)
        {
            std::cout << "Key " << key_code << " released." << std::endl;
        }

        void OnCharTyped(wchar_t typed_char)
        {
        }

        static LRESULT CALLBACK WindowEventHandler(
            HWND window,
            UINT message,
            WPARAM param_1,
            LPARAM param_2
        )
        {
            Window* owner =
                reinterpret_cast<Window*>(GetWindowLongPtrW(window, 0));

            if (!owner)
                return DefWindowProcW(window, message, param_1, param_2);

            switch (message)
            {
            case WM_DESTROY:
                owner->OnWindowClosed();
                return 0;
            case WM_CLOSE:
                owner->m_ShouldClose = true;
                return 0;
            case WM_SIZE:
                owner->OnWindowResized(
                    LOWORD(param_2),
                    HIWORD(param_2)
                );
                return 0;
            case WM_MOUSEMOVE:
                owner->OnMouseMoved(
                    GET_X_LPARAM(param_2),
                    GET_Y_LPARAM(param_2)
                );
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
                owner->OnMouseScrolled(
                    GET_WHEEL_DELTA_WPARAM(param_1)
                );
                return 0;
            case WM_KEYDOWN:
                owner->OnKeyPressed(
                    param_1,
                    param_2 & AWML_REPEATED_BIT,
                    param_2 & AWML_REPEAT_COUNT_MASK
                );
                return 0;
            case WM_KEYUP:
                owner->OnKeyReleased(param_1);
                return 0;
            case WM_CHAR:
                owner->OnCharTyped(static_cast<wchar_t>(param_1));
                return 0;
            default:
                return DefWindowProcW(window, message, param_1, param_2);
            }
        }
    };

    HINSTANCE Window::s_ThisInstance = GetModuleHandleW(NULL);
    uint16_t  Window::s_WindowID = 1;
}

int main()
{
    auto win = awml::Window(L"MyWindow", 1280, 720);

    while (!win.ShouldClose())
    {
        win.PollEvents();
    }

    return 0;
}