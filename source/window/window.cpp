#include <window/window.h>
#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <math.h>
#include <window/interface/builder.h>
#include <window/interface/instruments.h>
#include <window/graphics/matrix.h>
#include <iostream>

Window::Window() {
    context = new Context{};
    context->shapes_builder = new ShapesBuilder();
    context->set_current_instrument(new PointCreatorInstrument(context));
}

int Window::run() {    
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow *window;
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE, NULL, NULL);
    
    if (!window) {
        glfwTerminate();
        return -1;
    }

    int w, h;
    glfwGetWindowSize(window, &w, &h);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return -1;
    }

    glfwSetKeyCallback(window, EventHandler::key_callback);
    glfwSetMouseButtonCallback(window, EventHandler::mouse_button_callback);
    glfwSetCursorPosCallback(window, EventHandler::cursor_position_callback);
    glfwSetFramebufferSizeCallback(window, EventHandler::resize_callback);

    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();

    glEnable(GL_DEPTH_TEST);

    update_matrix(w, h);
    glViewport(0, 0, w, h);

    glUseProgram(0);
    
    context->shapes_builder->build();
    InterfaceBuilder* interface_builder = new DefaultInterfaceBuilder();
    interface_builder->build(context, w, h);

    while (!glfwWindowShouldClose(window)) {
        // glfwGetWindowSize(window, &w, &h);
        // glMatrixMode(GL_PROJECTION);
        // glLoadIdentity();
        // glOrtho(0, w, h, 0, 0, 1);
        // glMatrixMode(GL_MODELVIEW);
        // glLoadIdentity();
        glClearColor(.05, .35, .13, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        Drawer::get_drawer().draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}