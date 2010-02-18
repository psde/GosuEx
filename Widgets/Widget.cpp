#include "Widget.hpp"
#include "Effects/Effect.hpp"
#include <vector>

using namespace GosuEx;

struct Widgets::Widget::Impl {
		double x;
		double y;
		double z;
		double relX;
		double relY;
		double factorX;
		double factorY;
		double width;
		double height;
		bool enabled;
		bool visible;
		bool aimed;
		bool leftClick, rightClick;
		std::vector<WidgetP> children;
		WidgetWP myPointer;
		Gosu::Graphics* graphics;
		Gosu::Input* input;
		Effects::EffectP effect;
};

Widgets::Widget::Widget():
pimpl(new Impl) {
		pimpl->x = pimpl->y = pimpl->z = pimpl->width = pimpl->height = pimpl->relX = pimpl->relY = pimpl->factorX = pimpl->factorY = 0.0;
		pimpl->enabled = pimpl->visible = true;
		pimpl->aimed = false;
		pimpl->leftClick = pimpl->rightClick = false;
		//pimpl->myPointer.reset(this);
		pimpl->graphics = NULL;
		pimpl->input = NULL;
}

Widgets::Widget::Widget(double x, double y, double z, double width, double height):
pimpl(new Impl) {
		pimpl->x = x;
		pimpl->y = y;
		pimpl->z = z;
		pimpl->width = width;
		pimpl->height = height;
		pimpl->relX = pimpl->relY = 0.0;
		pimpl->factorX = pimpl->factorY = 1.0;
		pimpl->enabled = pimpl->visible = true;
		pimpl->leftClick = pimpl->rightClick = false;
		pimpl->aimed = false;
		//pimpl->myPointer.reset(this);
		pimpl->graphics = NULL;
		pimpl->input = NULL;
}

Widgets::Widget::~Widget() {}

bool Widgets::Widget::shouldHandleUpdate() {
		return enabled() && visible();
}

void Widgets::Widget::update() {
		if (effect()) effect()->update();
		if (!shouldHandleUpdate()) return;
		for (std::vector<WidgetP>::iterator it = pimpl->children.begin(); it != pimpl->children.end(); ++it) {
				(**it).update();
		}

		if (visible()) {
				bool nowAiming = checkAiming();
				if (!aimed() && nowAiming) {
						if (onHover) onHover(ptr());
				} else if (aimed() && !nowAiming) {
						if (onBlur) onBlur(ptr());
				}
				pimpl->aimed = nowAiming;
		}
}

bool Widgets::Widget::shouldHandleDraw() {
		return visible();
}

void Widgets::Widget::draw() {
		if (effect()) effect()->draw();
		if (!shouldHandleDraw()) return;
		for (std::vector<WidgetP>::iterator it = pimpl->children.begin(); it != pimpl->children.end(); ++it) {
				(**it).draw();
		}
}

bool Widgets::Widget::shouldHandleButtonDown() {
		return enabled() && visible();
}

void Widgets::Widget::buttonDown(Gosu::Button btn) {
		if (effect()) effect()->buttonDown(btn);
		if (!shouldHandleButtonDown()) return;
		for (std::vector<WidgetP>::iterator it = pimpl->children.begin(); it != pimpl->children.end(); ++it) {
				(**it).buttonDown(btn);
		}
		if (btn == Gosu::msLeft) {
				if (aimed()) {
						if (onLeftDown)
								onLeftDown(ptr());
						pimpl->leftClick = true;
				}
		} else if (btn == Gosu::msRight) {
				if (aimed()) {
						if (onRightDown)
								onRightDown(ptr());
						pimpl->rightClick = true;
				}
		}
}

bool Widgets::Widget::shouldHandleButtonUp() {
		return enabled() && visible();
}

void Widgets::Widget::buttonUp(Gosu::Button btn) {
		if (effect()) effect()->buttonUp(btn);
		if (!shouldHandleButtonUp()) return;
		for (std::vector<WidgetP>::iterator it = pimpl->children.begin(); it != pimpl->children.end(); ++it) {
				(**it).buttonUp(btn);
		}
		if (btn == Gosu::msLeft) {
				if (aimed()) {
						if (onLeftUp)
								onLeftUp(ptr());
						if (leftClick() && onLeftClick)
								onLeftClick(ptr());
				}
				
				if (leftClick())
						pimpl->leftClick = false;
		} else if (btn == Gosu::msRight) {
				if (aimed()) {
						if (onRightUp)
								onRightUp(ptr());
						if (rightClick() && onRightClick)
								onRightClick(ptr());
				}
				if (rightClick())
						pimpl->rightClick = false;
		}
}

