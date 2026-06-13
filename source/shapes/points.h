#pragma once

#include <linmath.h>
#include <window/graphics/drawer.h>
#include <window/graphics/shaders.h>

#include <vector>

class Point {
private:
	glm::vec2 pos;
public:
	Point(glm::vec2);
	glm::vec2 get_pos();
};

class PointsManager : public ZIndexedDrawable {
private:
	std::vector<Point*> points;
	unsigned selected_count = 0;
	const float POINTS_SIZE = 6;
	const float SELECTED_POINTS_SIZE = 10;
	const vec3 POINT_COLOR = {.9, .9, .95};
	const vec3 SELECTED_POINT_COLOR = {.9, .7, .1};
	const float SELECTION_RADIUS = 15;
	unsigned VAO;
	unsigned VBO;
	unsigned EBO;
public:
	PointsManager();
	void draw() override;
	void add_point(glm::vec2);
	bool add_selected_points(glm::vec2);
	unsigned int add_selected_points(glm::vec2, glm::vec2);
	void clear_selected_points();
};
