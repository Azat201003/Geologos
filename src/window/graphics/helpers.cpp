#include <window/graphics/helpers.h>

void drawCircle( GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides )
{
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
