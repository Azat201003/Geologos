#include <glad/glad.h>
#include <iostream>

#include <shapes/points.h>

using namespace std;

void Point::get_pos(vec2 out) {
    out[0] = pos[0];
    out[1] = pos[1];
    return;
}

void PointsManager::draw() {
    if (!points.empty()) {
        glVertexPointer(2, GL_FLOAT, 0, &poses[0]);
        glEnableClientState(GL_VERTEX_ARRAY);
            glColor3fv(POINT_COLOR);
            glPointSize(POINTS_SIZE);
            glDrawArrays(GL_POINTS, 0, poses.size()/2);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
}

Point::Point(float* pos) {
    this->pos = pos;
}

PointsManager::PointsManager() {
    
}

void PointsManager::add_point(vec2 pos) {
    poses.push_back(pos[0]);
    poses.push_back(pos[1]);
    points.push_back(new Point(&poses[poses.size()-2]));
}
