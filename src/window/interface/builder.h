#include <window/interface/interface.h>
#include <context.h>

class InterfaceBuilder {
protected:
    InterfaceGlyph* root;
public:
    virtual void build(Context* context, int w, int h) = 0;
};

class DefaultInterfaceBuilder : public InterfaceBuilder {
private:
public:
    void build(Context* context, int w, int h) override;
};
