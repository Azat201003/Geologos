#include <shapes/points.h>

#include <glad/glad.h>
#include <iostream>
#include <window/graphics/helpers.h>

#define MAX_DIST 4294967295u

using namespace std;

void Point::get_pos(vec2 out) {
    out[0] = pos[0];
    out[1] = pos[1];
    return;
}

void PointsManager::draw() {
    if (points.empty()) return;

    glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, poses.data());
        if (!selected_ids.empty()) {
            glPointSize(SELECTED_POINTS_SIZE);
            glColor3fv(SELECTED_POINT_COLOR);
            glDrawElements(GL_POINTS, selected_ids.size(), GL_UNSIGNED_INT, selected_ids.data());
        }
        glPointSize(POINTS_SIZE);
        glColor3fv(POINT_COLOR);
        glDrawArrays(GL_POINTS, 0, poses.size()/2);
    glDisableClientState(GL_VERTEX_ARRAY);
}

Point::Point(float* pos) {
    this->pos = pos;
}

void PointsManager::add_point(vec2 cursor) {
    poses.push_back(cursor[0]);
    poses.push_back(cursor[1]);
    points.push_back(new Point(&poses[poses.size()-2]));
}

bool PointsManager::add_selected_points(vec2 cursor) {
    unsigned int nearest = -1;
    unsigned int min_dist = MAX_DIST;
    for (int i = 0; i < points.size(); ++i) {
        vec2 sub;
        vec2_sub(sub, vec2{poses[i*2], poses[i*2+1]}, cursor);
        float current_dist = vec2_len(sub);
        if (current_dist < min_dist) {
            nearest = i;
            min_dist = current_dist;
        }
    }
    if (nearest != -1 && min_dist < SELECTION_RADIUS) {
        selected_ids.push_back(nearest);
        return true;
    }
    return false;
}

unsigned int PointsManager::add_selected_points(vec2 begin_pos, vec2 end_pos) {
    if (begin_pos[0] > end_pos[0]) swap(begin_pos[0], end_pos[0]);
    if (begin_pos[1] > end_pos[1]) swap(begin_pos[1], end_pos[1]);

    vec4 borders{begin_pos[0], begin_pos[1], end_pos[0], end_pos[1]};

    int cnt = 0;

    for (int i = 0; i < points.size(); ++i) {
        vec2 pos{poses[i*2], poses[i*2+1]};
        if (inside(borders, pos))
            selected_ids.push_back(i);
    }
    return cnt;
}

void PointsManager::clear_selected_points() {
    selected_ids = {};
}
