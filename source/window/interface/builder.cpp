#include <window/interface/builder.h>
#include <window/interface/instruments.h>
#include <iostream>

void DefaultInterfaceBuilder::build(Context* context, int w, int h) {
	FocusManager* focus_manager = new FocusManager();
	InterfaceGlyph* main = new InterfaceGlyph(context, vec2{0., 0.}, vec2{1., 1.});
	FieldGlyph* field = new FieldGlyph(context, vec2{0., 0.}, vec2{.8, 1.}, focus_manager);
	field->set_z_index(1);
	// std::cout << 3 << std::endl;
	FocusableFilledGlyph* instruments_selector = new FocusableFilledGlyph(context, vec2{.8, 0.}, vec2{.2, 1.}, focus_manager);
	instruments_selector->set_z_index(7);
	RadioButtonGlyph* instrument1_button = new RadioButtonGlyph(context, vec2{0., 0.}, vec2{1., .1}, vec3{.2, .1, .3}, [context] () {
		context->set_current_instrument(new PointSelectorInstrument(context));
	}, focus_manager);
	instrument1_button->set_z_index(8);
	RadioButtonGlyph* instrument2_button = new RadioButtonGlyph(context, vec2{0., .1}, vec2{1., .1}, vec3{.2, .1, .4}, [context] () {
		context->set_current_instrument(new PointCreatorInstrument(context));
	}, focus_manager);
	instrument2_button->set_z_index(8);
	DetailsGlyph* details = new DetailsGlyph(context, vec2{0., .8}, vec2{.4, .2}, focus_manager);
	details->set_z_index(7);
	instruments_selector->add_child(instrument1_button);
	instruments_selector->add_child(instrument2_button);
	main->add_child(field);
	main->add_child(instruments_selector);
	main->add_child(details);
	main->update_pos(vec4{0, 0, float(w), float(h)});
	root = main;
}

void DefaultInterfaceBuilder::resize_callback(GLFWwindow* window, int width, int height) {
	root->update_pos(vec4{0, 0, float(width), float(height)});
}
