#ifndef GOSUEX_FRAMES_FRAME_HPP
#define GOSUEX_FRAMES_FRAME_HPP

/**
 * Basic Frame.
*/
#include <GosuEx/Frames/Fwd.hpp>
#include <GosuEx/Frames/Widget.hpp>

namespace GosuEx {
	namespace Frames {
		
		// Easy Frame. Is Easy.
		class Frame : public Widget {
			struct Impl;
			boost::scoped_ptr<Impl> pimpl;
		public:
			// Empty constructor for cursors.
			Frame();

			Frame(Unit x, Unit y, Unit z, Unit width, Unit height, Color backgroundColor = Colors::none, Color borderColor = Colors::none, Unit borderWidth = 0);

			virtual ~Frame();

			//virtual Unit dispX() const;
			//virtual Unit dispY() const;
			//virtual Unit dispWidth() const;
			//virtual Unit dispHeight() const;

			virtual void draw();

			virtual Color backgroundColor() const;
			virtual Color borderColor() const;
			Color defaultBackgroundColor() const;
			Color defaultBorderColor() const;

			Unit borderWidth() const;
			Unit dispBorderWidth() const;
			Unit borderFactor() const;
			void setBorderFactor(Unit newFactor);

			virtual void setBackgroundColor(Color c);
			virtual void setBorderColor(Color c);
			virtual void setAllBackgroundColors(Color c);
			virtual void setAllBorderColors(Color c);
			void setBorderWidth(Unit newWidth);
		};

		// This is the more extended one.
		class ExtFrame : public Frame {
			struct Impl;
			boost::scoped_ptr<Impl> pimpl;
		public:
			ExtFrame(Unit x, Unit y, Unit z, Unit width, Unit height, Color backgroundColor = Colors::none, Color borderColor = Colors::none, Unit borderWidth = 0, Color highlightedBackgroundColor = Colors::none, Color highlightedBorderColor = Colors::none, Color disabledBackgroundColor = Colors::none, Color disabledBorderColor = Colors::none, Color disabledHighlightBackgroundColor = Colors::none, Color disabledHighlightBorderColor = Colors::none);

			virtual ~ExtFrame();

			Color disabledBackgroundColor() const;
			Color disabledBorderColor() const;
			Color highlightedBackgroundColor() const;
			Color highlightedBorderColor() const;
			Color disabledHighlightBackgroundColor() const;
			Color disabledHighlightBorderColor() const;

			void setDisabledBackgroundColor(Color newColor);
			void setDisabledBorderColor(Color newColor);
			void setHighlightedBackgroundColor(Color newColor);
			void setHighlightedBorderColor(Color newColor);
			void setDisabledHighlightBackgroundColor(Color newColor);
			void setDisabledHighlightBorderColor(Color newColor);
			virtual void setAllBackgroundColors(Color newColor);
			virtual void setAllBorderColors(Color newColor);

			Color backgroundColor() const;

			Color borderColor() const;
		};
	}
}
#endif