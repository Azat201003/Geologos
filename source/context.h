#pragma once
#include <mutex>
#include <memory>

class ShapesBuilder;
class Instrument;

struct Context {
private:
	std::unique_ptr<Instrument> current_instrument;
	std::mutex mutex;
public:
	~Context();
	ShapesBuilder* shapes_builder = nullptr;
	Instrument* get_current_instrument() {
		std::lock_guard<std::mutex> lock(mutex);
		return current_instrument.get();
	}
	void set_current_instrument(Instrument* instrument);
};