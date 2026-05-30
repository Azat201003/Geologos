#include <glad/glad.h>

#include <window/interface/interface.h>
#include <window/interface/instruments.h>
#include <window/graphics/helpers.h>
#include <iostream>

InterfaceGlyph::InterfaceGlyph(Context* context, vec2 offset, vec2 scale) : context(context) {
    this->offset[0] = offset[0];
    this->offset[1] = offset[1];
    this->scale[0] = scale[0];
    this->scale[1] = scale[1];
}

void InterfaceGlyph::get_offset(vec2 out) {
    out[0] = offset[0];
    out[1] = offset[1];
}

void InterfaceGlyph::get_scale(vec2 out) {
    out[0] = scale[0];
    out[1] = scale[1];
}

void InterfaceGlyph::add_child(InterfaceGlyph* child) {
    children.push_back(child);
}

void InterfaceGlyph::update_pos(vec4 new_pos) {
    pos[0] = new_pos[0];
    pos[1] = new_pos[1];
    pos[2] = new_pos[2];
    pos[3] = new_pos[3];
    for (InterfaceGlyph* child : children) {
        vec2 child_offset;
        vec2 child_scale;
        child->get_offset(child_offset);
        child->get_scale(child_scale);
        child->update_pos(vec4{
            child_offset[0]*(pos[2]-pos[0]) + pos[0],
            child_offset[1]*(pos[3]-pos[1]) + pos[1],
            (child_offset[0] + child_scale[0])*(pos[2]-pos[0]) + pos[0],
            (child_offset[1] + child_scale[1])*(pos[3]-pos[1]) + pos[1],
        });
    }
}

void ButtonGlyph::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (hover && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        on_click();
}

void ButtonGlyph::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (inside(pos, vec2{float(xpos), float(ypos)})) {
        glfwSetCursor(window, glfwCreateStandardCursor(GLFW_HAND_CURSOR));
        if (hover) return;
        on_hover();
        std::cout << 1 << std::endl;
        hover = true;
        FILL_COLOR[0] = .5;
    } else {
        if (!hover) return;
        off_hover();
        glfwSetCursor(window, glfwCreateStandardCursor(GLFW_ARROW_CURSOR));
        hover = false;
        FILL_COLOR[0] = .2;
    }
}

void RadioButtonGlyph::on_click() {
    on_click_dummy();
    selected = true;
}

void RadioButtonGlyph::unselect() {
    selected = false;
}

void FieldGlyph::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    
}

void FieldGlyph::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    vec2 pos{x, y};
    if (!inside(this->pos, pos)) return;
    if (button == GLFW_MOUSE_BUTTON_LEFT && mods == 0) {
        if (action == GLFW_PRESS)
            context->current_instrument->first_click_action_press(window, pos);
        else if (action == GLFW_RELEASE)
            context->current_instrument->first_click_action_release(window, pos);
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS)
            context->current_instrument->second_click_action_press(window, pos);
        else if (action == GLFW_RELEASE)
            context->current_instrument->second_click_action_release(window, pos);
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && mods == GLFW_MOD_CONTROL) {
        if (action == GLFW_PRESS)
            context->current_instrument->third_click_action_press(window, pos);
        else if (action == GLFW_RELEASE)
            context->current_instrument->third_click_action_release(window, pos);
    }
}

void FieldGlyph::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    context->current_instrument->action_move(window, vec2{float(xpos), float(ypos)});
}

void FilledGlyph::draw() {
    glEnableClientState(GL_VERTEX_ARRAY);
        float vertices[8];
        get_vertices(pos, vertices);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glColor3fv(FILL_COLOR);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
}
