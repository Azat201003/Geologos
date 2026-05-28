#pragma once

#include <window/graphics/drawer.h>
#include <vector>
#include <linmath.h>

class Point {
private:
    float* pos;
public:
    Point(float*);
    void get_pos(vec2);
};

class PointsManager : public Drawable {
private:
    std::vector<Point*> points;
    std::vector<float> poses;
    const float POINTS_SIZE = 10;
    const vec3 POINT_COLOR = {1, 1, 1};
public:
    PointsManager();
    void draw() override;
    void add_point(vec2);
};
