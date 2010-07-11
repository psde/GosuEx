#include "Widget.hpp"
#include <Gosu/Math.hpp>
#include <boost/foreach.hpp>
#include <algorithm>
#include <GosuEx/Frames/FrameManager.hpp>
#include <map>
#include <set>
#include <iostream>

using namespace GosuEx::Frames;

struct Widget::Impl {
	// Widget's parent
	Widget* parent;
	// Widget's children
	std::set<Widget*> children;
	// Name
	std::wstring name;
	// Coordinates
	Unit x, y, z;
	// Size
	Unit width, height;
	// Visibility, enabled
	bool visible, enabled;
	// Highlighted
	bool highlighted, highlightable;
	// Relation
	double relX, relY;
	// Factors
	double factorX, factorY;
	// Buttons
	std::map<Gosu::Button, boost::function<void(Widget*, Gosu::Button btn)> > btnUp, btnDown, click;
	std::map<Gosu::Button, bool> clicked;
	typedef std::map<Gosu::Button, bool> ButtonBoolMap;
};

Widget::Widget():
	pimpl(new Impl)
{
	pimpl->parent = NULL;
	pimpl->name = L"";
	pimpl->x = pimpl->y = pimpl->z = 0;
	pimpl->width = pimpl->height = 0;
	pimpl->visible = pimpl->enabled = true;
	pimpl->highlighted = false;
	pimpl->relX = pimpl->relY = 0;
	pimpl->factorX = pimpl->factorY = 1;
	pimpl->highlightable = false;
}

Widget::Widget(Unit x, Unit y, Unit z, Unit width, Unit height):
	pimpl(new Impl)
{
	pimpl->parent = NULL;
	pimpl->name = L"";
	pimpl->x = x;
	pimpl->y = y;
	pimpl->z = z;
	pimpl->width = width;
	pimpl->height = height;
	pimpl->visible = pimpl->enabled = true;
	pimpl->highlighted = false;
	pimpl->relX = pimpl->relY = 0;
	pimpl->factorX = pimpl->factorY = 1;
	pimpl->highlightable = true;
}

Widget::~Widget() {
	// hardcore. erst die parents löschen. dann die children.
	BOOST_FOREACH(Widget* it, pimpl->children) {
		it->pimpl->parent = NULL;
		delete it;
		it = NULL;
	}
	pimpl->children.clear();
	pimpl->click.clear();
	pimpl->btnDown.clear();
	pimpl->btnUp.clear();
	pimpl->clicked.clear();
}

// Coordinates
Unit Widget::x() const { return pimpl->x; }
Unit Widget::dispX() const { return x()-relX()*dispWidth(); }
Unit Widget::y() const { return pimpl->y; }
Unit Widget::dispY() const { return y()-relY()*dispHeight(); }
Unit Widget::z() const { return pimpl->z; }
Unit Widget::width() const { return pimpl->width; }
Unit Widget::dispWidth() const { return width()*factorX(); }
Unit Widget::height() const { return pimpl->height; }
Unit Widget::dispHeight() const { return height()*factorY(); }

double Widget::relX() const { return pimpl->relX; }
double Widget::relY() const { return pimpl->relY; }
double Widget::factorX() const { return pimpl->factorX; }
double Widget::factorY() const { return pimpl->factorY; }

// Returns true if enabled, false if not
bool Widget::enabled() const { 
	if (!parent())
		return pimpl->enabled;
	return parent()->enabled() && pimpl->enabled;
}

// Returns true if visible, false if not
bool Widget::visible() const { 
	if (!parent())
		return pimpl->visible;
	return parent()->visible() && pimpl->visible;
}
// Highlighted?
bool Widget::highlighted() const { return pimpl->highlighted; }

// Returns the parent
Widget* Widget::parent() const { return pimpl->parent; }
// Returns the name
const std::wstring& Widget::name() const { return pimpl->name; }
// Returns all children
const std::set<Widget*> Widget::children() const { return pimpl->children; }

			
// adds an ALREADY EXISTING widget.
// do NOT use this for new widgets. Use createChild instead.
Widget& Widget::addChild(Widget* child) {
	pimpl->children.insert(child);
	child->pimpl->parent = this;
	return *child;
}
			
// adds an NEW widget. Do not use this if you createChild-ed
// child already before
Widget& Widget::createChild(Widget* child) {
	addChild(child);
	FrameManager::singleton().addWidget(child);
	return *child;
}

// adds an NEW widget and set its name. Do not use this if you createChild-ed
// child already before
Widget& Widget::createChild(Widget* child, const std::wstring& name) {
	child->pimpl->name = name;
	createChild(child);
	return *child;
}

// removes the widget. IT IS NOT DEAD YET.
// Use this to move widgets around.
void Widget::removeChild(Widget* child) {
	child->pimpl->parent = NULL;
	pimpl->children.erase(child);
}
		
// deletes and removes this widget. Use this for memory purposes.
void Widget::deleteChild(Widget* child) {
	removeChild(child);
	FrameManager::singleton().deleteWidget(child);
}

