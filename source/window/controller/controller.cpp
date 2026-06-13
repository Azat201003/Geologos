#include <glad/glad.h>
#include <window/controller/controller.h>
#include <window/graphics/matrix.h>

#include <algorithm>

std::vector<Callbackable*> EventHandler::listeners{};

void EventHandler::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_Q && action == GLFW_RELEASE && mods & GLFW_MOD_CONTROL)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	for (Callbackable* listener : listeners)
		listener->key_callback(window, key, scancode, action, mods);
}

void EventHandler::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	for (Callbackable* listener : listeners)
		listener->mouse_button_callback(window, button, action, mods);
}

void EventHandler::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	for (Callbackable* listener : listeners)
		listener->cursor_position_callback(window, xpos, ypos);
}

void EventHandler::resize_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	update_matrix(width, height);
	for (Callbackable* listener : listeners)
		listener->resize_callback(window, width, height);
}

void EventHandler::add_listener(Callbackable* listener) {
	listeners.push_back(listener);
}

void EventHandler::remove_listener(Callbackable* listener) {
	listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
}
