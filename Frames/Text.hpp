#ifndef GOSUEX_FRAMES_TEXT_HPP
#define GOSUEX_FRAMES_TEXT_HPP

#include <GosuEx/Frames/Fwd.hpp>
#include <GosuEx/Frames/Frame.hpp>
#include <Gosu/Input.hpp>
#include <Gosu/Text.hpp>
#include <Gosu/Image.hpp>
#include <Gosu/Font.hpp>
#include <Gosu/TextInput.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace GosuEx {
	namespace Frames {

		template<typename T> class BasicText : public T {
			struct Impl {
				boost::shared_ptr<Gosu::Font> font;
				Color textColor;
			} pimpl;
		public:
			BasicText(): T() {}
			BasicText(Unit x, Unit y, Unit z, boost::shared_ptr<Gosu::Font> font, Color color):
				T(x, y, z, 0, 0)
			{
				pimpl.font = font;
				setTextColor(color);
			}

			virtual ~BasicText() {}

			virtual void setText(const std::wstring& newText) = 0;
			virtual std::wstring text() const = 0;

			virtual void setTextColor(Color newColor) { pimpl.textColor = newColor; }
			virtual void setAllTextColors(Color newColor) { setTextColor(newColor); }
			virtual Color textColor() const { return defaultTextColor(); }
			Color defaultTextColor() const { return pimpl.textColor; }
			
			virtual void setFont(boost::shared_ptr<Gosu::Font> newFont) {
				pimpl.font = newFont;
			}

			boost::shared_ptr<Gosu::Font> font() const { 
				return pimpl.font;
			}
		};
		
		template<typename T> class ExtBasicText : public BasicText<T> {
			struct Impl {
				Color disabledTextColor, highlightedTextColor, disabledHighlightTextColor;
			} pimpl;
		public:
			ExtBasicText(Unit x, Unit y, Unit z, boost::shared_ptr<Gosu::Font> font, Color textColor, Color highlightedTextColor = Colors::none, Color disabledTextColor = Colors::none, Color disabledHighlightTextColor = Colors::none):
				BasicText(x, y, z, font, textColor)
			{
				if (disabledTextColor != Colors::none)
					pimpl.disabledTextColor = disabledTextColor;
				else
					pimpl.disabledTextColor = defaultTextColor();
				if (highlightedTextColor != Colors::none)
					pimpl.highlightedTextColor = highlightedTextColor;
				else
					pimpl.highlightedTextColor = defaultTextColor();
				if (disabledHighlightTextColor != Colors::none)
					pimpl.disabledHighlightTextColor = disabledHighlightTextColor;
				else
					pimpl.disabledHighlightTextColor = this->disabledTextColor();
			}

			Color disabledTextColor() const { return pimpl.disabledTextColor; }
			Color highlightedTextColor() const { return pimpl.highlightedTextColor; }
			Color disabledHighlightTextColor() const { return pimpl.disabledHighlightTextColor; }

			void setDisabledTextColor(Color newColor) { pimpl.disabledTextColor = newColor; }
			void setHighlightedTextColor(Color newColor) { pimpl.highlightedTextColor = newColor; }
			void setDisabledHighlightTextColor(Color newColor) { pimpl.disabledHighlightTextColor = newColor; }

			virtual void setAllTextColors(Color newColor) { 
				BasicText::setAllTextColors(newColor);
				setDisabledTextColor(newColor);
				setHighlightedTextColor(newColor);
				setDisabledHighlightTextColor(newColor);
			}

			virtual Color textColor() const {
				if (enabled()) {
					if (highlighted())
						return highlightedTextColor();
					else
						return defaultTextColor();
				}
				else {
					if (highlighted())
						return disabledHighlightTextColor();
					else
						return disabledTextColor();
				}
			}
		};

		template<typename T> class BasicStaticText : public T {
			struct Impl {
				boost::scoped_ptr<Gosu::Image> img;
				std::wstring text;
			} pimpl;
		public:
			BasicStaticText(Unit x, Unit y, Unit z, boost::shared_ptr<Gosu::Font> font, Color color):
				T(x, y, z, font, color)
			{
			}

			virtual void setFont(boost::shared_ptr<Gosu::Font> newFont) {
				T::setFont(newFont);
				reset();
			}

			virtual void setText(const std::wstring& newText) {
				pimpl.text = newText;
				reset();
			}

			virtual std::wstring text() const {
				return pimpl.text;
			}

			virtual void draw() {
				if (!shouldDraw())
					return;
				pimpl.img->draw(dispX(), dispY(), z(), factorX(), factorY(), textColor());
				T::draw();
			}

		protected:
			void reset() {
				Gosu::Bitmap bm;
				if (!text().empty()) {
					bm.resize(Gosu::textWidth(text(), font()->name(), font()->height(), font()->flags()), font()->height());
					Gosu::drawText(bm, text(), 0, 0, Colors::white, font()->name(), font()->height(), font()->flags());
				}
				pimpl.img.reset(new Gosu::Image(FrameManager::singleton().graphics(), bm));
				setWidth(pimpl.img->width());
				setHeight(pimpl.img->height());
			}
		};

		typedef BasicStaticText<BasicText<Widget> > StaticText;
		typedef BasicStaticText<ExtBasicText<Widget> > ExtStaticText;
		//typedef BasicStaticText<BasicText<Frame> > FramedStaticText;
		//typedef BasicStaticText<ExtBasicText<ExtFrame> > ExtFramedStaticText;
		template<typename T> class BasicFontText : public T {
			struct Impl {
				std::wstring text;
			} pimpl;
		public:
			BasicFontText(Unit x, Unit y, Unit z, boost::shared_ptr<Gosu::Font> font, Color color):
				T(x, y, z, font, color)
			{
			}

			virtual void setFont(boost::shared_ptr<Gosu::Font> newFont) {
				T::setFont(newFont);
				reset();
			}

			virtual std::wstring text() const { return pimpl.text; }
			void setText(const std::wstring& newText) { 
				pimpl.text = newText;
				reset();
			}

			void draw() {
				if (!shouldDraw())
					return;
				font()->draw(text(), dispX(), dispY(), z(), factorX(), factorY(), textColor());
			}
		protected:
			void reset() {
				setWidth(Gosu::textWidth(text(), font()->name(), font()->height(), font()->flags()));
				setHeight(font()->height());
			}
		};

		typedef BasicFontText<BasicText<Widget> > FontText;
		typedef BasicFontText<ExtBasicText<Widget> > ExtFontText;
		//typedef BasicFontText<BasicText<Frame> > FramedFontText;
		//typedef BasicFontText<ExtBasicText<ExtFrame> > ExtFramedFontText;

		template<typename T, typename TInput> class BasicTextInput : public T {
			struct Impl {
				TInput input;
				Unit caretWidth;
				Color caretColor, selectionColor;
			} pimpl;
		public:
			BasicTextInput(Unit x, Unit y, Unit z, boost::shared_ptr<Gosu::Font> font, Color color, Color caretColor = Colors::none, Unit caretWidth = 0.0, Color selectionColor = Colors::none):
				T(x, y, z, font, color)
			{
				setCaretColor(caretColor);
				setCaretWidth(caretWidth);
				setSelectionColor(selectionColor);
			}

			virtual ~BasicTextInput() {}

			virtual std::wstring text() const {
				return pimpl.input.text();
			}

			virtual void setText(const std::wstring& newText) {
				pimpl.input.setText(newText);
				T::setText(newText);
			}

			void setCaretColor(Color newColor) { pimpl.caretColor = newColor; }
			void setCaretWidth(Unit newWidth) { pimpl.caretWidth = newWidth; }
			void setSelectionColor(Color newColor) { pimpl.selectionColor = newColor; }

			Unit caretWidth() const { return pimpl.caretWidth; }
			Color caretColor() const { return pimpl.caretColor; }
			Color selectionColor() const { return pimpl.selectionColor; }

			bool enabledTextInput() const { return FrameManager::singleton().input().textInput() == &pimpl.input; }
			
			void enableTextInput() {
				FrameManager::singleton().enableTextInput(this);
				FrameManager::singleton().input().setTextInput(&pimpl.input);
				hover();
			}

			void disableTextInput() {
				FrameManager::singleton().disableTextInput(this);
				FrameManager::singleton().input().setTextInput(NULL);
				blur();
			}

			virtual void draw() {
				if (!shouldDraw())
					return;
				T::draw();
				FrameManager::singleton().graphics().drawQuad(
					dispX()+font()->textWidth(text().substr(0, pimpl.input.selectionStart()), factorX()), dispY(), selectionColor(),
					dispX()+font()->textWidth(text().substr(0, pimpl.input.caretPos()), factorX()), dispY(), selectionColor(),
					dispX()+font()->textWidth(text().substr(0, pimpl.input.caretPos()), factorX()), dispY()+dispHeight(), selectionColor(),
					dispX()+font()->textWidth(text().substr(0, pimpl.input.selectionStart()), factorX()), dispY()+dispHeight(), selectionColor(),
					z()
				);

				std::wstring subtext = text().substr(0, pimpl.input.caretPos());
				Unit cp = dispX() + factorX()*font()->textWidth(text().substr(0, pimpl.input.caretPos()), factorX());
				FrameManager::singleton().graphics().drawQuad(
					cp - caretWidth()/2, dispY(), caretColor(),
					cp + caretWidth()/2, dispY(), caretColor(),
					cp + caretWidth()/2, dispY()+dispHeight(), caretColor(),
					cp - caretWidth()/2, dispY()+dispHeight(), caretColor(),
					z()
				);
			}

			virtual void update() {
				if (enabledTextInput() && clicked(Gosu::msLeft)) {
					double dist = std::numeric_limits<double>::infinity();
					Unit mx = FrameManager::singleton().input().mouseX();
					for (std::size_t i = 0; i < text().length(); i++) {
						Unit z = dispX() + font()->textWidth(text().substr(0, i), factorX());
						if (abs(mx-z) < dist) {
							dist = abs(mx-z);
						} else {
							pimpl.input.setSelectionStart(i-1);
							break;
						}
					}
				}

				T::update();
			}

			virtual void buttonDown(Gosu::Button btn) {
				if (enabledTextInput() && btn == Gosu::msLeft) {
					double dist = std::numeric_limits<double>::infinity();
					Unit mx = FrameManager::singleton().input().mouseX();
					for (std::size_t i = 0; i < text().length(); i++) {
						Unit z = dispX() + font()->textWidth(text().substr(0, i), factorX());
						if (abs(mx-z) < dist) {
							dist = abs(mx-z);
						} else {
							pimpl.input.setCaretPos(i-1);
							if (!FrameManager::singleton().input().down(Gosu::kbLeftShift) && !FrameManager::singleton().input().down(Gosu::kbRightShift))
								pimpl.input.setSelectionStart(i-1);
							break;
						}
					}
					// # set caret pos
				}
				T::buttonDown(btn);
			}
		};

		//typedef BasicTextInput<StaticText, Gosu::TextInput> StaticTextInput;
		typedef BasicTextInput<FontText, Gosu::TextInput> FontTextInput;
	}
}
#endif