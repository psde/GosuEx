#ifndef GOSUEX_FRAMES_BUTTON_HPP
#define GOSUEX_FRAMES_BUTTON_HPP

#include <GosuEx/Frames/Fwd.hpp>
#include <GosuEx/Frames/Widget.hpp>
#include <GosuEx/Frames/Frame.hpp>
#include <GosuEx/Frames/Text.hpp>
#include <GosuEx/Frames/Image.hpp>

namespace GosuEx {
	namespace Frames {
		template<typename TBack, typename TText> class BasicButton : public TBack {
			struct Impl {
				TText* textWidget;
			} pimpl;
		public:
			BasicButton(Unit x, Unit y, Unit z, Unit width, Unit height, boost::shared_ptr<Gosu::Font> font, const std::wstring& text, Color textColor, Color backgroundColor = Colors::none, Color borderColor = Colors::none, Unit borderWidth = 0.0):
				TBack(x, y, z, width, height, backgroundColor, borderColor, borderWidth)
			{
				createChild(pimpl.textWidget = new TText(x, y, z+1, font, textColor));
				pimpl.textWidget->setText(text);
				reset();
			}

			virtual void reset() {
				pimpl.textWidget->setX(dispX()+dispWidth()/2.0);
				pimpl.textWidget->setY(dispY()+dispHeight()/2.0);
				pimpl.textWidget->setRelX(0.5);
				pimpl.textWidget->setRelY(0.5);
			}
		};

		typedef BasicButton<Frame, StaticText> FramedStaticButton;
		typedef BasicButton<Image, StaticText> ImageStaticButton;
		typedef BasicButton<ExtImage, ExtStaticText> ExtImageStaticButton;
	}
}
#endif