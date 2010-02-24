#ifndef GOSUEX_WIDGETS_FONTTEXTBUTTON_HPP
#define GOSUEX_WIDGETS_FONTTEXTBUTTON_HPP

#include <GosuEx/Widgets/Fwd.hpp>
#include <GosuEx/Widgets/Button.hpp>
#include <GosuEx/Widgets/FontText.hpp>

namespace GosuEx {
		namespace Widgets {
				class FontTextButton : public Button {
						struct Impl;
						boost::scoped_ptr<Impl> pimpl;
				public:
						FontTextButton(FontTextP text, WidgetP normal, WidgetP aimed = WidgetP(), WidgetP disabled = WidgetP());
						virtual ~FontTextButton();

						// Draw even more different
						virtual void draw();
						virtual void setPosition(double newX, double newY, double newZ);

						// Tell it the "children"
						virtual void setGosuGraphics(Gosu::Graphics* graphics);
						virtual void setGosuInput(Gosu::Input* input);
				protected:
						// Reset position of the widgets.
						void reset();
				};
		}
}
#endif