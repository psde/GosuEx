#ifndef GOSUEX_WIDGETS_CURSOR_HPP
#define GOSUEX_WIDGETS_CURSOR_HPP

/**
 * A simple, automagically placed cursor.
	* Is always disabled. Won't trigger anything.
	* Can't have X/Y. Only Z.
	* Setting X or Y will have no effect.
	* Of course you can, if you're a funny person, set relX/relY
	* and with cosinus/sinus you can make the cursor moving in circles
	* whooho, you're cool heh.
*/

#include <GosuEx/Widgets/Fwd.hpp>
#include <GosuEx/Widgets/Widget.hpp>
#include <GosuEx/Widgets/Image.hpp>

namespace GosuEx {
		namespace Widgets {
				class Cursor : public Image {
				public:
						Cursor(double z, GosuImageP image);
						virtual ~Cursor();

						virtual void update();

						// Always false -> always ignored.
						virtual void setEnabled(bool enabled);
				};
		}
}
#endif