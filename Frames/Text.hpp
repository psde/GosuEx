#ifndef GOSUEX_FRAMES_TEXT_HPP
#define GOSUEX_FRAMES_TEXT_HPP

#include <GosuEx/Frames/Fwd.hpp>
#include <GosuEx/Frames/Frame.hpp>
#include <Gosu/Input.hpp>
#include <Gosu/Text.hpp>
#include <Gosu/Image.hpp>
#include <Gosu/Font.hpp>
#include <Gosu/TextInput.hpp>

namespace GosuEx {
	namespace Frames {

		typedef Gosu::TextAlign TextAlign;

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
				BasicText<T>(x, y, z, font, textColor)
			{
				if (disabledTextColor != Colors::none)
					pimpl.disabledTextColor = disabledTextColor;
				else
					pimpl.disabledTextColor = this->defaultTextColor();
				if (highlightedTextColor != Colors::none)
					pimpl.highlightedTextColor = highlightedTextColor;
				else
					pimpl.highlightedTextColor = this->defaultTextColor();
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
				BasicText<T>::setAllTextColors(newColor);
				this->setDisabledTextColor(newColor);
				this->setHighlightedTextColor(newColor);
				this->setDisabledHighlightTextColor(newColor);
			}

			virtual Color textColor() const {
				if (this->enabled()) {
					if (this->highlighted())
						return this->highlightedTextColor();
					else
						return this->defaultTextColor();
				}
				else {
					if (this->highlighted())
						return this->disabledHighlightTextColor();
					else
						return this->disabledTextColor();
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

			virtual ~BasicStaticText() {
				pimpl.img.reset();
			}

			virtual void setFont(boost::shared_ptr<Gosu::Font> newFont) {
				T::setFont(newFont);
				this->reset();
			}

			virtual void setText(const std::wstring& newText) {
				pimpl.text = newText;
				this->reset();
			}

			virtual std::wstring text() const {
				return pimpl.text;
			}

			virtual void draw() {
				if (!this->shouldDraw())
					return;
				pimpl.img->draw(this->dispX(), this->dispY(), z(), this->factorX(), this->factorY(), this->textColor());
				T::draw();
			}

			const Gosu::Image* textImage() { return pimpl.img.get(); }

		protected:
			void setTextImage(Gosu::Image* img) {
				pimpl.img.reset(img);
			}

			virtual void reset() {
				Gosu::Bitmap bm;
				if (!this->text().empty()) {
					bm.resize(Gosu::textWidth(this->text(), this->font()->name(), this->font()->height(), this->font()->flags()), this->font()->height());
					Gosu::drawText(bm, this->text(), 0, 0, Colors::white, this->font()->name(), this->font()->height(), this->font()->flags());
				}
				this->setTextImage(new Gosu::Image(FrameManager::singleton().graphics(), bm));
				this->setWidth(pimpl.img->width());
				this->setHeight(pimpl.img->height());
			}
		};
		
		/**
		* It's necessary to set maxwidth (constructor or later)
		* otherwise, it'll be crippled.
		* But, you'll see for yourself.
		*/
		template<typename T> class ExtBasicStaticText : public BasicStaticText<T> {
			struct Impl {
				unsigned int lineSpacing;
				Unit maxWidth;
				TextAlign align;
			} pimpl;
		public:
			ExtBasicStaticText(Unit x, Unit y, Unit z, boost::shared_ptr<Gosu::Font> font, Color color, unsigned int lineSpacing = 0, Unit maxWidth = 42, TextAlign align = Gosu::taLeft):
				BasicStaticText<T>(x, y, z, font, color)
			{
				pimpl.lineSpacing = lineSpacing;
				pimpl.align = align;
				pimpl.maxWidth = maxWidth;
			}

				void setLineSpacing(unsigned int newSpacing) { pimpl.lineSpacing = newSpacing; reset(); }
				unsigned int lineSpacing() const { return pimpl.lineSpacing; }

				void setTextAlign(TextAlign newAlign) { pimpl.align = newAlign; reset(); }
				void textAlign() const { return pimpl.align; }

				void setMaxWidth(Unit w) { pimpl.maxWidth = w; reset(); }
				Unit maxWidth() const { return pimpl.maxWidth; }

		protected:			
			virtual void reset() {
				this->setTextImage(new Gosu::Image(FrameManager::singleton().graphics(), Gosu::createText(this->text(), this->font()->name(), this->font()->height(), pimpl.lineSpacing, static_cast<unsigned int>(pimpl.maxWidth), pimpl.align, this->font()->flags())));
				this->setWidth(this->textImage()->width());
				this->setHeight(this->textImage()->height());
			}
		};

		typedef BasicStaticText<BasicText<Widget> > StaticText;
		typedef BasicStaticText<ExtBasicText<Widget> > ExtStaticText;
		typedef ExtBasicStaticText<ExtBasicText<Widget> > ExtExtStaticText;
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
				this->reset();
			}

