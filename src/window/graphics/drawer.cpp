#include <window/graphics/drawer.h>

#include <algorithm>

void Drawer::draw() {
    for (Drawable* object : objects)
        if (object != nullptr)
            object->draw();
}

void Drawer::add_object(Drawable* object) {
    objects.push_back(object);
}

void Drawer::remove_object(Drawable* object) {
    objects.erase(std::remove(objects.begin(), objects.end(), object), objects.end());
}

Drawable::Drawable() {
    Drawer::get_drawer().add_object(this);
}

Drawable::~Drawable() {
    Drawer::get_drawer().remove_object(this);
}
