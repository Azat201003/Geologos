#pragma once

#include <GLFW/glfw3.h>

#include <filesystem>

class Callbackable {
private:
public:
  Callbackable();
  virtual void key_callback(GLFWwindow *window, int key, int scancode,
							int action, int mods) {}
  virtual void mouse_button_callback(GLFWwindow *window, int button, int action,
									 int mods) {}
  virtual void cursor_position_callback(GLFWwindow *window, double xpos,
										double ypos) {}
  virtual void resize_callback(GLFWwindow *window, int width, int height) {}
};
