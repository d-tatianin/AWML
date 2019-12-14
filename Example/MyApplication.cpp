//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
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

    window->OnKeyPressed(
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
                window->Resize(window->GetWidth() + 100, window->GetHeight() + 100);
            else if (key_code == awml_key::E)
                window->Resize(window->GetWidth() - 100, window->GetHeight() - 100);

            std::cout
                << key_code
                << " pressed. Repeated ("
                << repeated
                << ") " << "Repeat count ("
                << repeat_count
                << ")"
                << std::endl;
        }
    );

    window->OnKeyReleased(
        [](awml_key key_code)
        {
            std::cout
                << key_code
                << " released."
                << std::endl;
        }
    );

    window->OnWindowResized(
        [&window](uint16_t width, uint16_t height)
        {
            
            glViewport(0, 0, window->GetWidth(), window->GetHeight());

            std::cout <<
                "Resized the window, New size: "
                << width
                << "x"
                << height
                << std::endl;
        }
    );

    window->OnWindowClosed(
        []()
        {
            std::cout << "Window terminated!"
                << std::endl;
        }
    );

    window->OnMouseMoved(
        [](uint16_t xpos, uint16_t ypos)
        {
            std::cout << "Mouse moved, x:"
                << xpos
                << " y:"
                << ypos
                << std::endl;
        }
    );

    window->OnMousePressed(
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

    window->OnMouseReleased(
        [](awml_key code)
        {
            std::cout
                << code
                << " released."
                << std::endl;
        }
    );

    window->OnMouseScrolled(
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

    window->OnCharTyped(
        [](wchar_t typed_char)
        {
            // PS: Don't use cout with wchar_t.
            std::cout << "Typed a char: "
                << static_cast<char>(typed_char)
                << std::endl;
        }
    );

    window->OnError(
        [](awml::error code, const std::string& msg)
        {
            std::cout << "AWML ERROR: "
                << static_cast<uint16_t>(code)
                << " " << msg
                << std::endl;
        }
    );

    window->Launch();
 
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    while (!window->ShouldClose())
    {
        static float moving_x = -1.0f;

        if (moving_x - 1.0f > 1.0f)
            moving_x = -1.0f;

        moving_x += 0.01f;
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);

        window->Update();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return 0;
}
