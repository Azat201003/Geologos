#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <shapes/builder.h>

class EventHandler {
private:
    static ShapesBuilder* shapes_builder;
public:
    EventHandler(ShapesBuilder* shapes_builder) { EventHandler::shapes_builder = shapes_builder; }
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};
