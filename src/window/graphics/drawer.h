#pragma once

#include <vector>

class Drawable {
public:
    virtual void draw() = 0;
};

class Drawer {
private:
    std::vector<Drawable*> objects;
public:
    void draw();
    void add_object(Drawable*);
};
