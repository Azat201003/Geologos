#pragma once

#include <vector>
#include <functional>
#include <linmath.h>
#include <window/controller/callbackable.h>
#include <window/graphics/drawer.h>
#include <shapes/builder.h>
#include <context.h>

class InterfaceGlyph {
protected:
    std::vector<InterfaceGlyph*> children;
    Context* context;
    vec4 pos; // x1, y1, x2, y2
    vec2 offset;
    vec2 scale;
public:
    InterfaceGlyph(Context* context, vec2 offset, vec2 scale);
    void get_scale(vec2 out);
    void get_offset(vec2 out); // relative
    void add_child(InterfaceGlyph* child);
    virtual void individual_draw() {}
    virtual void update_pos(vec4 new_pos); // absolute
};

class FilledGlyph : public InterfaceGlyph, public Drawable {
protected:
    vec3 FILL_COLOR{.1, .2, .3};
public:
    FilledGlyph(Context* context, vec2 offset, vec2 scale) : InterfaceGlyph(context, offset, scale), Drawable() {}
    FilledGlyph(Context* context, vec2 offset, vec2 scale, vec3 fill_color) : InterfaceGlyph(context, offset, scale), Drawable() {
        FILL_COLOR[0] = fill_color[0]; FILL_COLOR[1] = fill_color[1]; FILL_COLOR[2] = fill_color[2];
    }
    virtual void draw() override;
};


class ButtonGlyph : public Callbackable, public FilledGlyph {
protected:
    std::function<void(void)> on_click_dummy;
    std::function<void(void)> on_hover_dummy;
    std::function<void(void)> off_hover_dummy;
    bool hover = false;
    bool click_started = false;
public:
    ButtonGlyph(Context* context, vec2 offset, vec2 scale, vec3 fill_color, std::function<void(void)> onclick,
                                                                            std::function<void(void)> onhover,
                                                                            std::function<void(void)> offhover)
                                                                                : FilledGlyph(context, offset, scale, fill_color), 
                                                                                Callbackable(),
                                                                                on_click_dummy(onclick),
                                                                                on_hover_dummy(onhover),
                                                                                off_hover_dummy(offhover) {}
    virtual void on_click() {on_click_dummy();}
    virtual void on_hover() {on_hover_dummy();}
    virtual void off_hover() {off_hover_dummy();}
    virtual void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) override;
    virtual void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) override;
};

class RadioButtonGlyph : public ButtonGlyph {
protected:
    bool selected = false;
public:
    RadioButtonGlyph(Context* context, vec2 offset, vec2 scale, vec3 fill_color, std::function<void(void)> onclick,
                                                                                 std::function<void(void)> onhover,
                                                                                 std::function<void(void)> offhover)
                                                                                 : ButtonGlyph(context, offset, scale, fill_color, onclick, onhover, offhover) {}
    void on_click() override;
    virtual void on_select() {}
    virtual void on_unselect() {}
    void unselect();
};

class FieldGlyph : public InterfaceGlyph, public Callbackable {  
public:
    FieldGlyph(Context* context, vec2 offset, vec2 scale) : InterfaceGlyph(context, offset, scale), Callbackable() {}
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) override;
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) override;
    void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) override;
};
