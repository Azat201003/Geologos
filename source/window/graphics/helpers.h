#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <linmath.h>

void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides);

bool is_inside(vec4 borders, vec2 point); // x1, y1, x2, y2, x3, y3

void get_vertices(vec4 in, float out[8]);
