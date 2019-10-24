#include <iostream>

#include "awml.h"

int main()
{
    auto window = awml::Window::Create(L"My Window", 1280, 720);

    window->OnKeyPressedFunc(
        [](awml_keycode key_code, bool repeated, uint16_t repeat_count)
        {
            std::cout << "Key "
                << key_code
                << " pressed. Repeated ("
                << repeated
                << ") " << "Repeat count ("
                << repeat_count
                << ")"
                << std::endl;
        }
    );

    window->OnKeyReleasedFunc(
        [](awml_keycode key_code)
        {
            std::cout << "Key "
                << key_code
                << " released."
                << std::endl;
        }
    );

    while (!window->ShouldClose())
    {
        window->PollEvents();
    }
}
