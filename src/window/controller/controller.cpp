#include <window/controller/controller.h>
#include <glad/glad.h>

ShapesBuilder* EventHandler::shapes_builder = nullptr;
Instrument* EventHandler::instrument = nullptr;

EventHandler::EventHandler(ShapesBuilder* shapes_builder) {
    EventHandler::shapes_builder = shapes_builder;
    instrument = new PointCreatorInstrument(shapes_builder);
}

void EventHandler::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_Q && action == GLFW_RELEASE && mods & GLFW_MOD_CONTROL)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_S && action == GLFW_RELEASE)
        instrument = new PointSelectorInstrument(shapes_builder);
    if (key == GLFW_KEY_C && action == GLFW_RELEASE)
        instrument = new PointCreatorInstrument(shapes_builder);
}

void EventHandler::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    vec2 pos{x, y};
    if (button == GLFW_MOUSE_BUTTON_LEFT && mods == 0) {
        if (action == GLFW_PRESS)
            instrument->first_click_action_press(window, pos);
        else if (action == GLFW_RELEASE)
            instrument->first_click_action_release(window, pos);
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS)
            instrument->second_click_action_press(window, pos);
        else if (action == GLFW_RELEASE)
            instrument->second_click_action_release(window, pos);
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && mods == GLFW_MOD_CONTROL) {
        if (action == GLFW_PRESS)
            instrument->third_click_action_press(window, pos);
        else if (action == GLFW_RELEASE)
            instrument->third_click_action_release(window, pos);
    }
}

void EventHandler::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    instrument->action_move(window, vec2{float(xpos), float(ypos)});
}

void EventHandler::set_instrument(Instrument* instrument) {
    delete EventHandler::instrument;
    EventHandler::instrument = instrument;
}



void PointCreatorInstrument::first_click_action_release(GLFWwindow*, vec2 pos) {
    shapes_builder->points_manager->clear_selected_points();
    shapes_builder->points_manager->add_selected_points(pos);
}

void PointCreatorInstrument::second_click_action_release(GLFWwindow*, vec2 pos) {
    shapes_builder->points_manager->add_point(pos);
}

void PointCreatorInstrument::third_click_action_release(GLFWwindow*, vec2 pos) {
    shapes_builder->points_manager->add_selected_points(pos);
}



void PointSelectorInstrument::first_click_action_press(GLFWwindow*, vec2 begin_pos) {
    this->begin_pos[0] = begin_pos[0];
    this->begin_pos[1] = begin_pos[1];
    this->current_pos[0] = begin_pos[0];
    this->current_pos[1] = begin_pos[1];
    selection = true;
    shapes_builder->points_manager->clear_selected_points();
}

void PointSelectorInstrument::first_click_action_release(GLFWwindow*, vec2 end_pos) {
    selection = false;
    shapes_builder->points_manager->add_selected_points(begin_pos, end_pos);
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
    shapes_builder->points_manager->add_selected_points(begin_pos, end_pos);
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
