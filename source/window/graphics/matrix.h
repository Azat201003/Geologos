#pragma once

#include <glm/glm.hpp>

extern glm::mat4 matrix;

void update_matrix(float w, float h);

void push_matrix();
void pop_matrix();

