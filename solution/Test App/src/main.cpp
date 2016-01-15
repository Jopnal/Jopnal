#include <Jopnal/Window/Window.hpp>
#include <cstdlib>

int main()
{
    using jop::Window;

    Window win;
    Window::Settings s;
    s.displayMode = Window::DisplayMode::Windowed;
    s.visible = true;
    s.title = "Jop Window";
    s.size.x = 1024;
    s.size.y = 600;

    win.open(s);
    

    return EXIT_SUCCESS;
}