// set the parent. may be buggy.
void Widget::setParent(Widget* newParent) {
	if (parent() != NULL)
		parent()->removeChild(this);
	pimpl->parent = newParent;
	parent()->addChild(this);
}

// set a widget's name
void Widget::setName(const std::wstring& newName) {
	if (!name().empty())
		FrameManager::singleton().removeNamedWidget(name());
	pimpl->name = newName;
	FrameManager::singleton().addNamedWidget(this);
}

void Widget::update() {
	if (!shouldDraw())
		return;
	BOOST_FOREACH(Widget* it, pimpl->children) {
		it->update();
	}
}

void Widget::draw() {
	if (!shouldDraw())
		return;
	BOOST_FOREACH(Widget* it, pimpl->children) {
		it->draw();
	}
}

// "MOUSE IS NOW OVER WIDGET"
// FrameManager only.
void Widget::hover() {
	pimpl->highlighted = true;

	BOOST_FOREACH(Widget* it, pimpl->children) {
		if (it->z() < z())
			it->hover();
	}

	if (onHover)
		onHover(this);
}

// FrameManager only.
void Widget::blur() {
	pimpl->highlighted = false;

	BOOST_FOREACH(Widget* it, pimpl->children) {
		if (it->z() < z())
			it->blur();
	}

	BOOST_FOREACH(Impl::ButtonBoolMap::value_type& b, pimpl->clicked) {
		b.second = false;
	}

	if (onBlur)
		onBlur(this);
}

// Set coordinates
void Widget::setX(Unit newX) { pimpl->x = newX; }
void Widget::setY(Unit newY) { pimpl->y = newY; }
void Widget::setZ(Unit newZ) { pimpl->z = newZ; }
			
void Widget::setWidth(Unit newWidth) { pimpl->width = newWidth; }
void Widget::setHeight(Unit newHeight) { pimpl->height = newHeight; }

void Widget::setRelX(double newRelX) { pimpl->relX = newRelX; }
void Widget::setRelY(double newRelY) { pimpl->relY = newRelY; }

void Widget::setFactorX(double newFactorX) { pimpl->factorX = newFactorX; }
void Widget::setFactorY(double newFactorY) { pimpl->factorY = newFactorY; }

bool Widget::highlightable() const { return visible() && pimpl->highlightable; }
void Widget::setHighlightable(bool highlightable) { pimpl->highlightable = highlightable; }

bool Widget::shouldDraw() const {
	return visible();
}

bool Widget::shouldUpdate() const {
	return enabled() && visible();
}

void Widget::buttonDown(Gosu::Button btn) {
	if (!shouldUpdate())
		return;

	if (pimpl->btnDown.find(btn) != pimpl->btnDown.end()) {
		pimpl->btnDown[btn](this, btn);
		pimpl->clicked[btn] = true;
	}

	pimpl->clicked[btn] = true;

	BOOST_FOREACH(Widget* it, pimpl->children) {
		if (it->z() < z())
			it->buttonDown(btn);
	}
}

void Widget::buttonUp(Gosu::Button btn) {
	if (!shouldUpdate())
		return;

	if (pimpl->btnUp.find(btn) != pimpl->btnUp.end())
		pimpl->btnUp[btn](this, btn);
	
	if (pimpl->clicked.find(btn) != pimpl->clicked.end() && pimpl->clicked[btn]) {
		pimpl->clicked[btn] = false;
		if (pimpl->click.find(btn) != pimpl->click.end())
			pimpl->click[btn](this, btn);
	}
	
	BOOST_FOREACH(Widget* it, pimpl->children) {
		if (it->z() < z())
			it->buttonUp(btn);
	}
}

void Widget::setButtonUpHandler(Gosu::Button btn, boost::function<void(Widget*, Gosu::Button btn)> fct) {
	pimpl->btnUp[btn] = fct;
}

void Widget::setButtonDownHandler(Gosu::Button btn, boost::function<void(Widget*, Gosu::Button btn)> fct) {
	pimpl->btnDown[btn] = fct;
}

void Widget::setClickHandler(Gosu::Button btn, boost::function<void(Widget*, Gosu::Button btn)> fct) {
	pimpl->click[btn] = fct;
}

void Widget::show() {
	pimpl->visible = true;
}

void Widget::hide() {
	pimpl->visible = false;
}

void Widget::enable() {
	pimpl->enabled = true;
}

void Widget::disable() {
	pimpl->enabled = false;
}

bool Widget::clicked(Gosu::Button btn) {
	return pimpl->clicked.find(btn) != pimpl->clicked.end() && pimpl->clicked[btn];
}

void Widget::setParentLikeANinja(Widget* mommy) {
	pimpl->parent = mommy;
}

void GosuEx::Frames::dumpWidgets(Widget* root, unsigned int depth) {
	std::string pre;
	for (unsigned int i = 0; i < depth; i++)
		pre += "  ";
	std::cout << pre << "Dumping " << typeid(*root).name() << std::endl;
	std::cout << pre << root->x() << "/" << root->y() << "/" << root->z() << " - " << root->width() << "x" << root->height() << std::endl;
	BOOST_FOREACH(Widget* it, root->children()) {
		dumpWidgets(it, depth+1);
	}
}