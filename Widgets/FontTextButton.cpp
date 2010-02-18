#include "FontTextButton.hpp"

using namespace GosuEx;

struct Widgets::FontTextButton::Impl {
		FontTextP text;
};

Widgets::FontTextButton::FontTextButton(GosuEx::Widgets::FontTextP text, GosuEx::Widgets::WidgetP normal, GosuEx::Widgets::WidgetP aimed, GosuEx::Widgets::WidgetP disabled):
Button(normal, aimed, disabled), pimpl(new Impl) {
		pimpl->text = text;
		reset();
}

Widgets::FontTextButton::~FontTextButton() {}

void Widgets::FontTextButton::draw() {
		if (!shouldHandleDraw()) return;
		Button::draw();
		pimpl->text->draw();
}

void Widgets::FontTextButton::setPosition(double newX, double newY, double newZ) {
	Button::setPosition(newX, newY, newZ);
	pimpl->text->setPosition(x()-relX()*factorX()*width()+width()*factorX()/2, y()-relY()*factorY()*height()+height()*factorY()/2, pimpl->text->z());
	pimpl->text->setRelations(0.5, 0.5);
}

void Widgets::FontTextButton::setGosuGraphics(Gosu::Graphics *graphics) {
		Button::setGosuGraphics(graphics);
		pimpl->text->setGosuGraphics(graphics);
}

void Widgets::FontTextButton::setGosuInput(Gosu::Input *input) {
		Button::setGosuInput(input);
		pimpl->text->setGosuInput(input);
}

void Widgets::FontTextButton::reset() {
	Button::reset();
	pimpl->text->setFactors(factorX(), factorY());
	pimpl->text->setPosition(x()-relX()*factorX()*width()+width()*factorX()/2, y()-relY()*factorY()*height()+height()*factorY()/2, pimpl->text->z());
	pimpl->text->setRelations(0.5, 0.5);
}