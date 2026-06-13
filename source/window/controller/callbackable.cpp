#include <window/controller/callbackable.h>

#include <window/controller/controller.h>

Callbackable::Callbackable() {
	EventHandler::add_listener(this);
}
