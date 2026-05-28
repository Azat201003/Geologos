#pragma once

#include <window/graphics/drawer.h>
#include <shapes/points.h>

class ShapesBuilder {
private:
public:
    PointsManager* points_manager;
    void build(Drawer*);
};
