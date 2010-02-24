#ifndef GOSUEX_WIDGETS_EFFECTS_FADECOLOR_HPP
#define GOSUEX_WIDGETS_EFFECTS_FADECOLOR_HPP
/**
 * Fades between the original color and a defined.
*/

#include <GosuEx/Widgets/Fwd.hpp>
#include <GosuEx/Widgets/Effects/Effect.hpp>

namespace GosuEx {
		namespace Widgets {
				namespace Effects {

						//FadeColorP createFadeColorEffect(WidgetP widget, unsigned long lifetime, Gosu::Color endColor);
						//FadeColorP createFadeColorEffect(EffectP effect, unsigned long lifetime, Gosu::Color endColor);

						class FadeColor : public Effect {
								struct Impl;
								boost::scoped_ptr<Impl> pimpl;
						public:
								FadeColor(unsigned long lifetime, Gosu::Color end);
								virtual ~FadeColor();


								// Sets a (new) end color.
								// NOTE: If you call this and the effect is running, the 
								// start color will be reset!
								void setEndColor(Gosu::Color color);

								virtual void start(bool children = false);
								virtual bool draw();
						};
				}
		}
}
#endif