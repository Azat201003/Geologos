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
        glPointSize(POINTS_SIZE);
        glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(2, GL_FLOAT, 0, poses.data());
            glColor3fv(POINT_COLOR);
            glDrawArrays(GL_POINTS, 0, poses.size()/2);
        
        if (!selected_ids.empty()) {
            glPointSize(SELECTED_POINTS_SIZE);
            glColor3fv(SELECTED_POINT_COLOR);
            glDrawElements(GL_POINTS, selected_ids.size(), GL_UNSIGNED_INT, selected_ids.data());
        }
        glDisableClientState(GL_VERTEX_ARRAY);
    }
}

Point::Point(float* pos) {
    this->pos = pos;
}

PointsManager::PointsManager() {
    
}

void PointsManager::add_point(vec2 cursor) {
    poses.push_back(cursor[0]);
    poses.push_back(cursor[1]);
    points.push_back(new Point(&poses[poses.size()-2]));
}

bool PointsManager::add_selected_points(vec2 cursor) {
    for (int i = 0; i < points.size(); ++i) {
        vec2 sub;
        vec2_sub(sub, vec2{poses[i*2], poses[i*2+1]}, cursor);
        if (vec2_len(sub) < SELECTION_RADIUS) {
            selected_ids.push_back(i);
            return true;
        }
        
    }
    return false;
}

void PointsManager::clear_selected_points() {
    selected_ids = {};
}
