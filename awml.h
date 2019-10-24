#include <string>
#include <functional>
#include <memory>

#include "utilities.h"

namespace awml {

    // A callback that gets called when a key is pressed.
    // Parameters: keycode, repeated, repeat_count.
    typedef std::function<void(awml_keycode, bool, uint16_t)>
        key_pressed_callback;

    // A callback that gets called when a key is released.
    // Parameters: keycode.
    typedef std::function<void(awml_keycode)>
        key_released_callback;

    class Window
    {
    public:
        typedef std::shared_ptr<Window>
            SharedWindow;
    public:
        static SharedWindow Create(
            const std::wstring& title,
            uint16_t width,
            uint16_t height
        );

        Window(const Window& other) = delete;
        Window& operator=(const Window& other) = delete;
    protected:
        Window() {}
    public:
        virtual void OnKeyPressedFunc(
            key_pressed_callback cb
        ) = 0;

        virtual void OnKeyReleasedFunc(
            key_released_callback cb
        ) = 0;

        virtual void PollEvents() = 0;

        virtual bool ShouldClose() = 0;

        virtual void Close() = 0;

        virtual uint16_t Width() = 0;
        virtual uint16_t Height() = 0;

        virtual uint16_t MouseX() = 0;
        virtual uint16_t MouseY() = 0;

        virtual bool KeyPressed(awml_keycode key_code) = 0;

        virtual ~Window() {}
    };
}
