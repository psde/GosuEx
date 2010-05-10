#ifndef GOSUEX_FRAMES_FRAME_HPP
#define GOSUEX_FRAMES_FRAME_HPP

/**
 * Basic Frame.
*/

#include <GosuEx/Frames/Widget.hpp>
#include <GosuEx/Frames/FrameManager.hpp>
#include <Gosu/Graphics.hpp>

namespace GosuEx {
	namespace Frames {
		
		// Easy Frame. Is Easy.
		class Frame : public Widget {
			struct Impl {
				Color background, border;
				Unit borderWidth, borderFactor;
			} pimpl;
		public:
			// Empty constructor for cursors.
			Frame(): Widget() {
				pimpl.background = pimpl.border = Colors::none;
				pimpl.borderWidth = 0;
				pimpl.borderFactor = 1.0;
			}

			Frame(Unit x, Unit y, Unit z, Unit width, Unit height, Color backgroundColor = Colors::none, Color borderColor = Colors::none, Unit borderWidth = 0):
				Widget(x, y, z, width, height)
			{
				setBackgroundColor(backgroundColor);
				setBorderColor(borderColor);
				pimpl.borderWidth = borderWidth;
				pimpl.borderFactor = 1.0;
			}

			virtual ~Frame() {}

			virtual Unit dispX() const { return Widget::dispX() - dispBorderWidth(); }
			virtual Unit dispY() const { return Widget::dispY() - dispBorderWidth(); }
			virtual Unit dispWidth() const { return Widget::dispWidth() + 2*dispBorderWidth(); }
			virtual Unit dispHeight() const { return Widget::dispHeight() + 2*dispBorderWidth(); }

			virtual void draw() {
				if (!shouldDraw())
					return;
				// Draw background
				FrameManager::singleton().graphics().drawQuad(
						dispX(), dispY(), backgroundColor(),
						dispX()+dispWidth(), dispY(), backgroundColor(),
						dispX()+dispWidth(), dispY()+dispHeight(), backgroundColor(),
						dispX(), dispY()+dispHeight(), backgroundColor(),
						z());
				// Top
				FrameManager::singleton().graphics().drawQuad(
					dispX()-dispBorderWidth(), dispY()-dispBorderWidth(), borderColor(),
					dispX()+dispWidth()+dispBorderWidth(), dispY()-dispBorderWidth(), borderColor(),
					dispX()+dispWidth()+dispBorderWidth(), dispY(), borderColor(),
					dispX()-dispBorderWidth(), dispY(), borderColor(),
					z());
				// Right
				FrameManager::singleton().graphics().drawQuad(
					dispX()+dispWidth(), dispY()-dispBorderWidth(), borderColor(),
					dispX()+dispWidth()+dispBorderWidth(), dispY()-dispBorderWidth(), borderColor(),
					dispX()+dispWidth()+dispBorderWidth(), dispY()+dispHeight()+dispBorderWidth(), borderColor(),
					dispX()+dispWidth(), dispY()+dispHeight()+dispBorderWidth(), borderColor(),
					z());
				// Bottom
				FrameManager::singleton().graphics().drawQuad(
					dispX()-dispBorderWidth(), dispY()+dispHeight(), borderColor(),
					dispX()+dispWidth()+dispBorderWidth(), dispY()+dispHeight(), borderColor(),
					dispX()+dispWidth()+dispBorderWidth(), dispY()+dispHeight()+dispBorderWidth(), borderColor(),
					dispX()-dispBorderWidth(), dispY()+dispHeight()+dispBorderWidth(), borderColor(),
					z());
				// Left
				FrameManager::singleton().graphics().drawQuad(
					dispX()-dispBorderWidth(), dispY()-dispBorderWidth(), borderColor(),
					dispX(), dispY()-dispBorderWidth(), borderColor(),
					dispX(), dispY()+dispHeight()+dispBorderWidth(), borderColor(),
					dispX()-dispBorderWidth(), dispY()+dispHeight()+dispBorderWidth(), borderColor(),
					z());
				Widget::draw();
			}

			virtual Color backgroundColor() const { return defaultBackgroundColor(); }
			virtual Color borderColor() const { return defaultBorderColor(); }
			Color defaultBackgroundColor() const { return pimpl.background; }
			Color defaultBorderColor() const { return pimpl.border; }

			Unit borderWidth() const { return pimpl.borderWidth; }
			Unit dispBorderWidth() const { return borderWidth()*borderFactor(); }
			Unit borderFactor() const { return pimpl.borderFactor; }
			void setBorderFactor(Unit newFactor) { pimpl.borderFactor = newFactor; }

			virtual void setBackgroundColor(Color c) { pimpl.background = c; }
			virtual void setBorderColor(Color c) { pimpl.border = c; }
			virtual void setAllBackgroundColors(Color c) { setBackgroundColor(c); }
			virtual void setAllBorderColors(Color c) { setBorderColor(c); }
			void setBorderWidth(Unit newWidth) { pimpl.borderWidth = newWidth; }
		};

