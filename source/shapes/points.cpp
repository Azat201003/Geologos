#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shapes/points.h>
#include <window/graphics/helpers.h>
#include <window/graphics/matrix.h>

#include "window/graphics/shaders.h"

#define MAX_DIST 4294967295u

using namespace std;

glm::vec2 Point::get_pos() {
	return pos;
}

void PointsManager::draw() {
	auto shader = ShaderStorage::get_shader(ShaderKit::DEFAULT); 
	shader->use();
	shader->set_mat4("matrix", matrix);
	if (points.empty()) return;
	if (selected_count) {
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glPointSize(SELECTED_POINTS_SIZE);
		shader->set_vec4("inColor", glm::vec4(SELECTED_POINT_COLOR[0], SELECTED_POINT_COLOR[1], SELECTED_POINT_COLOR[2], 1));
		glDrawElements(GL_POINTS, selected_count, GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glPointSize(POINTS_SIZE);
	push_matrix();
	matrix = glm::translate(matrix, glm::vec3(0, 0, .01));
	shader->set_mat4("matrix", matrix);
	pop_matrix();
	
	shader->set_vec4("inColor", glm::vec4(POINT_COLOR[0], POINT_COLOR[1], POINT_COLOR[2], 1));
	glDrawArrays(GL_POINTS, 0, points.size());
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Point::Point(glm::vec2 pos) {
	this->pos = pos;
}

PointsManager::PointsManager() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBufferData(GL_ARRAY_BUFFER, 1024, NULL, GL_DYNAMIC_DRAW); 
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 1024, NULL, GL_DYNAMIC_DRAW);  // not sure


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	this->set_z_index(9);
}

void PointsManager::add_point(glm::vec2 cursor) {
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(VAO);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float)*points.size()*2, sizeof(float)*2, glm::value_ptr(cursor));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	points.push_back(new Point(cursor));
}

bool PointsManager::add_selected_points(glm::vec2 cursor) {
	unsigned int nearest = -1;
	unsigned int min_dist = MAX_DIST;
	for (int i = 0; i < points.size(); ++i) {
		float current_dist = glm::distance(points[i]->get_pos(), cursor);
		if (current_dist < min_dist) {
			nearest = i;
			min_dist = current_dist;
		}
	}
	if (nearest != -1 && min_dist < SELECTION_RADIUS) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*selected_count, sizeof(unsigned), &nearest);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		
		selected_count++;
		return true;
	}
	return false;
}

unsigned int PointsManager::add_selected_points(glm::vec2 begin_pos, glm::vec2 end_pos) {
	int cnt = 0;

	for (int i = 0; i < points.size(); ++i) {
		if (is_inside(glm::vec4(begin_pos, end_pos), points[i]->get_pos())) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*selected_count, sizeof(unsigned), &i);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			
			selected_count++;
		}
	}
	return cnt;
}

void PointsManager::clear_selected_points() {
	selected_count = 0;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glClearBufferData(GL_ELEMENT_ARRAY_BUFFER, GL_RG32F, GL_RG32F, GL_RG32F, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