			virtual std::wstring text() const { return pimpl.text; }
			void setText(const std::wstring& newText) { 
				pimpl.text = newText;
				this->reset();
			}

			void draw() {
				if (!this->shouldDraw())
					return;
				this->font()->draw(this->text(), this->dispX(), this->dispY(), this->z(), this->factorX(), this->factorY(), this->textColor());
			}
		protected:
			void reset() {
				this->setWidth(Gosu::textWidth(this->text(), this->font()->name(), this->font()->height(), this->font()->flags()));
				this->setHeight(this->font()->height());
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
				this->setCaretColor(caretColor);
				this->setCaretWidth(caretWidth);
				this->setSelectionColor(selectionColor);
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
				this->hover();
			}

			void disableTextInput() {
				FrameManager::singleton().disableTextInput(this);
				FrameManager::singleton().input().setTextInput(NULL);
				this->blur();
			}

			virtual void draw() {
				if (!this->shouldDraw())
					return;
				T::draw();

				if (!this->enabledTextInput())
					return;
				FrameManager::singleton().graphics().drawQuad(
					this->dispX()+this->font()->textWidth(this->text().substr(0, pimpl.input.selectionStart()), this->factorX()), this->dispY(), this->selectionColor(),
					this->dispX()+this->font()->textWidth(this->text().substr(0, pimpl.input.caretPos()), this->factorX()), this->dispY(), this->selectionColor(),
					this->dispX()+this->font()->textWidth(this->text().substr(0, pimpl.input.caretPos()), this->factorX()), this->dispY()+this->dispHeight(), this->selectionColor(),
					this->dispX()+this->font()->textWidth(this->text().substr(0, pimpl.input.selectionStart()), this->factorX()), this->dispY()+this->dispHeight(), this->selectionColor(),
					this->z()
				);

				std::wstring subtext = this->text().substr(0, pimpl.input.caretPos());
				Unit cp = this->dispX() + this->factorX()*this->font()->textWidth(this->text().substr(0, pimpl.input.caretPos()), this->factorX());
				FrameManager::singleton().graphics().drawQuad(
					cp - caretWidth()/2, this->dispY(), caretColor(),
					cp + caretWidth()/2, this->dispY(), caretColor(),
					cp + caretWidth()/2, this->dispY()+this->dispHeight(), caretColor(),
					cp - caretWidth()/2, this->dispY()+this->dispHeight(), caretColor(),
					this->z()
				);
			}

			virtual void update() {
				if (enabledTextInput() && this->clicked(Gosu::msLeft)) {
					double dist = std::numeric_limits<double>::infinity();
					Unit mx = FrameManager::singleton().input().mouseX();
					for (std::size_t i = 0; i < this->text().length(); i++) {
						Unit z = this->dispX() + this->font()->textWidth(this->text().substr(0, i), this->factorX());
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
						Unit z = this->dispX() + this->font()->textWidth(this->text().substr(0, i), this->factorX());
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