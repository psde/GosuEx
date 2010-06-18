#ifndef GOSUEX_FRAMES_CURSOR_HPP
#define GOSUEX_FRAMES_CURSOR_HPP

#include <GosuEx/Frames/Fwd.hpp>
#include <GosuEx/Frames/Widget.hpp>
#include <GosuEx/Frames/Frame.hpp>
#include <GosuEx/Frames/FrameManager.hpp>
#include <GosuEx/Frames/Image.hpp>
#include <Gosu/Graphics.hpp>

namespace GosuEx {
	namespace Frames {

		// This class is all what it is about. It's managing X/Y things.
		template<typename T> class BasicCursor : public T {
		public:
			BasicCursor(Unit z): T() {
				setZ(z);
			}

			BasicCursor(Unit z, boost::shared_ptr<Gosu::Image> image): T(0, 0, z, image) {}

			virtual void update() {
				if (!visible())
					return;
				setX(FrameManager::singleton().input().mouseX());
				setY(FrameManager::singleton().input().mouseY());
				T::update();
			}

			virtual bool highlightable() const { return false; }
		};

		// And some examples
		class SquareCursor : public BasicCursor<Frame> {
		public:
			SquareCursor(Unit z, Unit width, Unit height, Color cursor, Color border = Colors::none, Unit borderWidth = 0):
			BasicCursor<Frame>(z)
			{
				setWidth(width);
				setHeight(height);
				setBackgroundColor(cursor);
				setBorderColor(border);
				setBorderWidth(borderWidth);
			}
		};

		class TriangleCursor : public BasicCursor<Frame> {
		public:
			TriangleCursor(Unit z, Unit width, Unit height, Color cursor, Color border = Colors::none, Unit borderWidth = 0):
			BasicCursor<Frame>(z)
			{
				setWidth(width);
				setHeight(height);
				setBackgroundColor(cursor);
				setBorderColor(border);
				setBorderWidth(borderWidth);
			}

			virtual void draw() {
				if (!shouldDraw())
					return;
				// Background
				FrameManager::singleton().graphics().drawTriangle(
					x(), y(), backgroundColor(), 
					x()+width(), y(), backgroundColor(),
					x(), y()+height(), backgroundColor(),
					z());
				// Lines
				// Top
				FrameManager::singleton().graphics().drawQuad(
					x()-borderWidth(), y()-borderWidth(), borderColor(),
					x()+width()+borderWidth(), y()-borderWidth(), borderColor(),
					x()+width(), y(), borderColor(),
					x()-borderWidth(), y(), borderColor(),
					z());
				// left
				FrameManager::singleton().graphics().drawQuad(
					x()-borderWidth(), y()-borderWidth(), borderColor(),
					x(), y()-borderWidth(), borderColor(),
					x(), y()+height()+borderWidth(), borderColor(),
					x()-borderWidth(), y()+height()+borderWidth(), borderColor(),
					z());
			}
		};

		typedef BasicCursor<Image> ImageCursor;
	}
}
#endif