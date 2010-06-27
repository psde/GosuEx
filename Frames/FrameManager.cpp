#include "FrameManager.hpp"
#include "Widget.hpp"
#include <algorithm>
#include <Gosu/Input.hpp>
#include <Gosu/Utility.hpp>
#include <boost/foreach.hpp>
#include <exception>

using namespace GosuEx::Frames;

FrameManager* FrameManager::gManager;

struct FrameManager::Impl {
	FrameSet* currentSet;
	std::map<std::wstring, FrameSet*> namedSets;
	std::map<std::wstring, boost::shared_ptr<Gosu::Font> > namedFonts;
	Gosu::Graphics* graphics;
	Gosu::Input* input;
};

struct FrameSet::Impl {
	std::vector<Widget*> widgets;
	Widget* rootWidget;
	Widget* highlightedWidget;
	Widget* textInputWidget;
	std::map<std::wstring, Widget*> namedWidgets;
	std::wstring name;
};

FrameManager& FrameManager::singleton() { 
	if (FrameManager::gManager == NULL)
		throw std::logic_error("Tried to get singleton of an uninitialized FrameManager");
	return *FrameManager::gManager;
}

Widget& FrameSet::root() {
	if (!pimpl->rootWidget) {
		pimpl->rootWidget = new Widget();
		pimpl->rootWidget->setName(L"root");
		this->addWidget(pimpl->rootWidget);
	}
	return *pimpl->rootWidget;
}

Widget& FrameManager::root() {
	return singleton().currentSet().root();
}


void FrameManager::deleteWidget(Widget* widget) {
	currentSet().deleteWidget(widget);
}

void FrameSet::deleteWidget(Widget* widget) {
	pimpl->widgets.erase(std::find(pimpl->widgets.begin(), pimpl->widgets.end(), widget));
	if (pimpl->highlightedWidget == widget)
		pimpl->highlightedWidget = NULL;
	if (pimpl->textInputWidget == widget)
		pimpl->textInputWidget = NULL;
	delete widget;
}

FrameManager::FrameManager(Gosu::Graphics& graphics, Gosu::Input& input, const std::wstring& setname):
	pimpl(new Impl)
{
	pimpl->graphics = &graphics;
	pimpl->input = &input;
	pimpl->currentSet = new FrameSet(setname);
	pimpl->namedSets[setname] = pimpl->currentSet;
	currentSet().activate();
}

FrameSet::FrameSet(const std::wstring& name):
	pimpl(new Impl)
{
	pimpl->rootWidget = pimpl->textInputWidget = pimpl->highlightedWidget = NULL;
	pimpl->name = name;
}

FrameSet::FrameSet(const std::wstring& name, Widget* root):
	pimpl(new Impl)
{
	pimpl->rootWidget = root;
	pimpl->textInputWidget = pimpl->highlightedWidget = NULL;
	pimpl->name = name;
}

FrameManager::~FrameManager() {
	for (std::map<std::wstring, FrameSet*>::iterator it = pimpl->namedSets.begin(); it != pimpl->namedSets.end(); ++it) {
		delete it->second;
	}
	pimpl->graphics = NULL;
	pimpl->input = NULL;
}

FrameSet::~FrameSet() {
	if (pimpl->rootWidget)
		delete pimpl->rootWidget;
	pimpl->highlightedWidget = NULL;
	pimpl->textInputWidget = NULL;
}

bool FrameManager::spawn(Gosu::Graphics& graphics, Gosu::Input& input, const std::wstring& setname) {
	if (FrameManager::gManager != NULL)
		return false;
	FrameManager::gManager = new FrameManager(graphics, input, setname);
	return true;
}

void FrameManager::despawn() {
	if (FrameManager::gManager) {
		delete FrameManager::gManager;
		FrameManager::gManager = NULL;
	}
}

void FrameSet::addWidget(Widget* widget) {
	pimpl->widgets.push_back(widget);
	if (!widget->name().empty()) {
		addNamedWidget(widget);
	}
}

void FrameManager::addWidget(Widget* widget) {
	currentSet().addWidget(widget);
}

void FrameSet::removeNamedWidget(const std::wstring& name) {
	pimpl->namedWidgets.erase(pimpl->namedWidgets.find(name));
}
void FrameManager::removeNamedWidget(const std::wstring& name) {
	currentSet().removeNamedWidget(name);
}

void FrameSet::addNamedWidget(Widget* widget) {
	pimpl->namedWidgets[widget->name()] = widget;
}

