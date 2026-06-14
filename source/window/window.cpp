#include <window/window.h>

#include "window/graphics/shaders.h"

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <math.h>
#include <window/interface/builder.h>
#include <window/interface/instruments.h>
#include <window/graphics/matrix.h>
#include <spdlog/spdlog.h>

Window::Window() {
	context = new Context{};
	context->shapes_builder = new ShapesBuilder();
	context->set_current_instrument(new PointCreatorInstrument(context));
}

int Window::run() {	   
	if (!glfwInit()) {
		spdlog::critical("GLFW initialization failed");
		return -1;
	}
	spdlog::info("GLFW successfuly initialized");

	GLFWwindow *window;
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE, NULL, NULL);
	
	if (!window) {
		spdlog::critical("GLFW window creation failed");
		glfwTerminate();
		return -1;
	}
	
	spdlog::info("GLFW window successfuly created");

	int w, h;
	glfwGetWindowSize(window, &w, &h);

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		spdlog::critical("GLAD loading failed");
		return -1;
	}
	spdlog::info("GLAD successfuly loaded");

	glfwSetKeyCallback(window, EventHandler::key_callback);
	glfwSetMouseButtonCallback(window, EventHandler::mouse_button_callback);
	glfwSetCursorPosCallback(window, EventHandler::cursor_position_callback);
	glfwSetFramebufferSizeCallback(window, EventHandler::resize_callback);
	
	spdlog::info("Callbacks set");

	glEnable(GL_DEPTH_TEST);

	update_matrix(w, h);
	glViewport(0, 0, w, h);

	glUseProgram(0);
	
		ShaderStorage::load();
	context->shapes_builder->build();
	InterfaceBuilder* interface_builder = new DefaultInterfaceBuilder();
	interface_builder->build(context, w, h);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(.05, .35, .13, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		Drawer::get_drawer().draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}
