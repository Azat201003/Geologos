#include <window/graphics/drawer.h>

void Drawer::draw() {
    for (Drawable* object : objects) {
        object->draw();
    }
}

void Drawer::add_object(Drawable* object) {
    objects.push_back(object);
}


