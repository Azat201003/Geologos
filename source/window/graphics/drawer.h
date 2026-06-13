#pragma once

#include <vector>
#include <window/graphics/zindex.h>

class Drawer;

class Drawable {
public:
	Drawable();
	virtual ~Drawable();
	virtual void pre_draw() {}
	virtual void draw() = 0;
	virtual void post_draw() {}
};

class ZIndexedDrawable : public Drawable, public virtual ZIndexed {
private:
public:
	void pre_draw() override;
	void post_draw() override;
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
