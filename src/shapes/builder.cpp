#include <shapes/points.h>

#include <shapes/builder.h>

void ShapesBuilder::build(Drawer* drawer) {
    points_manager = new PointsManager();
    drawer->add_object(points_manager);
}