void FrameManager::addNamedWidget(Widget* widget) {
	currentSet().addNamedWidget(widget);
}

void FrameManager::addNamedFont(const std::wstring& name, boost::shared_ptr<Gosu::Font> font) {
	pimpl->namedFonts[name] = font;
}

Gosu::Graphics& FrameManager::graphics() const {
	return *pimpl->graphics;
}

Gosu::Input& FrameManager::input() const {
	return *pimpl->input;
}

void FrameSet::update() {
	root().update();
	// Highlights
	Unit mx = FrameManager::singleton().input().mouseX(), my = FrameManager::singleton().input().mouseY();
	Widget* possibleHL = NULL;
	BOOST_FOREACH(Widget* it, pimpl->widgets) {
		if (!it->highlightable() || !it->visible() || mx < it->dispX() || my < it->dispY() || mx > it->dispX()+it->dispWidth() || my > it->dispY()+it->dispHeight()) continue;
		if (possibleHL == NULL || possibleHL->z() < it->z())
			possibleHL = it;
	}

	if (possibleHL != NULL) {
		// We found one. Is it the old one?
		if (pimpl->highlightedWidget != possibleHL) {
			// No? Tell them he's not focussed anymore. 
			if (pimpl->highlightedWidget != NULL)
				pimpl->highlightedWidget->blur();
			// Set the new focus and focus it.
			pimpl->highlightedWidget = possibleHL;
			pimpl->highlightedWidget->hover();
		}
	}
}

void FrameManager::update() {
	currentSet().update();
}

void FrameSet::draw() {
	root().draw();
}

void FrameManager::draw() {
	currentSet().draw();
}

void FrameSet::buttonUp(Gosu::Button btn) {
	if (pimpl->highlightedWidget)
		pimpl->highlightedWidget->buttonUp(btn);
	if (pimpl->textInputWidget)
		pimpl->textInputWidget->buttonUp(btn);
}

void FrameManager::buttonUp(Gosu::Button btn) {
	currentSet().buttonUp(btn);
}

void FrameSet::buttonDown(Gosu::Button btn) {
	if (pimpl->highlightedWidget)
		pimpl->highlightedWidget->buttonDown(btn);
	if (pimpl->textInputWidget)
		pimpl->textInputWidget->buttonDown(btn);
}

void FrameManager::buttonDown(Gosu::Button btn) {
	currentSet().buttonDown(btn);
}

Widget& FrameSet::namedWidget(const std::wstring& name) const {
	if (pimpl->namedWidgets.find(name) == pimpl->namedWidgets.end())
		throw std::invalid_argument("Widget " + Gosu::narrow(name) + " does not exist");
	return *pimpl->namedWidgets[name];
}

Widget& FrameManager::namedWidget(const std::wstring& name) const {
	return currentSet().namedWidget(name);
}

boost::shared_ptr<Gosu::Font> FrameManager::namedFont(const std::wstring& name) const {
	if (pimpl->namedFonts.find(name) == pimpl->namedFonts.end())
		throw std::invalid_argument("Font " + Gosu::narrow(name) + " does not exist");
	return pimpl->namedFonts[name];
}

void FrameSet::enableTextInput(Widget* inputter) {
	pimpl->textInputWidget = inputter;
}

void FrameManager::enableTextInput(Widget* inputter) {
	currentSet().enableTextInput(inputter);
}

void FrameSet::disableTextInput(Widget* inputter) {
	pimpl->textInputWidget = NULL;
}

void FrameManager::disableTextInput(Widget* inputter) {
	currentSet().disableTextInput(inputter);
}

void FrameSet::activate() {
	// Nothing~
}

void FrameSet::deactivate() {
	pimpl->highlightedWidget = NULL;
	if (pimpl->textInputWidget)
		FrameManager::singleton().disableTextInput(pimpl->textInputWidget);
}

FrameSet& FrameManager::currentSet() const {
	return *pimpl->currentSet;
}

void FrameManager::addSet(FrameSet& set) {
	pimpl->namedSets[set.name()] = &set;
}

std::wstring FrameSet::name() const {
	return pimpl->name;
}

void FrameManager::deleteSet(const std::wstring& name) {
	pimpl->namedSets.erase(pimpl->namedSets.find(name));
}

void FrameManager::changeSet(const std::wstring& newSet) {
	pimpl->currentSet->deactivate();
	pimpl->currentSet = pimpl->namedSets[newSet];
	pimpl->currentSet->activate();
}