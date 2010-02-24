#include "Effect.hpp"
#include "../Widget.hpp"
#include <Gosu/Timing.hpp>
#include <deque>

namespace WGT = GosuEx::Widgets;
namespace EFT = GosuEx::Widgets::Effects;

/*EFT::EffectP EFT::createEffect(WGT::WidgetP widget, unsigned long lifetime) {
		EffectP ptr(new Effect(widget, lifetime));
		return ptr;
}

EFT::EffectP EFT::createEffect(EffectP effect, unsigned long lifetime) {
		EffectP ptr(new Effect(effect->widget(), lifetime));
		effect->addEffect(ptr);
		return ptr;
}*/

struct EFT::Effect::Impl {
		unsigned long start, lifetime;
		bool running;
		WGT::WidgetWP attachedTo;
		EffectWP ptr;
		std::deque<EffectP> children;
};


EFT::Effect::Effect(unsigned long lifetime):
pimpl(new Impl) {
		pimpl->lifetime = lifetime;
		pimpl->running = false;
		pimpl->start = 0;
}

EFT::Effect::~Effect() {}

void EFT::Effect::attachTo(WGT::WidgetP widget) {
		WidgetWP p(widget);
		attachTo(p);
}

void EFT::Effect::attachTo(WGT::WidgetWP widget) {
		for (std::deque<EffectP>::iterator it = pimpl->children.begin(); it != pimpl->children.end(); ++it)
				(**it).attachTo(widget);
		pimpl->attachedTo = widget;
}

bool EFT::Effect::update() {
		for (std::deque<EffectP>::iterator it = pimpl->children.begin(); it != pimpl->children.end(); ++it)
				(**it).update();
		if (!running() || !widget()) return false;
		if (startTime()+lifetime() < Gosu::milliseconds()) {
				if (onDeath)
						onDeath(ptr());
				stop(false);
				return true;
		}
		return false;
}

bool EFT::Effect::draw() {
		for (std::deque<EffectP>::iterator it = pimpl->children.begin(); it != pimpl->children.end(); ++it)
				(**it).draw();
		if (!running() || !widget()) return false;
		return true;
}

bool EFT::Effect::buttonDown(Gosu::Button btn) {
		for (std::deque<EffectP>::iterator it = pimpl->children.begin(); it != pimpl->children.end(); ++it)
				(**it).buttonDown(btn);
		if (!running() || !widget()) return false;
		return true;
}

bool EFT::Effect::buttonUp(Gosu::Button btn) {
		for (std::deque<EffectP>::iterator it = pimpl->children.begin(); it != pimpl->children.end(); ++it)
				(**it).buttonUp(btn);
		if (!running() || !widget()) return false;
		return true;
}

void EFT::Effect::start(bool children) {
		if (children) {
				for (std::deque<EffectP>::iterator it = pimpl->children.begin(); it != pimpl->children.end(); ++it)
						(**it).start(true);
		}
		if (running() || !lifetime() || !widget()) return;
		pimpl->running = true;
		pimpl->start = Gosu::milliseconds();
}

void EFT::Effect::stop(bool children) {
		if (children) {
				for (std::deque<EffectP>::iterator it = pimpl->children.begin(); it != pimpl->children.end(); ++it)
						(**it).stop(true);
		}
		pimpl->running = false;
}

void EFT::Effect::addEffect(EffectP effect) {
		pimpl->children.push_back(effect);
		if (widget())
				effect->attachTo(widget());
}

bool EFT::Effect::running() const { return pimpl->running; }
WGT::WidgetP EFT::Effect::widget() { 
		WidgetP p = pimpl->attachedTo.lock();
		return p;
}
unsigned long EFT::Effect::startTime() const { return pimpl->start; }
unsigned long EFT::Effect::lifetime() const { return pimpl->lifetime; }

void EFT::Effect::setPtr(EffectP nptr) {
		if (ptr()) return;
		pimpl->ptr = nptr;
}

void EFT::Effect::cleanUp() {
		for (std::deque<EffectP>::iterator it = pimpl->children.begin(); it != pimpl->children.end(); ++it)
				(**it).cleanUp();
		onDeath.clear();
}

EFT::EffectP EFT::Effect::ptr() {
		EffectP ptr;
		ptr = pimpl->ptr.lock();
		return ptr;
}

EFT::EffectP EFT::Effect::operator &() { return ptr(); }