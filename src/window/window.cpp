#include <window/window.h>
#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <math.h>
#include <window/interface/builder.h>
#include <window/interface/instruments.h>

Window::Window() {
    context = new Context{};
    context->shapes_builder = new ShapesBuilder();
    context->current_instrument = new PointCreatorInstrument(context);
}

int Window::run() {
    GLFWwindow *window;
    
    if (!glfwInit()) {
        return -1;
    }
    
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE, NULL, NULL);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    
    if (!window) {
        glfwTerminate();
        return -1;
    }

    int w, h;
    glfwGetWindowSize(window, &w, &h);

    context->shapes_builder->build();

    InterfaceBuilder* interface_builder = new DefaultInterfaceBuilder();
    interface_builder->build(context, w, h);

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, EventHandler::key_callback);
    glfwSetMouseButtonCallback(window, EventHandler::mouse_button_callback);
    glfwSetCursorPosCallback(window, EventHandler::cursor_position_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return -1;
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    while (!glfwWindowShouldClose(window)) {
        glfwGetWindowSize(window, &w, &h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, w, h, 0, 0, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glViewport( 0, 0, w, h );
        glClearColor(.05, .35, .13, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        
        Drawer::get_drawer().draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}