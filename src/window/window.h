#include <window/controller/controller.h>
#include <string>

class Window {
private:
    Context* context;
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;
    const char* TITLE = "Geologos";
public:
    Window();
    EventHandler* event_handler;
    int run();
};