#include <window/interface/instruments.h>
#include <glad/glad.h>

void PointCreatorInstrument::first_click_action_release(GLFWwindow*, vec2 pos) {
    context->shapes_builder->points_manager->clear_selected_points();
    context->shapes_builder->points_manager->add_selected_points(pos);
}

void PointCreatorInstrument::second_click_action_release(GLFWwindow*, vec2 pos) {
    context->shapes_builder->points_manager->add_point(pos);
}

void PointCreatorInstrument::third_click_action_release(GLFWwindow*, vec2 pos) {
    context->shapes_builder->points_manager->add_selected_points(pos);
}



void PointSelectorInstrument::first_click_action_press(GLFWwindow*, vec2 begin_pos) {
    this->begin_pos[0] = begin_pos[0];
    this->begin_pos[1] = begin_pos[1];
    this->current_pos[0] = begin_pos[0];
    this->current_pos[1] = begin_pos[1];
    selection = true;
    context->shapes_builder->points_manager->clear_selected_points();
}

void PointSelectorInstrument::first_click_action_release(GLFWwindow*, vec2 end_pos) {
    selection = false;
    context->shapes_builder->points_manager->add_selected_points(begin_pos, end_pos);
}

void PointSelectorInstrument::third_click_action_press(GLFWwindow*, vec2 begin_pos) {
    this->begin_pos[0] = begin_pos[0];
    this->begin_pos[1] = begin_pos[1];
    this->current_pos[0] = begin_pos[0];
    this->current_pos[1] = begin_pos[1];
    selection = true;
}

void PointSelectorInstrument::third_click_action_release(GLFWwindow*, vec2 end_pos) {
    selection = false;
    context->shapes_builder->points_manager->add_selected_points(begin_pos, end_pos);
}

void PointSelectorInstrument::action_move(GLFWwindow*, vec2 current_pos) {
    this->current_pos[0] = current_pos[0];
    this->current_pos[1] = current_pos[1];
}

void PointSelectorInstrument::draw() {
    if (!selection) return;
    float vertexes[] {
        begin_pos[0], begin_pos[1],
        current_pos[0], begin_pos[1],
        current_pos[0], current_pos[1],
        begin_pos[0], current_pos[1],
    };

    glEnable(GL_LINE_STIPPLE);

    glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, vertexes);
        glColor3fv(SELECTION_COLOR);
        glLineWidth(SELECTION_LINE_WIDTH);
        glLineStipple(1, 0x00FF);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);

    glDisable(GL_LINE_STIPPLE);
}
