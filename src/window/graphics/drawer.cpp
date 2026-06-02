#include <window/graphics/drawer.h>

#include <algorithm>
#include <iostream>

void Drawer::draw() {
    for (Drawable* object : objects)
        if (object != nullptr)
            object->draw();
}

void Drawer::add_object(Drawable* object) {
    objects.push_back(object);
}

void Drawer::remove_object(Drawable* object) {
    std::cout << "Before: " << objects.size() << std::endl;
    objects.erase(std::remove(objects.begin(), objects.end(), object), objects.end());
    std::cout << "After: " << objects.size() << std::endl;
}

Drawable::Drawable() {
    Drawer::get_drawer().add_object(this);
}

Drawable::~Drawable() {
    std::cout << 52 << std::endl;
    Drawer::get_drawer().remove_object(this);
}
