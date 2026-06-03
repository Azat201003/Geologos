#pragma once

class ZIndexed {
private:
    int z_index;
public:
    int get_z_index();
    void set_z_index(int);
    virtual void on_z_index_update() {}
};

class ZIndexComporator {
public:
    bool operator()(ZIndexed* const& x, ZIndexed* const& y) const;
};
