#include <window/interface/interface.h>
#include <context.h>

class InterfaceBuilder {
protected:
	InterfaceGlyph* root;
public:
	virtual void build(Context* context, int w, int h) = 0;
};

class DefaultInterfaceBuilder : public InterfaceBuilder, private Callbackable {
private:
	void resize_callback(GLFWwindow* window, int width, int height) override;
public:
	DefaultInterfaceBuilder() : Callbackable() {}
	void build(Context* context, int w, int h) override;
};
