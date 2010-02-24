#include "Cursor.hpp"

using namespace GosuEx;

Widgets::Cursor::Cursor(double z, GosuImageP image):
Image(0, 0, z, image) {
		if (gosuInput())
				setPosition(gosuInput()->mouseX(), gosuInput()->mouseY(), z);
		setEnabled(false);
}

Widgets::Cursor::~Cursor() {}

void Widgets::Cursor::update() {
		// because we are disabled, this would fail in so many ways
		// *facepalm*
		//if (!Image::update()) return false;
		if (!gosuInput()) return;
		Widget::update();
		setPosition(gosuInput()->mouseX(), gosuInput()->mouseY(), z());
}

// Okay
// It's not ignored
// It's forced to false
// I APOLOGIZE

void Widgets::Cursor::setEnabled(bool enabled) {
		Widget::setEnabled(false);
}