#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <shapes/builder.h>
#include <window/graphics/drawer.h>
#include <window/interface/interface.h>
#include <window/controller/callbackable.h>

class EventHandler {
// ! To make non-static
private:
    static std::vector<Callbackable*> listeners;
    EventHandler();
    ~EventHandler();
public:
    EventHandler(const EventHandler&) = delete;
    EventHandler& operator=(const EventHandler&) = delete;
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void add_listener(Callbackable* listener);
    static void remove_listener(Callbackable* listener);
};
