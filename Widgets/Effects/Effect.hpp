#ifndef GOSUEX_WIDGETS_EFFECTS_EFFECT_HPP
#define GOSUEX_WIDGETS_EFFECTS_EFFECT_HPP

/**
 * You want to do make more of your widget?
	* Effects are the best way to do it.
	* There will be some example effects which you can use
	* Creating own effects will be as easy as a widget.
	* You can construct an effect with the createEffect-function.
	* Either the first parameter is the parent widget or the parent effect.
	* Effects can die. If they do, they will fire the onDeath-Event.
	* Effects won't start automagically. You have to start() them.
	* Death events can live their second summer with start().
	* Their timer will be reset.
	* If you want to stop them, well, stop(). Won't fire the event.
	* 
	* It's really important that you use the createEffect since there's pointer magic.
*/

#include <GosuEx/Widgets/Fwd.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/function.hpp>
#include <Gosu/Input.hpp>

namespace GosuEx {
		namespace Widgets {
				namespace Effects {
						//EffectP createEffect(EffectP effect, unsigned long lifetime);
						//EffectP createEffect(WidgetP widget, unsigned long lifetime);

						class Effect {
								struct Impl;
								boost::scoped_ptr<Impl> pimpl;
						public:
								Effect(unsigned long lifetime);
								virtual ~Effect();
								
								// Tells this effect and all children that
								// the new widget (that should be modified)
								// is now widget.
								void attachTo(WidgetP widget);
								void attachTo(WidgetWP widget);

								// Called with Widget::update
								virtual bool update();
								// Called with Widget::draw
								virtual bool draw();
								// Called with Widget::buttonUp
								virtual bool buttonDown(Gosu::Button btn);
								// Called with Widget::buttonDown
								virtual bool buttonUp(Gosu::Button btn);
								
								// Sets the pointer.
								void setPtr(EffectP ptr);
								// cleanUp
								void cleanUp();

								// Starts this effect. Won't have an effect on effects...
								// - without an widget (!widget())
								// - with lifetime ==  0
								// - which are running (running())
								// Anyway, the call will be directed to the children if children == true
								// If the parent has no widget, the children won't have one -> nothing happens.
								// most likely. I hope so.
								virtual void start(bool children = false);
								// Stops this widget and, if children == true, all children.
								virtual void stop(bool children = true);
								// Adds an effect as child. Will set the widget which the parent is attached to
								// if the parent has one.
								void addEffect(EffectP effect);

								// Is this effect running now?
								bool running() const;
								// Widget this effect is attached to
								WidgetP widget();
								// When the effect was started (milliseconds)
								unsigned long startTime() const;
								// How long it lives (milliseconds)
								unsigned long lifetime() const;
								
								// Pointer to this effect
								EffectP ptr();
								// Same like ptr()
								EffectP operator&();

								// Will be fired as soon as its lifetime reached zero (startTime()+lifeTime() < Gosu::milliseconds())
								// You can bind anything to this, for example, start another effect. Or exit the game.
								boost::function<void(EffectP)> onDeath;
						};

						// Create-a-Sim^WEffect, with attaching to an widget.
						template<class T> boost::shared_ptr<T> createEffect(WidgetP attachTo, T* effect) {
								boost::shared_ptr<T> ptr(effect);
								effect->setPtr(ptr);
								attachTo->attachEffect(ptr);
								return ptr;
						}

						// Create-A-Pet^WEffect, with attaching to an other effect.
						template<class T> boost::shared_ptr<T> createEffect(EffectP attachTo, T* effect) {
								boost::shared_ptr<T> ptr(effect);
								effect->setPtr(ptr);
								attachTo->addEffect(ptr);
								return ptr;
						}
				}
		}
}
#endif