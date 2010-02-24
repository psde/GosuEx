#include "Button.hpp"
#include "Colorable.hpp"

using namespace GosuEx;

struct Widgets::Button::Impl {
		WidgetP normal, aimed, disabled;
};

Widgets::Button::Button(GosuEx::Widgets::WidgetP normal, GosuEx::Widgets::WidgetP aimed, GosuEx::Widgets::WidgetP disabled):
Widget(normal->x(), normal->y(), normal->z(), normal->width(), normal->height()), pimpl(new Impl) {
		setRelations(normal->relX(), normal->relY());
		setFactors(normal->factorX(), normal->factorY());
		pimpl->normal = normal;
		pimpl->aimed = aimed;
		pimpl->disabled = disabled;
		reset();
}

Widgets::Button::~Button() {}

void Widgets::Button::draw() {
		if (!shouldHandleDraw()) return;
		Widget::draw();
		if (enabled()) {
				if (aimed() && pimpl->aimed)
						pimpl->aimed->draw();
					else
						pimpl->normal->draw();
		} else {
				if (pimpl->disabled)
						pimpl->disabled->draw();
		}
}

void Widgets::Button::setPosition(double newX, double newY, double newZ) {
	Widget::setPosition(newX, newY, newZ);
	double mX = x() - relX()*factorX()*width() + width()*factorX()/2;
	double mY = y() - relY()*factorY()*height() + height()*factorY()/2;
	normalWidget()->setPosition(mX, mY, z());
	normalWidget()->setRelations(0.5, 0.5);

	if (aimedWidget()) {
		aimedWidget()->setPosition(mX, mY, z());
		aimedWidget()->setRelations(0.5, 0.5);
	}
		
	if (disabledWidget()) {
		disabledWidget()->setPosition(mX, mY, z());
		disabledWidget()->setRelations(0.5, 0.5);
	}
}

Widgets::WidgetP Widgets::Button::normalWidget() const { return pimpl->normal; }
Widgets::WidgetP Widgets::Button::aimedWidget() const { return pimpl->aimed; }
Widgets::WidgetP Widgets::Button::disabledWidget() const { return pimpl->disabled; }

void Widgets::Button::setGosuGraphics(Gosu::Graphics* graphics) {
		Widget::setGosuGraphics(graphics);
		pimpl->normal->setGosuGraphics(graphics);
		if (pimpl->aimed)
				pimpl->aimed->setGosuGraphics(graphics);
		if (pimpl->disabled)
				pimpl->disabled->setGosuGraphics(graphics);
}

void Widgets::Button::setGosuInput(Gosu::Input* input) {
		Widget::setGosuInput(input);
		pimpl->normal->setGosuInput(input);
		if (pimpl->aimed)
				pimpl->aimed->setGosuInput(input);
		if (pimpl->disabled)
				pimpl->disabled->setGosuInput(input);
}

void Widgets::Button::reset() {
	Widget::reset();
	setPosition(x(), y(), z());
}