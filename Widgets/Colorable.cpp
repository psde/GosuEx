#include "Colorable.hpp"

using namespace GosuEx;

struct Widgets::Colorable::Impl {
		Gosu::Color color;
		Gosu::AlphaMode almode;
};

Widgets::Colorable::Colorable():
Widget(), pimpl(new Impl) {
		setColor(Gosu::Colors::none);
		setAlphaMode(Gosu::amDefault);
}

Widgets::Colorable::Colorable(double x, double y, double z, double width, double height):
Widget(x, y, z, width, height), pimpl(new Impl) {
		setColor(Gosu::Colors::white);
		setAlphaMode(Gosu::amDefault);
}

Widgets::Colorable::~Colorable() {}

void Widgets::Colorable::draw() {
		if (!shouldHandleDraw()) return;
		Widget::draw();
		// If we have no Graphics we can't draw.
		if (!gosuGraphics()) return;
		gosuGraphics()->drawQuad(x()-relX()*factorX()*width(), y()-relY()*factorY()*height(), color(), x()-relX()*factorX()*width()+width()*factorX(), y()-relY()*factorY()*height(), color(), x()-relX()*factorX()*width()+width()*factorX(), y()-relY()*factorY()*height()+height()*factorY(), color(), x()-relX()*factorX()*width(), y()-relY()*factorY()*height()+height()*factorY(), color(), z(), alphaMode());
}

Gosu::Color Widgets::Colorable::color() const { return pimpl->color; }
Gosu::AlphaMode Widgets::Colorable::alphaMode() const { return pimpl->almode; }
double Widgets::Colorable::width() const { return Widget::width()*factorX(); }
double Widgets::Colorable::height() const { return Widget::height()*factorY(); }

void Widgets::Colorable::setColor(Gosu::Color newColor) { pimpl->color = newColor; }
void Widgets::Colorable::setAlphaMode(Gosu::AlphaMode newAlphaMode) { pimpl->almode = newAlphaMode; }

///void Widgets::Colorable::setPosition(double newX, double newY, double newZ) {		Widget::setPosition(newX+width()*relX()*factorX(), newY+height()*relY()*factorY(), newZ); }
/*bool Widgets::Colorable::checkAiming() {
		if (!gosuInput() || !width() || !height()) return false;
		return gosuInput()->mouseX() >= x()-width()*relX()*factorX() && gosuInput()->mouseX() <= x()+width() && gosuInput()->mouseY() >= y() && gosuInput()->mouseY() <= y()+height();
}*/