#ifndef GOSUEX_FRAMES_BUTTON_HPP
#define GOSUEX_FRAMES_BUTTON_HPP

#include <GosuEx/Frames/Fwd.hpp>
#include <GosuEx/Frames/Widget.hpp>
#include <GosuEx/Frames/Frame.hpp>
#include <GosuEx/Frames/Text.hpp>
#include <GosuEx/Frames/Image.hpp>
#include <GosuEx/Frames/FrameManager.hpp>

/**
* Don't forget to reset() after doing things with the textWidget
* Also, textWidget is always z+1. Of course, you can set it afterwards to whatever you like. That's just the default.
*/

namespace GosuEx {
	namespace Frames {
		template<typename TBack, typename TText> class BasicButton : public TBack {
			struct Impl {
				TText* textWidget;
			} pimpl;
		public:
			BasicButton(Unit x, Unit y, Unit z, boost::shared_ptr<Gosu::Image> image, boost::shared_ptr<Gosu::Font> font, const std::wstring& text, Color textColor, Color backgroundColor = Colors::white):
				TBack(x, y, z, image, backgroundColor)
			{
				createChild(pimpl.textWidget = new TText(x, y, z+1, font, textColor));
				textWidget().setText(text);
				textWidget().setHighlightable(false);
				textWidget().setRelX(0.5);
				textWidget().setRelY(0.5);
				reset();
			}

			BasicButton(Unit x, Unit y, Unit z, Unit width, Unit height, boost::shared_ptr<Gosu::Font> font, const std::wstring& text, Color textColor, Color backgroundColor = Colors::none, Color borderColor = Colors::none, Unit borderWidth = 0.0):
				TBack(x, y, z, width, height, backgroundColor, borderColor, borderWidth)
			{
				createChild(pimpl.textWidget = new TText(x, y, z+1, font, textColor));
				textWidget().setText(text);
				textWidget().setHighlightable(false);
				textWidget().setRelX(0.5);
				textWidget().setRelY(0.5);
				reset();
			}

			virtual void update() {
				pimpl.textWidget->setX(dispX()+dispWidth()/2.0);
				pimpl.textWidget->setY(dispY()+dispHeight()/2.0);
				TBack::update();
			}


			virtual void blur() {
				textWidget().blur();
				TBack::blur();
			}

			virtual void hover() {
				textWidget().hover();
				TBack::hover();
			}

			virtual void buttonDown(Gosu::Button btn) {
				textWidget().buttonDown(btn);
				TBack::buttonDown(btn);
			}

			virtual void buttonUp(Gosu::Button btn) {
				textWidget().buttonUp(btn);
				TBack::buttonUp(btn);
			}

			TText& textWidget() const { return *pimpl.textWidget; }

			virtual void reset() {
				setWidth(pimpl.textWidget->dispWidth());
				setHeight(pimpl.textWidget->dispHeight());
			}

		};
		typedef BasicButton<Frame, StaticText> FramedStaticButton;
		typedef BasicButton<ExtFrame, ExtStaticText> ExtFramedStaticButton;
		typedef BasicButton<ExtFrame, ExtExtStaticText> ExtExtFramedStaticButton;
		typedef BasicButton<Image, StaticText> ImageStaticButton;
		typedef BasicButton<ExtImage, ExtStaticText> ExtImageStaticButton;
	}
}
#endif