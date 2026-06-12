#pragma once

#include <window/graphics/drawer.h>
#include <window/graphics/shaders.h>
#include <vector>
#include <linmath.h>

class Point {
private:
    float* pos;
public:
    Point(float*);
    void get_pos(vec2);
};

class PointsManager : public ZIndexedDrawable {
private:
    Shader* shader;
    std::vector<Point*> points;
    std::vector<float> poses;
    std::vector<unsigned int> selected_ids;
    const float POINTS_SIZE = 6;
    const float SELECTED_POINTS_SIZE = 10;
    const vec3 POINT_COLOR = {.9, .9, .95};
    const vec3 SELECTED_POINT_COLOR = {.9, .7, .1};
    const float SELECTION_RADIUS = 15;
public:
    PointsManager();
    void draw() override;
    void add_point(vec2);
    bool add_selected_points(vec2);
    unsigned int add_selected_points(vec2, vec2);
    void clear_selected_points();
};
