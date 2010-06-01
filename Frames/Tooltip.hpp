#ifndef GOSUEX_FRAMES_TOOLTIP_HPP
#define GOSUEX_FRAMES_TOOLTIP_HPP
#include <GosuEx/Frames/Fwd.hpp>
#include <GosuEx/Frames/Frame.hpp>

/**
* Tooltips always follow their attached widget.
* It will, per default, try to stay in the middle down. 
* You can, however, manipulate this with offsetX, offsetY.
* You should not rel these... It's just a simple tooltip.
* 2do: Extend it.
*/

namespace GosuEx {
	namespace Frames {
		template<typename T> class Tooltip : public T {
			struct Impl {
				Unit offsetX, offsetY;
			} pimpl;
		public:
			Tooltip(Unit x, Unit y, Unit z):
				T(x, y, z)
			{
				pimpl.offsetX = pimpl.offsetY = 0.0;
			}

			Tooltip(Unit x, Unit y, Unit z, boost::shared_ptr<Gosu::Image> image, Color color = Colors::white):
				T(x, y, z, image, color)
			{
				pimpl.offsetX = pimpl.offsetY = 0.0;
			}

			Tooltip(Unit x, Unit y, Unit z, boost::shared_ptr<Gosu::Font> font, Color color = Colors::white):
				T(x, y, z, font, color)
			{
				pimpl.offsetX = pimpl.offsetY = 0.0;
			}

			Tooltip(Unit x, Unit y, Unit z, Unit width, Unit height, boost::shared_ptr<Gosu::Font> font, const std::wstring& text, Color textColor):
				T(x, y, z, width, height, font, text, textColor)
			{
				pimpl.offsetX = pimpl.offsetY = 0.0;
			}

			virtual void update() {
				if (!visible())
					return;
				setX(max(0.0, parent()->dispX()+parent()->dispWidth()/2.0 - dispWidth()/2 + offsetX()));
				setY(max(0.0, parent()->dispY()+parent()->dispHeight() - dispHeight()/2 + offsetY()));
				if (dispX()+dispWidth() > FrameManager::singleton().graphics().width()) {
					setX(FrameManager::singleton().graphics().width()-dispWidth());
				}
				if (dispY()+dispHeight() > FrameManager::singleton().graphics().height()) {
					setY(FrameManager::singleton().graphics().height()-dispHeight());
				}
				T::update();
			}

			Unit offsetX() const {
				return pimpl.offsetX;
			}

			Unit offsetY() const {
				return pimpl.offsetY;
			}

			void setOffsetX(Unit offsetX) {
				pimpl.offsetX = offsetX;
			}

			void setOffsetY(Unit offsetY) {
				pimpl.offsetY = offsetY;
			}

			virtual void setRelX(double _d) {}
			virtual void setRelY(double _d) {}
		};
	}
}
#endif