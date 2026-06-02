#include <window/interface/builder.h>
#include <window/interface/instruments.h>

void DefaultInterfaceBuilder::build(Context* context, int w, int h) {
    InterfaceGlyph* main = new InterfaceGlyph(context, vec2{0., 0.}, vec2{1., 1.});
    InterfaceGlyph* field = new FieldGlyph(context, vec2{0., 0.}, vec2{.8, 1.});
    InterfaceGlyph* instruments_selector = new FilledGlyph(context, vec2{.8, 0.}, vec2{.2, 1.});
    InterfaceGlyph* instrument1_button = new RadioButtonGlyph(context, vec2{0., 0.}, vec2{1., .1}, vec3{.2, .1, .3}, [context] () {
        context->set_current_instrument(new PointSelectorInstrument(context));
    }, [] () {}, [] () {});
    InterfaceGlyph* instrument2_button = new RadioButtonGlyph(context, vec2{0., .1}, vec2{1., .1}, vec3{.2, .1, .4}, [context] () {
        context->set_current_instrument(new PointCreatorInstrument(context));
    }, [] () {}, [] () {});
    instruments_selector->add_child(instrument1_button);
    instruments_selector->add_child(instrument2_button);
    main->add_child(field);
    main->add_child(instruments_selector);
    main->update_pos(vec4{0, 0, float(w), float(h)});
    root = main;
}
