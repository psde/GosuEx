#ifndef GOSUEX_WIDGETS_BUTTON_HPP
#define GOSUEX_WIDGETS_BUTTON_HPP

#include <GosuEx/Widgets/Fwd.hpp>
#include <GosuEx/Widgets/Widget.hpp>

namespace GosuEx {
		namespace Widgets {
				class Button : public Widget {
						struct Impl;
						boost::scoped_ptr<Impl> pimpl;
				public:
						Button(WidgetP normal, WidgetP aimed = WidgetP(), WidgetP disabled = WidgetP());
						virtual ~Button();

						// Draw different.
						virtual void draw();
						
						// DO IT DIFFERENT
						virtual void setPosition(double newX, double newY, double newZ);

						// Get the subwidgets
						WidgetP normalWidget() const;
						WidgetP aimedWidget() const;
						WidgetP disabledWidget() const;

						// Tell it the "children"
						virtual void setGosuGraphics(Gosu::Graphics* graphics);
						virtual void setGosuInput(Gosu::Input* input);
				protected:
						// Reset the position of the widgets.
						void reset();
				};
		}
}
#endif