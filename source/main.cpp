#include <spdlog/spdlog.h>
#include <window/window.h>

int main() {
	spdlog::set_level(spdlog::level::debug);
	spdlog::info("Program started");
  Window *window = new Window();
  return window->run();
}
