#include <string>
#include <functional>
#include <memory>

#ifdef _WIN32
  #include <windows.h>
  #include <GL/gl.h>
#endif

#include "key_codes.h"

namespace awml {

    enum class Context
    {
        NONE   = 0,
        OpenGL = 1
    };

    // A callback that gets called when a key is pressed.
    // Parameters:
    // awml_key -> Key that was pressed.
    // bool -> Whether it's a repeated event.
    // uint16_t -> Repeat count (usually 1 if polled often enough).
    typedef std::function<void(awml_key, bool, uint16_t)>
        key_pressed_callback;

    // A callback that gets called when a key is released.
    // Parameters:
    // awml_key -> Key that was released.
    typedef std::function<void(awml_key)>
        key_released_callback;

    // A callback that gets called whenever window is resized
    // Parameters:
    // uint16_t -> New window width.
    // uint16_t -> New window height.
    typedef std::function<void(uint16_t, uint16_t)>
        window_resized_callback;

    // A callback that gets called once the window is closed.
    // Parameters:
    // NONE
    typedef std::function<void()>
        window_closed_callback;

    // A callback that gets called whenever mouse is moved.
    // Parameters:
    // uint16_t -> New mouse x coordinate.
    // uint16_t -> New mouse y coordinate.
    typedef std::function<void(uint16_t, uint16_t)>
        mouse_moved_callback;

    // A callback that gets called whenever a mouse button is pressed.
    // Parameters:
    // awml_key -> Mouse button that was pressed. 
    typedef std::function<void(awml_key)>
        mouse_pressed_callback;

    // A callback that gets called whenever a mouse button is released.
    // Parameters:
    // awml_key -> Mouse button that was released. 
    typedef std::function<void(awml_key)>
        mouse_released_callback;

    // A callback that gets called when mouse wheel is scrolled.
    // Parameters:
    // int16_t -> A value between [-10; 10] that indicates the delta of the scroll.
    // bool -> Set to true if it's a vertical scroll, false if horizontal.
    typedef std::function<void(int16_t, bool)>
        mouse_scrolled_callback;

    // A callback that gets called after a character is typed.
    // Parameters:
    // wchar_t -> The character typed by the user.
    typedef std::function<void(wchar_t)>
        char_typed_callback;

    class Window;

    class GraphicsContext
    {
    public:
        virtual bool Setup(Window* self) = 0;
        virtual bool Activate() = 0;
        virtual void SwapBuffers() = 0;
        virtual ~GraphicsContext() {}
    };

    typedef std::unique_ptr<GraphicsContext>
        window_context;

    class Window
    {
    public:
        typedef std::shared_ptr<Window>
            SharedWindow;
    public:
        static SharedWindow Create(
            const std::wstring& title,
            uint16_t width,
            uint16_t height,
            Context context,
            bool resizable = true,
            bool fullscreen = false
        );

        Window(const Window& other) = delete;
        Window& operator=(const Window& other) = delete;
    protected:
        Window() {}

        virtual void SetContext(
            window_context wc
        ) = 0;
    public:
        virtual void OnKeyPressedFunc(
            key_pressed_callback cb
        ) = 0;

        virtual void OnKeyReleasedFunc(
            key_released_callback cb
        ) = 0;

        virtual void OnWindowResizedFunc(
            window_resized_callback cb
        ) = 0;

        virtual void OnWindowClosedFunc(
            window_closed_callback cb
        ) = 0;

        virtual void OnMouseMovedFunc(
            mouse_moved_callback cb
        ) = 0;

        virtual void OnMousePressedFunc(
            mouse_pressed_callback cb
        ) = 0;

        virtual void OnMouseReleasedFunc(
            mouse_released_callback cb
        ) = 0;

        virtual void OnMouseScrolledFunc(
            mouse_scrolled_callback cb
        ) = 0;

        virtual void OnCharTypedFunc(
            char_typed_callback cb
        ) = 0;

        virtual void Update() = 0;

        virtual bool ShouldClose() = 0;

        virtual void Close() = 0;

        virtual uint16_t Width() = 0;
        virtual uint16_t Height() = 0;

        virtual uint16_t MouseX() = 0;
        virtual uint16_t MouseY() = 0;

        virtual bool Minimized() = 0;

        virtual void CaptureCursor(bool mode) = 0;
        virtual void HideCursor(bool mode) = 0;

        virtual bool KeyPressed(awml_key key_code) = 0;

        virtual void SetFullscreen(bool mode) = 0;

        virtual ~Window() {}
    };
}