void Widgets::Widget::cleanUp() {
		for (std::vector<WidgetP>::iterator it = pimpl->children.begin(); it != pimpl->children.end(); ++it) {
				(**it).cleanUp();
		}
		onHover.clear();
		onBlur.clear();
		onLeftDown.clear();
		onRightDown.clear();
		onLeftUp.clear();
		onRightUp.clear();
		onLeftClick.clear();
		onRightClick.clear();
		if (effect()) effect()->cleanUp();
}

void Widgets::Widget::attachEffect(Effects::EffectP effect) { 
		pimpl->effect = effect;
		effect->attachTo(ptr());
}

void Widgets::Widget::detachEffect() { pimpl->effect.reset(); }

void Widgets::Widget::setPosition(double newX, double newY, double newZ) {
		double oX = newX-x(), oY = newY-y();
		for (std::vector<WidgetP>::iterator it = pimpl->children.begin(); it != pimpl->children.end(); ++it) 
				(**it).setPosition((**it).x()+oX, (**it).y()+oY, (**it).z());
		pimpl->x = newX;
		pimpl->y = newY;
		pimpl->z = newZ;
}
// Uh. I think this should not be inherited.
void Widgets::Widget::setRelations(double newX, double newY) {
	pimpl->relX = newX;
	pimpl->relY = newY;
}

void Widgets::Widget::setFactors(double newX, double newY) {
	pimpl->factorX = newX;
	pimpl->factorY = newY;
}

void Widgets::Widget::setSize(double newWidth, double newHeight) {
		pimpl->width = newWidth;
		pimpl->height = newHeight;
}

void Widgets::Widget::setEnabled(bool enabled) {	
		pimpl->enabled = enabled;
		if (!enabled) pimpl->aimed = false;
}

void Widgets::Widget::setVisibility(bool visible) {	
		pimpl->visible = visible;
		if (!visible) pimpl->aimed = false;
}

void Widgets::Widget::setGosuGraphics(Gosu::Graphics* graphics) { pimpl->graphics = graphics; }

void Widgets::Widget::setGosuInput(Gosu::Input* input) { 
		pimpl->input = input;
		if (!input) pimpl->aimed = false;
}

double Widgets::Widget::x() const { return pimpl->x; }
double Widgets::Widget::y() const { return pimpl->y; }
double Widgets::Widget::z() const { return pimpl->z; }
double Widgets::Widget::relX() const { return pimpl->relX; }
double Widgets::Widget::relY() const { return pimpl->relY; }
double Widgets::Widget::factorX() const { return pimpl->factorX; }
double Widgets::Widget::factorY() const { return pimpl->factorY; }
double Widgets::Widget::width() const { return pimpl->width; }
double Widgets::Widget::height() const { return pimpl->height; }
bool Widgets::Widget::enabled() const { return pimpl->enabled; }
bool Widgets::Widget::visible() const { return pimpl->visible; }
std::size_t Widgets::Widget::size() const { return pimpl->children.size(); }
bool Widgets::Widget::aimed() const { return pimpl->aimed; }
bool Widgets::Widget::leftClick() const { return pimpl->leftClick; }
bool Widgets::Widget::rightClick() const { return pimpl->rightClick; }

Widgets::WidgetP Widgets::Widget::ptr() {
		WidgetP ptr = pimpl->myPointer.lock();
		return ptr;
}

Widgets::WidgetWP Widgets::Widget::wptr() {
		return pimpl->myPointer;
}

void Widgets::Widget::setPtr(WidgetP nptr) {
		if (ptr()) return;
		pimpl->myPointer = nptr;
}

Gosu::Graphics* Widgets::Widget::gosuGraphics() const { return pimpl->graphics; }
Gosu::Input* Widgets::Widget::gosuInput() const { return pimpl->input; }

Widgets::WidgetP Widgets::Widget::child(std::size_t index) {
		return pimpl->children[index];
}

Widgets::Effects::EffectP Widgets::Widget::effect() { return pimpl->effect; }

Widgets::Widget& Widgets::Widget::operator<<(WidgetP child) {
		pimpl->children.push_back(child);
		child->setGosuGraphics(gosuGraphics());
		child->setGosuInput(gosuInput());
		return *this;
}

Widgets::Widget& Widgets::Widget::operator<<(Widget* child) {
		
		if (!child->ptr()) {
				WidgetP nptr(child);
				child->setPtr(nptr);
				return *this << child->ptr();
		}

		return *this << child->ptr();
}

Widgets::WidgetP Widgets::Widget::operator&() {
		return ptr();
}

void Widgets::Widget::reset() {}

bool Widgets::Widget::checkAiming() {
		if (!gosuInput() || !width() || !height() || !visible() || !enabled()) return false;
		return gosuInput()->mouseX() >= x()-relX()*width()*factorX() && gosuInput()->mouseX() <= x()-relX()*width()*factorX()+width()*factorX() && gosuInput()->mouseY() >= y()-relY()*factorY()*height() && gosuInput()->mouseY() <= y()-relY()*factorY()*height()+height();
}