#include <window/graphics/helpers.h>

void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides) {
    GLfloat twicePi = 2.0f * M_PI;
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(0.3, 0, 0, 0.1);
    glVertex2f(x, y);
    for ( int i = 0; i <= numberOfSides; i++ ) {
        glColor4f(0.3, cos(i * twicePi / numberOfSides)*2+1, sin(i * twicePi / numberOfSides)*2+1, 0.1);
        glVertex2f(x + (radius * cos(i *  twicePi / numberOfSides)), y + (radius * sin( i * twicePi / numberOfSides)));
    }
    glEnd();
}

bool inside(vec4 borders, vec2 point) {
    if (borders[0] > borders[2]) std::swap(borders[0], borders[2]);
    if (borders[1] > borders[3]) std::swap(borders[1], borders[3]);
    return (borders[0] <= point[0] && point[0] <= borders[2] &&
            borders[1] <= point[1] && point[1] <= borders[3]);
}

void get_vertices(vec4 in, float out[8]) {
    out[0] = in[0]; out[1] = in[1];
    out[2] = in[0]; out[3] = in[3];
    out[4] = in[2]; out[5] = in[3];
    out[6] = in[2]; out[7] = in[1];
}
