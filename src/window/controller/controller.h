#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <shapes/builder.h>
#include <window/graphics/drawer.h>

class Instrument;

class EventHandler {
private:
    static ShapesBuilder* shapes_builder;
    static Instrument* instrument;
public:
    EventHandler(ShapesBuilder* shapes_builder);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void set_instrument(Instrument* instrument);
};

class Instrument {
protected:
    ShapesBuilder* shapes_builder;
public:
    Instrument(ShapesBuilder* shapes_builder) : shapes_builder(shapes_builder) {}
    virtual void first_click_action_press(GLFWwindow*, vec2) {} // Left click
    virtual void first_click_action_release(GLFWwindow*, vec2) {}
    virtual void second_click_action_press(GLFWwindow*, vec2) {} // Right click
    virtual void second_click_action_release(GLFWwindow*, vec2) {}
    virtual void third_click_action_press(GLFWwindow*, vec2) {} // CTRL + Left click
    virtual void third_click_action_release(GLFWwindow*, vec2) {}
    virtual void action_move(GLFWwindow*, vec2) {}
};

class PointSelectorInstrument : public Instrument, public Drawable {
private:
    vec2 begin_pos{0,0};
    vec2 current_pos{0,0};
    bool selection = false;
    const vec3 SELECTION_COLOR{.5, .5, .5};
    const float SELECTION_LINE_WIDTH = 2;
public:
    PointSelectorInstrument(ShapesBuilder* shapes_builder) : Instrument(shapes_builder) { Drawer::get_drawer().add_object(this); }
    void first_click_action_press(GLFWwindow*, vec2) override;
    void first_click_action_release(GLFWwindow*, vec2) override;
    void third_click_action_press(GLFWwindow*, vec2) override;
    void third_click_action_release(GLFWwindow*, vec2) override;
    void action_move(GLFWwindow*, vec2) override;
    void draw() override;
};

class PointCreatorInstrument : public Instrument {
private:
public:
    PointCreatorInstrument(ShapesBuilder* shapes_builder) : Instrument(shapes_builder) {}
    void first_click_action_release(GLFWwindow*, vec2) override;
    void second_click_action_release(GLFWwindow*, vec2) override;
    void third_click_action_release(GLFWwindow*, vec2) override;
};
