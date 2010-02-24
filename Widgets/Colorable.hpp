#ifndef GOSUEX_WIDGETS_COLORABLE_HPP
#define GOSUEX_WIDGETS_COLORABLE_HPP

/**
 * A colorable Widget is, well. The same like its parent, the normal widget.
	* The difference is that it can have a color and a alpha mode.
	* If you do use this class (and not an inheritance of it) and gosuGraphics()
	* is != NULL, it will draw a quad, based on x, y, width and height in the color
	* and the alphamode specified.
	* By default, it has no color.
	*/

#include <GosuEx/Widgets/Fwd.hpp>
#include <GosuEx/Widgets/Widget.hpp>
#include <Gosu/Color.hpp>
#include <Gosu/GraphicsBase.hpp>

namespace GosuEx {
		namespace Widgets {
				class Colorable : public Widget {
						struct Impl;
						boost::scoped_ptr<Impl> pimpl;
				public:
						// Constructor, Destructor, COOKIES
						Colorable();
						Colorable(double x, double y, double z, double width, double height);
						virtual ~Colorable();

						// foo
						virtual void draw();

						// "get"Foo
						Gosu::Color color() const;
						Gosu::AlphaMode alphaMode() const;
						double width() const;
						double height() const;

						// setFoo
						void setColor(Gosu::Color newColor);
						void setAlphaMode(Gosu::AlphaMode newAlphaMode);
				};
		}
}
#endif