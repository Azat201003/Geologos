#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <window/window.h>
#include <shapes/builder.h>

int main() {
    Window* window = new Window();
    return window->run();
}
