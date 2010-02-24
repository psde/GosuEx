#include "Image.hpp"

using namespace GosuEx;

struct Widgets::Image::Impl {
		GosuImageP image;
};

Widgets::Image::Image(double x, double y, double z, GosuImageP image):
Colorable(x, y, z, image->width(), image->height()), pimpl(new Impl) {
		pimpl->image = image;
}

Widgets::Image::~Image() {}

GosuImageP Widgets::Image::image() { return pimpl->image; }

void Widgets::Image::setImage(GosuImageP newImage) { pimpl->image = newImage; reset(); }

void Widgets::Image::draw() {
		if (!Widget::shouldHandleDraw()) return;
		Widget::draw();
		image()->draw(x()-relX()*factorX()*width(), y()-relY()*factorY()*height(), z(), factorX(), factorY(), color(), alphaMode());
}

void Widgets::Image::reset() {
		setSize(image()->width(), image()->height());
}