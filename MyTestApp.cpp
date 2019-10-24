#include "awml.h"

int main()
{
    auto window = awml::Window::Create(L"My Window", 1280, 720);

    while (!window->ShouldClose())
    {
        window->PollEvents();
    }
}
