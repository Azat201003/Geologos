#include <spdlog/spdlog.h>
#include <window/window.h>

int main() {
	spdlog::info("Program started");
  Window *window = new Window();
  return window->run();
}
