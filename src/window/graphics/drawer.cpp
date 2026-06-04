#include <window/graphics/drawer.h>

#include <algorithm>
#include <iostream>
#include <glad/glad.h>

void Drawer::draw() {
    for (Drawable* object : objects) {
        if (object != nullptr) {
            object->pre_draw();
            object->draw();
            object->post_draw();
        }
    }
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

void ZIndexedDrawable::pre_draw() {
    glPushMatrix();
    glTranslatef(0, 0, (-10+this->get_z_index())/10.f);
}

void ZIndexedDrawable::post_draw() {
    glPopMatrix();
}
