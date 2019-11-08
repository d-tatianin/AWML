#include <iostream>
#include <math.h>

#include <AWML/awml.h>

int main()
{
    auto window = awml::Window::Create(
        L"My Window",
        1280, 720,
        awml::Context::OpenGL,
        awml::WindowMode::WINDOWED,
        awml::CursorMode::FREE | awml::CursorMode::VISIBLE,
        true
    );

    window->OnKeyPressedFunc(
        [&window](awml_key key_code, bool repeated, uint16_t repeat_count)
        {
            if (key_code == awml_key::H)
                window->SetCursorMode(awml::CursorMode::CAPTURED);
            else if (key_code == awml_key::G)
                window->SetCursorMode(awml::CursorMode::VISIBLE);
            else if (key_code == awml_key::F9)
                window->SetWindowMode(awml::WindowMode::FULLSCREEN);
            else if (key_code == awml_key::ESCAPE || key_code == awml_key::F8)
                window->SetWindowMode(awml::WindowMode::WINDOWED);
            else if (key_code == awml_key::Q)
                window->Resize(window->Width() + 100, window->Height() + 100);
            else if (key_code == awml_key::E)
                window->Resize(window->Width() - 100, window->Height() - 100);

            std::cout
                << key_code
                << " pressed. Repeated ("
                << repeated
                << ") " << "Repeat count ("
                << repeat_count
                << ")" << static_cast<uint16_t>(key_code)
                << std::endl;
        }
    );

    window->OnKeyReleasedFunc(
        [](awml_key key_code)
        {
            std::cout
                << key_code
                << " released."
                << std::endl;
        }
    );

    window->OnWindowResizedFunc(
        [&window](uint16_t width, uint16_t height)
        {
            glViewport(0, 0, window->Width(), window->Height());

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
        [&window](awml_key code)
        {
            if (code == awml_key::MOUSE_LEFT)
                window->SetCursorMode(awml::CursorMode::HIDDEN);
            else if(code == awml_key::MOUSE_RIGHT)
                window->SetCursorMode(awml::CursorMode::VISIBLE);

            std::cout
                << code
                << " pressed."
                << std::endl;
        }
    );

    window->OnMouseReleasedFunc(
        [](awml_key code)
        {
            std::cout
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

    window->OnErrorFunc(
        [](awml::error code, const std::string& msg)
        {
            std::cout << "AWML ERROR: "
                << static_cast<uint16_t>(code)
                << " " << msg
                << std::endl;
        }
    );

    window->Launch();

    while (!window->ShouldClose())
    {
        static float moving_x = -1.0f;

        if (moving_x - 1.0f > 1.0f)
            moving_x = -1.0f;

        moving_x += 0.01f;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glColor3f(
            -sin(moving_x - 1.0f),
            -cos(moving_x * 0.44f),
             sin(moving_x * 0.24f)
        );

        glBegin(GL_TRIANGLES);
        glVertex3f(-1.0f  + moving_x, -0.25f, 0.0f);
        glVertex3f(-0.5f  + moving_x, -0.25f, 0.0f);
        glVertex3f(-0.75f + moving_x,  0.25f, 0.0f);
        glEnd();

        window->Update();
    }

    return 0;
}