		// This is the more extended one.
		class ExtFrame : public Frame {
			struct Impl {
				Color disabledBackgroundColor, highlightedBackgroundColor, disabledHighlightBackgroundColor;
				Color disabledBorderColor, highlightedBorderColor, disabledHighlightBorderColor;
			} pimpl;
		public:
			/**
			ExtFrame(): Frame() {
				pimpl.disabledBackgroundColor = pimpl.highlightedBackgroundColor = pimpl.disabledBorderColor = pimpl.highlightedBorderColor = Colors::none;
			}
			**/
			ExtFrame(Unit x, Unit y, Unit z, Unit width, Unit height, Color backgroundColor = Colors::none, Color borderColor = Colors::none, Unit borderWidth = 0, Color highlightedBackgroundColor = Colors::none, Color highlightedBorderColor = Colors::none, Color disabledBackgroundColor = Colors::none, Color disabledBorderColor = Colors::none, Color disabledHighlightBackgroundColor = Colors::none, Color disabledHighlightBorderColor = Colors::none):
				Frame(x, y, z, width, height, backgroundColor, borderColor, borderWidth)
			{
				if (disabledBackgroundColor != Colors::none)
					pimpl.disabledBackgroundColor = disabledBackgroundColor;
				else
					pimpl.disabledBackgroundColor = defaultBackgroundColor();
				if (disabledBorderColor != Colors::none)
					pimpl.disabledBorderColor = disabledBorderColor;
				else
					pimpl.disabledBorderColor = defaultBorderColor();
				if (highlightedBackgroundColor != Colors::none)
					pimpl.highlightedBackgroundColor = highlightedBackgroundColor;
				else
					pimpl.highlightedBackgroundColor = defaultBackgroundColor();
				if (highlightedBorderColor != Colors::none)
					pimpl.highlightedBorderColor = highlightedBorderColor;
				else
					pimpl.highlightedBorderColor = defaultBorderColor();
				if (disabledHighlightBackgroundColor != Colors::none)
					pimpl.disabledHighlightBackgroundColor = disabledHighlightBackgroundColor;
				else
					pimpl.disabledHighlightBackgroundColor = this->disabledBackgroundColor();
				if (disabledHighlightBorderColor != Colors::none)
					pimpl.disabledHighlightBorderColor = disabledHighlightBackgroundColor;
				else
					pimpl.disabledHighlightBorderColor = this->disabledBorderColor();
			}

			Color disabledBackgroundColor() const { return pimpl.disabledBackgroundColor; }
			Color disabledBorderColor() const { return pimpl.disabledBackgroundColor; }
			Color highlightedBackgroundColor() const { return pimpl.highlightedBackgroundColor; }
			Color highlightedBorderColor() const { return pimpl.highlightedBorderColor; }
			Color disabledHighlightBackgroundColor() const { return pimpl.disabledHighlightBackgroundColor; }
			Color disabledHighlightBorderColor() const { return pimpl.disabledHighlightBorderColor; }

			/**
			// Sets ALL backgroundColors to this color UNLESS THEY ARE ALREDY SPECIFIED
			virtual void setBackgroundColor(Color newColor) {
				Frame::setBackgroundColor(newColor);
				// If they're not set yet, set them.
				if (disabledBackgroundColor() == Colors::none)
					setDisabledBackgroundColor(newColor);
				if (highlightedBackgroundColor() == Colors::none)
					setHighlightedBackgroundColor(newColor);
				if (disabledHighlightBackgroundColor() == Colors::none)
					setDisabledHighlightBackgroundColor(newColor);
			}

			virtual void setBorderColor(Color newColor) {
				Frame::setBorderColor(newColor);
				// If they're not set yet, set them.
				if (disabledBorderColor() == Colors::none)
					setDisabledBorderColor(newColor);
				if (highlightedBorderColor() == Colors::none)
					setHighlightedBorderColor(newColor);
				if (disabledHighlightBorderColor() == Colors::none)
					setDisabledHighlightBorderColor(newColor);
			}
			**/

			void setDisabledBackgroundColor(Color newColor) { pimpl.disabledBackgroundColor = newColor; }
			void setDisabledBorderColor(Color newColor) { pimpl.disabledBorderColor = newColor; }
			void setHighlightedBackgroundColor(Color newColor) { pimpl.highlightedBackgroundColor = newColor; }
			void setHighlightedBorderColor(Color newColor) { pimpl.highlightedBorderColor = newColor; }
			void setDisabledHighlightBackgroundColor(Color newColor) { pimpl.disabledHighlightBackgroundColor = newColor; }
			void setDisabledHighlightBorderColor(Color newColor) { pimpl.disabledHighlightBorderColor = newColor; }
			virtual void setAllBackgroundColors(Color newColor) {
				Frame::setAllBackgroundColors(newColor);
				setDisabledBackgroundColor(newColor);
				setHighlightedBackgroundColor(newColor);
				setDisabledHighlightBackgroundColor(newColor);
			}
			virtual void setAllBorderColors(Color newColor) {
				Frame::setAllBorderColors(newColor);
				setDisabledBackgroundColor(newColor);
				setHighlightedBackgroundColor(newColor);
				setDisabledHighlightBackgroundColor(newColor);
			}

			Color backgroundColor() const {
				if (enabled()) {
					if (highlighted())
						return highlightedBackgroundColor();
					else
						return defaultBackgroundColor();
				}
				else {
					if (highlighted())
						return disabledHighlightBackgroundColor();
					else
						return disabledBackgroundColor();
				}
			}

			Color borderColor() const {
				if (enabled()) {
					if (highlighted())
						return highlightedBorderColor();
					else
						return defaultBorderColor();
				}
				else {
					if (highlighted())
						return disabledHighlightBorderColor();
					else
						return disabledBorderColor();
				}
			}
		};
	}
}
#endif