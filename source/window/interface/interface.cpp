#include <glad/glad.h>
#include <window/graphics/helpers.h>
#include <window/graphics/matrix.h>
#include <window/interface/instruments.h>
#include <window/interface/interface.h>

#include <iostream>

#include "window/graphics/shaders.h"

InterfaceGlyph::InterfaceGlyph(Context* context, vec2 offset, vec2 scale) : context(context) {
    vec2_dup(this->offset, offset);
    vec2_dup(this->scale, scale);
}

void InterfaceGlyph::get_offset(vec2 out) {
    vec2_dup(out, offset);
}

void InterfaceGlyph::get_scale(vec2 out) {
    vec2_dup(out, scale);
}

void InterfaceGlyph::add_child(InterfaceGlyph* child) {
    children.push_back(child);
}

void InterfaceGlyph::update_pos(vec4 new_pos) {
    vec4_dup(pos, new_pos);
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
    this->on_update_pos();
}

bool Focusable::is_focused() {
    return focused;
}

Focusable::Focusable(FocusManager* fm) {
    focus_manager = fm;
    fm->add_focusable(this);
}

Focusable::~Focusable() {
    focus_manager->remove_focusable(this);
}

void FocusableGlyph::on_update_pos() {
    vec4_dup(this->focus_area, this->pos);
}

void FocusManager::add_focusable(Focusable* focusable) {
    this->focusables.insert(focusable);
}

void FocusManager::remove_focusable(Focusable* focusable) {
    this->focusables.erase(focusable);
}

void FocusManager::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    bool focus_added = false;
    
    for (Focusable* focusable : focusables) {
        if (is_inside(focusable->focus_area, vec2{float(xpos), float(ypos)}) && !focus_added) {
            focus_added = true;
            if (!focusable->focused)
                focusable->on_focus();
            focusable->focused = true;
        } else if (focusable->focused) {
            focusable->off_focus();
            focusable->focused = false;
        }
    }
}

void ButtonGlyph::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (hover && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && click_started) on_click();
    if (button == GLFW_MOUSE_BUTTON_LEFT) click_started = false;
    if (hover && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) click_started = true;
}

void ButtonGlyph::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (is_inside(this->pos, vec2{float(xpos), float(ypos)})) {
        glfwSetCursor(window, glfwCreateStandardCursor(GLFW_HAND_CURSOR));
        if (hover) return;
        on_hover();
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
    vec2 pos{float(x), float(y)};
    bool inside = this->is_focused();
    if (button == GLFW_MOUSE_BUTTON_LEFT && mods == 0) {
        if (action == GLFW_PRESS)
            context->get_current_instrument()->first_click_action_press(window, pos, inside);
        else if (action == GLFW_RELEASE)
            context->get_current_instrument()->first_click_action_release(window, pos, inside);
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS)
            context->get_current_instrument()->second_click_action_press(window, pos, inside);
        else if (action == GLFW_RELEASE)
            context->get_current_instrument()->second_click_action_release(window, pos, inside);
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && mods == GLFW_MOD_CONTROL) {
        if (action == GLFW_PRESS)
            context->get_current_instrument()->third_click_action_press(window, pos, inside);
        else if (action == GLFW_RELEASE)
            context->get_current_instrument()->third_click_action_release(window, pos, inside);
    }
}

void FieldGlyph::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    vec2 pos{float(xpos), float(ypos)};
    bool inside = this->is_focused();
    context->get_current_instrument()->action_move(window, vec2{float(xpos), float(ypos)}, inside);
}

void FilledGlyph::draw() {
    auto shader = ShaderStorage::get_shader(ShaderKit::DEFAULT);
    shader->use();
    shader->set_vec4("inColor", glm::vec4(FILL_COLOR[0], FILL_COLOR[1], FILL_COLOR[2], 1));
    shader->set_mat4("matrix", matrix);
    glEnableClientState(GL_VERTEX_ARRAY);
        float vertices[8];
        get_vertices(pos, vertices);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        // glColor3fv(FILL_COLOR);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void FocusableFilledGlyph::draw() {
    auto shader = ShaderStorage::get_shader(ShaderKit::DEFAULT);
    shader->use();
    shader->set_vec4("inColor", glm::vec4(FILL_COLOR[0], FILL_COLOR[1], FILL_COLOR[2], 1));
    shader->set_mat4("matrix", matrix);
    glEnableClientState(GL_VERTEX_ARRAY);
        float vertices[8];
        get_vertices(pos, vertices);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glColor3fv(FILL_COLOR);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void Focusable::on_z_index_update() {
    focus_manager->remove_focusable(this);
    focus_manager->add_focusable(this);
}
