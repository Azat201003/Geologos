#include <glm/gtc/matrix_transform.hpp>
#include <window/graphics/matrix.h>

glm::mat4 matrix;

void update_matrix(float width, float height) {
    matrix = glm::ortho(0.f, width, height, 0.f);
}
