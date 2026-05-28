#include <window/controller/controller.h>

ShapesBuilder* EventHandler::shapes_builder = nullptr;

void EventHandler::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_Q && action == GLFW_RELEASE && mods & GLFW_MOD_CONTROL)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void EventHandler::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        shapes_builder->points_manager->add_point(vec2{float(x), float(y)});
    }
}
