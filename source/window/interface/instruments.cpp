#include <glad/glad.h>
#include <window/graphics/helpers.h>
#include <window/interface/instruments.h>

void PointCreatorInstrument::first_click_action_release(GLFWwindow*, vec2 pos, bool inside) {
	if (!inside) return;
	context->shapes_builder->points_manager->clear_selected_points();
	context->shapes_builder->points_manager->add_selected_points(glm::vec2(pos[0], pos[1]));
}

void PointCreatorInstrument::second_click_action_release(GLFWwindow*, vec2 pos, bool inside) {
	if (!inside) return;
	context->shapes_builder->points_manager->add_point(glm::vec2(pos[0], pos[1]));
}

void PointCreatorInstrument::third_click_action_release(GLFWwindow*, vec2 pos, bool inside) {
	if (!inside) return;
	context->shapes_builder->points_manager->add_selected_points(glm::vec2(pos[0], pos[1]));
}



void PointSelectorInstrument::first_click_action_press(GLFWwindow*, vec2 begin_pos, bool inside) {
	if (!inside) return;
	vec2_dup(this->begin_pos, begin_pos);
	vec2_dup(this->current_pos, begin_pos);
	selection = true;
	context->shapes_builder->points_manager->clear_selected_points();
}

void PointSelectorInstrument::first_click_action_release(GLFWwindow*, vec2 end_pos, bool) {
	if (!selection) return;
	selection = false;
	context->shapes_builder->points_manager->add_selected_points(glm::vec2(begin_pos[0], begin_pos[1]), glm::vec2(end_pos[0], end_pos[1]));
}

void PointSelectorInstrument::third_click_action_press(GLFWwindow*, vec2 begin_pos, bool inside) {
	if (!inside) return;
	this->begin_pos[0] = begin_pos[0];
	this->begin_pos[1] = begin_pos[1];
	this->current_pos[0] = begin_pos[0];
	this->current_pos[1] = begin_pos[1];
	selection = true;
}

void PointSelectorInstrument::third_click_action_release(GLFWwindow*, vec2 end_pos, bool) {
	selection = false;
	context->shapes_builder->points_manager->add_selected_points(glm::vec2(begin_pos[0], begin_pos[1]), glm::vec2(end_pos[0], end_pos[1]));
}

void PointSelectorInstrument::action_move(GLFWwindow*, vec2 current_pos, bool) {
	this->current_pos[0] = current_pos[0];
	this->current_pos[1] = current_pos[1];
}

void PointSelectorInstrument::draw() {
	if (!selection) return;
	
	float vertices[8];
	get_vertices(vec4{begin_pos[0], begin_pos[1], current_pos[0], current_pos[1]}, vertices);

	glEnable(GL_LINE_STIPPLE);

	glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, vertices);
		glColor3fv(SELECTION_COLOR);
		glLineWidth(SELECTION_LINE_WIDTH);
		glLineStipple(1, 0x00FF);
		glDrawArrays(GL_LINE_LOOP, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);

	glDisable(GL_LINE_STIPPLE);
}
