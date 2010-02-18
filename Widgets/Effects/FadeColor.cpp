#include "FadeColor.hpp"
#include "../Colorable.hpp"
#include <Gosu/Timing.hpp>

namespace WGT = GosuEx::Widgets;
namespace EFT = GosuEx::Widgets::Effects;

/*EFT::FadeColorP EFT::createFadeColorEffect(WGT::WidgetP widget, unsigned long lifetime, Gosu::Color endColor) {
		EFT::FadeColorP ptr(new FadeColor(widget, lifetime, endColor));
		widget->attachEffect(ptr);
		return ptr;
}

EFT::FadeColorP EFT::createFadeColorEffect(EffectP effect, unsigned long lifetime, Gosu::Color endColor) {
		EFT::FadeColorP ptr(new FadeColor(effect->widget(), lifetime, endColor));
		effect->addEffect(ptr);
		return ptr;
}*/

struct EFT::FadeColor::Impl {
		Gosu::Color startColor, endColor;
};

EFT::FadeColor::FadeColor(unsigned long lifetime, Gosu::Color end):
pimpl(new Impl), Effect(lifetime) {
		pimpl->endColor = end;
}

EFT::FadeColor::~FadeColor() {}

void EFT::FadeColor::setEndColor(Gosu::Color color) {
		pimpl->endColor = color;
		if (running()) pimpl->startColor = widget()->cptr<WGT::Colorable>()->color();
}

void EFT::FadeColor::start(bool children) {
		if (!widget()) return;
		pimpl->startColor = widget()->cptr<WGT::Colorable>()->color();
		Effect::start(children);
}

bool EFT::FadeColor::draw() {
		if (!Effect::draw()) 
				return false;
		double weight = static_cast<double>(Gosu::milliseconds()-startTime())/(lifetime());
		widget()->cptr<WGT::Colorable>()->setColor(Gosu::interpolate(pimpl->startColor, pimpl->endColor, weight));
		return true;
}