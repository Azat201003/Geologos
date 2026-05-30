#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <shapes/builder.h>
#include <window/interface/interface.h>

class Instrument {
protected:
    Context* context;
public:
    Instrument(Context* context) : context(context) {}
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
    const vec3 SELECTION_COLOR{.7, .7, .7};
    const float SELECTION_LINE_WIDTH = 2;
public:
    PointSelectorInstrument(Context* context) : Instrument(context), Drawable() {}
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
    PointCreatorInstrument(Context* context) : Instrument(context) {}
    void first_click_action_release(GLFWwindow*, vec2) override;
    void second_click_action_release(GLFWwindow*, vec2) override;
    void third_click_action_release(GLFWwindow*, vec2) override;
};
