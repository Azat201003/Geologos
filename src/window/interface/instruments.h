#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <shapes/builder.h>
#include <window/interface/interface.h>

class Instrument {
protected:
    Context* context;
public:
    Instrument(Context* context) : context(context) {}
    virtual ~Instrument() {}
    virtual void first_click_action_press(GLFWwindow* window, vec2 pos, bool inside) {} // Left click
    virtual void first_click_action_release(GLFWwindow* window, vec2 pos, bool inside) {}
    virtual void second_click_action_press(GLFWwindow* window, vec2 pos, bool inside) {} // Right click
    virtual void second_click_action_release(GLFWwindow* window, vec2 pos, bool inside) {}
    virtual void third_click_action_press(GLFWwindow* window, vec2 pos, bool inside) {} // CTRL + Left click
    virtual void third_click_action_release(GLFWwindow* window, vec2 pos, bool inside) {}
    virtual void action_move(GLFWwindow* window, vec2 pos, bool inside) {}
};

class PointSelectorInstrument : public Drawable, public Instrument {
private:
    vec2 begin_pos{0,0};
    vec2 current_pos{0,0};
    bool selection = false;
    const vec3 SELECTION_COLOR{.7, .7, .7};
    const float SELECTION_LINE_WIDTH = 2;
public:
    PointSelectorInstrument(Context* context) : Instrument(context), Drawable() {}
    void first_click_action_press(GLFWwindow*, vec2, bool) override;
    void first_click_action_release(GLFWwindow*, vec2, bool) override;
    void third_click_action_press(GLFWwindow*, vec2, bool) override;
    void third_click_action_release(GLFWwindow*, vec2, bool) override;
    void action_move(GLFWwindow*, vec2, bool) override;
    void draw() override;
};

class PointCreatorInstrument : public Instrument {
private:
public:
    PointCreatorInstrument(Context* context) : Instrument(context) {}
    void first_click_action_release(GLFWwindow*, vec2, bool) override;
    void second_click_action_release(GLFWwindow*, vec2, bool) override;
    void third_click_action_release(GLFWwindow*, vec2, bool) override;
};
