#include <window/graphics/zindex.h>
#include "zindex.h"

void ZIndexed::set_z_index(int z_index) {
    this->z_index = z_index;
    on_z_index_update();
}

int ZIndexed::get_z_index() {
    return z_index;
}

bool ZIndexComporator::operator()(ZIndexed * const& x, ZIndexed* const& y) const {
    return x->get_z_index() < y->get_z_index();
}
