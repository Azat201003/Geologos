#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <shapes/builder.h>
#include <window/window.h>

#include <math.h>

int main() {
  Window *window = new Window();
  return window->run();
}
