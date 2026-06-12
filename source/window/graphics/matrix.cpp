#include <window/graphics/matrix.h>

#include <glm/gtc/matrix_transform.hpp>
// #include <iostream>

glm::mat4 matrix;

void update_matrix(float width, float height) {
    matrix = glm::ortho(0.f, width, height, 0.f);
    // std::cout << matrix[0][0] << std::endl;
}
