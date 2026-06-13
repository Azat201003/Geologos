#include <context.h>
#include <window/interface/instruments.h>

void Context::set_current_instrument(Instrument* instrument) {
	std::lock_guard<std::mutex> lock(mutex);
	current_instrument.reset(instrument);
}

Context::~Context() = default;
