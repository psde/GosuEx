#ifndef GOSUEX_WIDGETS_IMAGE_HPP
#define GOSUEX_WIDGETS_IMAGE_HPP

/**
 * A widget that holds an image.
	* based on Widgets::Colorable, it supports relXY, factorXY and different colors.
	* You have to pass a boost::shared_ptr<Gosu::Image> (GosuEx::Image) to it.
	* You can use the loadImage-function for that.
	*/

#include <GosuEx/Widgets/Fwd.hpp>
#include <GosuEx/Widgets/Colorable.hpp>
#include <Gosu/Image.hpp>
#include <Gosu/Directories.hpp>

namespace GosuEx {
		typedef boost::shared_ptr<Gosu::Image> GosuImageP;

		//! If you specify the 3 arguments, it will be relative to the current directory.
		//! As filename you could say "media/star.png" for example.
		inline GosuImageP loadImage(Gosu::Graphics& graphics, const std::wstring& filename, bool tileable = false) {
				GosuImageP ptr(new Gosu::Image(graphics, Gosu::resourcePrefix()+filename, tileable));
				return ptr;
		}

		//! If you give it a Image* it won't do that automagic.
		inline GosuImageP loadImage(Gosu::Image* image) {
				GosuImageP ptr(image);
				return ptr;
		}

		namespace Widgets {
				class Image : public Colorable {
						struct Impl;
						boost::scoped_ptr<Impl> pimpl;
				public:
						/// CTor, DTor, E.T.
						Image(double x, double y, double z, GosuImageP image);
						virtual ~Image();

						/// "get"Foo
						GosuImageP image();
						
						/// setFoo
						void setImage(GosuImageP newImage);
						
						/// foo
						virtual void draw();
				protected:
						virtual void reset();
				};
		}
}
#endif