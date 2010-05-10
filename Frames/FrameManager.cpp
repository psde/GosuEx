#include "FrameManager.hpp"
#include "Widget.hpp"
#include <algorithm>
#include <Gosu/Input.hpp>

using namespace GosuEx::Frames;

FrameManager* FrameManager::gManager;

FrameManager& FrameManager::singleton() { 
	if (FrameManager::gManager == NULL)
		throw std::logic_error("Tried to get singleton of an uninitialized FrameManager");
	return *FrameManager::gManager;
}

Widget& FrameManager::root() {	
	if (!singleton().pimpl.rootWidget) {
		singleton().pimpl.rootWidget = new Widget();
		singleton().pimpl.rootWidget->setName(L"root");
		singleton().addWidget(singleton().pimpl.rootWidget);
	}
	return *singleton().pimpl.rootWidget;
}


void FrameManager::deleteWidget(Widget* widget) {
	pimpl.widgets.erase(std::find(pimpl.widgets.begin(), pimpl.widgets.end(), widget));
	if (pimpl.highlightedWidget = NULL)
		pimpl.highlightedWidget = NULL;
	delete widget;
}
FrameManager::FrameManager(Gosu::Graphics* graphics, Gosu::Input* input) {
	pimpl.graphics = graphics;
	pimpl.input = input;
	pimpl.rootWidget = NULL;
	pimpl.highlightedWidget = NULL;
}

bool FrameManager::spawn(Gosu::Graphics* graphics, Gosu::Input* input) {
	if (FrameManager::gManager != NULL)
		return false;
	FrameManager::gManager = new FrameManager(graphics, input);
	return true;
}

void FrameManager::addWidget(Widget* widget) {
	pimpl.widgets.push_back(widget);
	if (!widget->name().empty()) {
		addNamedWidget(widget);
	}
}

void FrameManager::removeNamedWidget(const std::wstring& name) {
	pimpl.namedWidgets.erase(pimpl.namedWidgets.find(name));
}

void FrameManager::addNamedWidget(Widget* widget) {
	pimpl.namedWidgets[widget->name()] = widget;
}

void FrameManager::addNamedFont(const std::wstring& name, boost::shared_ptr<Gosu::Font> font) {
	pimpl.namedFonts[name] = font;
}

Gosu::Graphics& FrameManager::graphics() const {
	return *pimpl.graphics;
}

Gosu::Input& FrameManager::input() const {
	return *pimpl.input;
}

void FrameManager::update() {
	root().update();
	// Highlights
	Unit mx = input().mouseX(), my = input().mouseY();
	Widget* possibleHL = NULL;
	BOOST_FOREACH(Widget* it, pimpl.widgets) {
		if (!it->highlightable() || !it->visible() || mx < it->dispX() || my < it->dispY() || mx > it->dispX()+it->dispWidth() || my > it->dispY()+it->dispHeight()) continue;
		if (possibleHL == NULL || possibleHL->z() < it->z())
			possibleHL = it;
	}

	if (possibleHL != NULL) {
		// We found one. Is it the old one?
		if (pimpl.highlightedWidget != possibleHL) {
			// No? Tell them he's not focussed anymore. 
			if (pimpl.highlightedWidget != NULL)
				pimpl.highlightedWidget->blur();
			// Set the new focus and focus it.
			pimpl.highlightedWidget = possibleHL;
			pimpl.highlightedWidget->hover();
		}
	}
}

void FrameManager::draw() {
	root().draw();
}

Widget& FrameManager::namedWidget(const std::wstring& name) const {
	return *pimpl.namedWidgets.at(name);
}


boost::shared_ptr<Gosu::Font> FrameManager::namedFont(const std::wstring& name) const {
	return pimpl.namedFonts.at(name);
}