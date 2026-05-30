#pragma once

#include <vector>

class Drawer;

class Drawable {
public:
    Drawable();
    ~Drawable();
    virtual void draw() = 0;
};

class Drawer {
private:
    std::vector<Drawable*> objects;
    Drawer() {}
    ~Drawer() {}
public:
    Drawer(const Drawer&) = delete;
    Drawer& operator=(const Drawer&) = delete;
    static Drawer& get_drawer() {
        static Drawer instance;
        return instance;
    }
    void draw();
    void add_object(Drawable*);
    void remove_object(Drawable*);
};
