#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <shapes/points.h>
#include <window/graphics/helpers.h>
#include <window/graphics/matrix.h>

#include "window/graphics/shaders.h"

#define MAX_DIST 4294967295u

using namespace std;

void Point::get_pos(vec2 out) {
	vec2_dup(out, pos);
}

void PointsManager::draw() {
	auto shader = ShaderStorage::get_shader(ShaderKit::DEFAULT); 
	shader->set_mat4("matrix", matrix);
	if (points.empty()) return;
	glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, poses.data());
		if (!selected_ids.empty()) {
			glPointSize(SELECTED_POINTS_SIZE);
			shader->set_vec4("inColor", glm::vec4(SELECTED_POINT_COLOR[0], SELECTED_POINT_COLOR[1], SELECTED_POINT_COLOR[2], 1));
			glDrawElements(GL_POINTS, selected_ids.size(), GL_UNSIGNED_INT, selected_ids.data());
		}
		push_matrix();
		matrix = glm::translate(matrix, glm::vec3(0, 0, .01));
		shader->set_mat4("matrix", matrix);
		glPointSize(POINTS_SIZE);
		shader->set_vec4("inColor", glm::vec4(POINT_COLOR[0], POINT_COLOR[1], POINT_COLOR[2], 1));
		glDrawArrays(GL_POINTS, 0, poses.size()/2);
		pop_matrix();
	glDisableClientState(GL_VERTEX_ARRAY);

}

Point::Point(float* pos) {
	this->pos = pos;
}

PointsManager::PointsManager() {
	this->set_z_index(9);
}

void PointsManager::add_point(vec2 cursor) {
	poses.push_back(cursor[0]);
	poses.push_back(cursor[1]);
	points.push_back(new Point(&poses[poses.size()-2]));
}

bool PointsManager::add_selected_points(vec2 cursor) {
	unsigned int nearest = -1;
	unsigned int min_dist = MAX_DIST;
	for (int i = 0; i < points.size(); ++i) {
		vec2 sub;
		vec2_sub(sub, vec2{poses[i*2], poses[i*2+1]}, cursor);
		float current_dist = vec2_len(sub);
		if (current_dist < min_dist) {
			nearest = i;
			min_dist = current_dist;
		}
	}
	if (nearest != -1 && min_dist < SELECTION_RADIUS) {
		selected_ids.push_back(nearest);
		return true;
	}
	return false;
}

unsigned int PointsManager::add_selected_points(vec2 begin_pos, vec2 end_pos) {
	vec4 borders{begin_pos[0], begin_pos[1], end_pos[0], end_pos[1]};
	int cnt = 0;

	for (int i = 0; i < points.size(); ++i) {
		vec2 pos{poses[i*2], poses[i*2+1]};
		if (is_inside(borders, pos))
			selected_ids.push_back(i);
	}
	return cnt;
}

void PointsManager::clear_selected_points() {
	selected_ids = {};
}
