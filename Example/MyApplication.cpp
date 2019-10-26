#include <iostream>

#include <AWML/awml.h>

int main()
{
    auto window = awml::Window::Create(
        L"My Window",
        1280, 720,
        awml::Context::OpenGL,
        false,
        true
    );

    window->OnKeyPressedFunc(
        [&window](awml_keycode key_code, bool repeated, uint16_t repeat_count)
        {
            if (key_code == AWML_F9)
                window->SetFullscreen(true);
            else if (key_code == AWML_ESCAPE || key_code == AWML_F8)
                window->SetFullscreen(false);

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

    window->OnWindowResizedFunc(
        [](uint16_t width, uint16_t height)
        {
            std::cout <<
                "Resized the window, New size: "
                << width
                << "x"
                << height
                << std::endl;
        }
    );

    window->OnWindowClosedFunc(
        []()
        {
            std::cout << "Window terminated!"
                << std::endl;
        }
    );

    window->OnMouseMovedFunc(
        [](uint16_t xpos, uint16_t ypos)
        {
            std::cout << "Mouse moved, x:"
                << xpos
                << " y:"
                << ypos
                << std::endl;
        }
    );

    window->OnMousePressedFunc(
        [](awml_keycode code)
        {
            std::cout << "Mouse button "
                << code
                << " pressed."
                << std::endl;
        }
    );

    window->OnMouseReleasedFunc(
        [](awml_keycode code)
        {
            std::cout << "Mouse button "
                << code
                << " released."
                << std::endl;
        }
    );

    window->OnMouseScrolledFunc(
        [](int16_t rotation, bool vertical)
        {
            static float red = 0.0f;
            static float green = 0.2f;
            static float blue = 0.5f;

            if (rotation > 0.0f)
            {
                red += 0.1f;
            }
            else
            {
                red -= 0.1f;
            }

            glClearColor(red, green, blue, 1.0f);

            std::cout << "Mouse scrolled "
                << rotation
                << (vertical ?
                    " verically" :
                    " horizontally")
                << std::endl;
        }
    );

    window->OnCharTypedFunc(
        [](wchar_t typed_char)
        {
            // PS: Don't use cout with wchar_t.
            std::cout << "Typed a char: "
                << static_cast<char>(typed_char)
                << std::endl;
        }
    );

    while (!window->ShouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        window->Update();
    }
}