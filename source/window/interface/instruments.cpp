#include <glad/gl.h>
#include <window/graphics/helpers.h>
#include <window/graphics/matrix.h>
#include <window/graphics/shaders.h>
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



PointSelectorInstrument::PointSelectorInstrument(Context* context) : Instrument(context) {
	this->set_z_index(3);
	
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void PointSelectorInstrument::first_click_action_press(GLFWwindow*, vec2 begin_pos, bool inside) {
	if (!inside) return;
	vec2_dup(this->begin_pos, begin_pos);
	vec2_dup(this->current_pos, begin_pos);
	selection = true;
	context->shapes_builder->points_manager->clear_selected_points();
	update_buffer();
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
	update_buffer();
}

void PointSelectorInstrument::third_click_action_release(GLFWwindow*, vec2 end_pos, bool) {
	if (!selection) return;
	selection = false;
	context->shapes_builder->points_manager->add_selected_points(glm::vec2(begin_pos[0], begin_pos[1]), glm::vec2(end_pos[0], end_pos[1]));
}

void PointSelectorInstrument::action_move(GLFWwindow*, vec2 current_pos, bool) {
	this->current_pos[0] = current_pos[0];
	this->current_pos[1] = current_pos[1];
	update_buffer();
}

void PointSelectorInstrument::update_buffer() {
	float vertices[8];
	get_vertices(vec4{begin_pos[0], begin_pos[1], current_pos[0], current_pos[1]}, vertices);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(VAO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void PointSelectorInstrument::draw() {
	if (!selection) return;

	auto shader = ShaderStorage::get_shader(ShaderKit::DEFAULT); 
	shader->use();
	shader->set_mat4("matrix", matrix);
	shader->set_vec4("inColor", glm::vec4(SELECTION_COLOR[0], SELECTION_COLOR[1], SELECTION_COLOR[2], 1.));

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(VAO);
	
	glEnable(GL_LINE_STIPPLE); // It is slow, as I know

	glColor3fv(SELECTION_COLOR);
	glLineWidth(SELECTION_LINE_WIDTH);
	glLineStipple(1, 0x00FF);
	glDrawArrays(GL_LINE_LOOP, 0, 4);

	glDisable(GL_LINE_STIPPLE);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
