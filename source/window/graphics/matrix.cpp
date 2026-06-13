#include <glm/gtc/matrix_transform.hpp>
#include <window/graphics/matrix.h>

#include <stack>

glm::mat4 matrix;

std::stack<glm::mat4> stack;

void update_matrix(float width, float height) {
	matrix = glm::ortho(0.f, width, height, 0.f);
}

void push_matrix() {
	glm::mat4 matrix_copy;
	matrix_copy = matrix;
	stack.push(matrix_copy); // ! May be move
}

void pop_matrix() {
	matrix = stack.top();
	stack.pop();
}

