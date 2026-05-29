#include <window/window.h>
#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <math.h>

Window::Window() {
    shapes_builder = new ShapesBuilder();
    event_handler = new EventHandler(shapes_builder);
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
    
    shapes_builder->build();

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, event_handler->key_callback);
    glfwSetMouseButtonCallback(window, event_handler->mouse_button_callback);
    glfwSetCursorPosCallback(window, event_handler->cursor_position_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return -1;
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    while (!glfwWindowShouldClose(window)) {
        int w, h;
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