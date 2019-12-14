#include <string>
#include <functional>
#include <memory>

#ifdef _WIN32
  #include <windows.h>
#endif

#include "awml_gl.h"

#include "key_codes.h"

namespace awml {

    enum class CursorMode : uint8_t
    {
        VISIBLE  = 1,
        HIDDEN   = 2,

        FREE     = 3, // Can be moved outside of the window
        CAPTURED = 4  // Cannot be moved outside of the window
    };

    inline CursorMode operator|(CursorMode l, CursorMode r)
    {
        return static_cast<CursorMode>(
            static_cast<uint8_t>(l) |
            static_cast<uint8_t>(r)
        );
    }

    inline uint8_t operator&(CursorMode l, CursorMode r)
    {
        return static_cast<uint8_t>(l) &
               static_cast<uint8_t>(r);
    }

    enum class WindowMode : uint8_t
    {
        UNSPECIFIED = 0, // do not use

        WINDOWED   = 1,
        FULLSCREEN = 2
    };

    enum class Context : uint8_t
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


    enum class error : uint16_t
    {
        OK          = 0,
        GENERIC     = 1,
        NULL_WINDOW = 2,
        BAD_ARGS    = 3
    };

    // A callback that gets called when an internal AWML error happens.
    // Parameters:
    // error -> One of the AWML error codes.
    // string -> A detailed description of the error.
    typedef std::function<void(error, const std::string&)>
        error_callback;

    inline const char* ErrorToString(error code)
    {
        switch (code)
        {
        case error::OK:          return "NO ERROR";
        case error::GENERIC:     return "GENERIC ERROR";
        case error::NULL_WINDOW: return "NULL WINDOW";
        case error::BAD_ARGS:    return "BAD ARGS";
        default:                 return "UNKNOWN ERROR";
        }
    }

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
            Context context = Context::NONE,
            WindowMode window_mode = WindowMode::WINDOWED,
            CursorMode cursor_mode = CursorMode::VISIBLE | CursorMode::FREE,
            bool resizable = false
        );

        Window(const Window& other) = delete;
        Window& operator=(const Window& other) = delete;
    protected:
        Window() {}

        virtual void SetContext(
            window_context wc
        ) = 0;
    public:
        virtual void Launch() = 0;

        virtual void OnError(
            error_callback cb
        ) = 0;

        virtual void OnKeyPressed(
            key_pressed_callback cb
        ) = 0;

        virtual void OnKeyReleased(
            key_released_callback cb
        ) = 0;

        virtual void OnWindowResized(
            window_resized_callback cb
        ) = 0;

        virtual void OnWindowClosed(
            window_closed_callback cb
        ) = 0;

        virtual void OnMouseMoved(
            mouse_moved_callback cb
        ) = 0;

        virtual void OnMousePressed(
            mouse_pressed_callback cb
        ) = 0;

        virtual void OnMouseReleased(
            mouse_released_callback cb
        ) = 0;

        virtual void OnMouseScrolled(
            mouse_scrolled_callback cb
        ) = 0;

        virtual void OnCharTyped(
            char_typed_callback cb
        ) = 0;

        virtual void SetTitle(const std::wstring& title) = 0;

        virtual void PollEvents() = 0;
        virtual void SwapBuffers() = 0;
        virtual void Update() = 0;

        virtual bool ShouldClose() = 0;

        virtual void Close() = 0;

        virtual uint16_t GetWidth() = 0;
        virtual uint16_t GetHeight() = 0;

        virtual uint16_t GetMouseX() = 0;
        virtual uint16_t GetMouseY() = 0;
        virtual std::pair<uint16_t, uint16_t> GetMouseCoords() = 0;

        virtual bool Minimized() = 0;

        virtual bool IsKeyPressed(awml_key key_code) = 0;

        virtual void SetCursorMode(CursorMode cursor_mode) = 0;

        virtual void SetWindowMode(WindowMode window_mode) = 0;

        virtual void Resize(uint16_t width, uint16_t height) = 0;

        // HWND for Windows
        // Display for Linux
        virtual void* GetNativeHandle() = 0;

        virtual ~Window() {}
    };
}
