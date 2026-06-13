#pragma once

#include <context.h>
#include <linmath.h>
#include <shapes/builder.h>
#include <window/controller/callbackable.h>
#include <window/graphics/drawer.h>
#include <window/graphics/text.h>
#include <window/graphics/zindex.h>

#include <functional>
#include <set>
#include <vector>

class InterfaceGlyph {
private:
	bool focused;
protected:
	std::vector<InterfaceGlyph*> children;
	Context* context;
	vec4 pos{0,0,0,0}; // x1, y1, x2, y2
	vec2 offset;
	vec2 scale;
public:
	InterfaceGlyph(Context* context, vec2 offset, vec2 scale);
	void get_scale(vec2 out);
	void get_offset(vec2 out); // relative
	void add_child(InterfaceGlyph* child);
	void update_pos(vec4 new_pos); // absolute
	virtual void on_update_pos() {}
	friend class FocusManager;
};

class FocusManager;

class Focusable : public virtual ZIndexed {
private:
	bool focused = false;
	FocusManager* focus_manager;
protected:
	vec4 focus_area{0,0,0,0}; // x1, y1, x2, y2
public:
	Focusable(FocusManager*);
	virtual ~Focusable();
	virtual void on_focus() {}
	virtual void off_focus() {}
	[[nodiscard]]
	bool is_focused();
	void on_z_index_update() override;
	friend class FocusManager;
};

class FocusableGlyph : public Focusable, public InterfaceGlyph {
public:
	FocusableGlyph(Context* context, vec2 offset, vec2 scale, FocusManager* focus_manager) : Focusable(focus_manager), InterfaceGlyph(context, offset, scale) {}
private:
	void on_update_pos() override;
};

class FocusManager : public Callbackable {
private:
	std::set<Focusable*, ZIndexComporator> focusables;
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) override;
public:
	void add_focusable(Focusable*);
	void remove_focusable(Focusable*);
};

class FilledGlyph : public InterfaceGlyph, public ZIndexedDrawable {
protected:
	vec3 FILL_COLOR{.1, .2, .3};
public:
	FilledGlyph(Context* context, vec2 offset, vec2 scale) : InterfaceGlyph(context, offset, scale) {
		this->set_z_index(7);
	}
	FilledGlyph(Context* context, vec2 offset, vec2 scale, vec3 fill_color) : InterfaceGlyph(context, offset, scale) {
		vec3_dup(FILL_COLOR, fill_color);
		set_z_index(7);
	}
	virtual void draw() override;
};

class FocusableFilledGlyph : public FocusableGlyph, public ZIndexedDrawable {
protected:
	vec3 FILL_COLOR{.1, .2, .3};
public:
	FocusableFilledGlyph(Context* context, vec2 offset, vec2 scale, FocusManager* focus_manager) : FocusableGlyph(context, offset, scale, focus_manager) {
		set_z_index(7);
	}
	FocusableFilledGlyph(Context* context, vec2 offset, vec2 scale, vec3 fill_color, FocusManager* focus_manager) : FocusableGlyph(context, offset, scale, focus_manager) {
		FILL_COLOR[0] = fill_color[0]; FILL_COLOR[1] = fill_color[1]; FILL_COLOR[2] = fill_color[2];
		set_z_index(7);
	}
	virtual void draw() override;
};


class ButtonGlyph : public Callbackable, public FocusableFilledGlyph {
protected:
	std::function<void(void)> on_click_dummy;
	std::function<void(void)> on_hover_dummy;
	std::function<void(void)> off_hover_dummy;
	bool hover = false;
	bool click_started = false;
public:
	ButtonGlyph(Context* context, vec2 offset, vec2 scale, vec3 fill_color, std::function<void(void)> onclick, FocusManager* focus_manager)
																				: FocusableFilledGlyph(context, offset, scale, fill_color, focus_manager), 
																				Callbackable(),
																				on_click_dummy(onclick) {set_z_index(8);}
	virtual void on_click() { on_click_dummy(); }
	virtual void on_hover() {/* on_hover_dummy(); */}
	virtual void off_hover() {/* off_hover_dummy(); */}
	virtual void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) override;
	virtual void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) override;
};

class RadioButtonGlyph : public ButtonGlyph {
protected:
	bool selected = false;
public:
	RadioButtonGlyph(Context* context, vec2 offset, vec2 scale, vec3 fill_color, std::function<void(void)> onclick, FocusManager* focus_manager)
																				 : ButtonGlyph(context, offset, scale, fill_color, onclick, focus_manager) {}
	void on_click() override;
	virtual void on_select() {}
	virtual void on_unselect() {}
	void unselect();
};

class FieldGlyph : public Callbackable, public FocusableGlyph {	 
public:
	FieldGlyph(Context* context, vec2 offset, vec2 scale, FocusManager* focus_manager) : Callbackable(), FocusableGlyph(context, offset, scale, focus_manager) {}
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) override;
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) override;
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) override;
};

class InstrumentSelectionGlyph : public FocusableFilledGlyph {
private:
public:
	InstrumentSelectionGlyph(Context* context, vec2 offset, vec2 scale, FocusManager* focus_manager) : FocusableFilledGlyph(context, offset, scale, focus_manager) {}
};

class DetailsGlyph : public FocusableFilledGlyph {
private:
	vec3 FILL_COLOR{.1, .2, .3};
	TextDrawer* text_drawer = nullptr;
public:
	DetailsGlyph(Context* context, vec2 offset, vec2 scale, FocusManager* focus_manager) : FocusableFilledGlyph(context, offset, scale, focus_manager) {}
	void draw() override {
		FocusableFilledGlyph::draw();
		if (text_drawer == nullptr) {
			text_drawer = new TextDrawer("resources/fonts/Lato-Black.ttf", 100);
		}
		text_drawer->render(0, 200, "some text", glm::vec4{1, .5, .5, 1});
	}
};
