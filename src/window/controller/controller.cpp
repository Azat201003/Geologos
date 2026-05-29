#include <window/controller/controller.h>

ShapesBuilder* EventHandler::shapes_builder = nullptr;

void EventHandler::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_Q && action == GLFW_RELEASE && mods & GLFW_MOD_CONTROL)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void EventHandler::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        shapes_builder->points_manager->add_point(vec2{float(x), float(y)});
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && mods == GLFW_MOD_CONTROL) {
        shapes_builder->points_manager->add_selected_points(vec2{float(x), float(y)});
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && mods == 0) {
        shapes_builder->points_manager->clear_selected_points();
        shapes_builder->points_manager->add_selected_points(vec2{float(x), float(y)});
    }
}
