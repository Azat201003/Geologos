#pragma once

class ShapesBuilder;
class Instrument;

struct Context {
    ShapesBuilder* shapes_builder = nullptr;
    Instrument* current_instrument = nullptr;
};