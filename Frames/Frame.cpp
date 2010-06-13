#include "Frame.hpp"

#include <GosuEx/Frames/FrameManager.hpp>
#include <Gosu/Graphics.hpp>

using namespace GosuEx::Frames;

struct Frame::Impl {
	Color background, border;
	Unit borderWidth, borderFactor;
};

Frame::Frame():
	Widget(), pimpl(new Impl)
{
	pimpl->background = pimpl->border = Colors::none;
	pimpl->borderWidth = 0;
	pimpl->borderFactor = 1.0;
}

Frame::Frame(Unit x, Unit y, Unit z, Unit width, Unit height, Color backgroundColor, Color borderColor, Unit borderWidth):
	Widget(x, y, z, width, height), pimpl(new Impl)
{
	setBackgroundColor(backgroundColor);
	setBorderColor(borderColor);
	pimpl->borderWidth = borderWidth;
	pimpl->borderFactor = 1.0;
}

Frame::~Frame() {}

// top left point: x
//Unit Frame::dispX() const { return Widget::dispX() - dispBorderWidth(); }
// top left point: y
//Unit Frame::dispY() const { return Widget::dispY() - dispBorderWidth(); }
// + dispX: bottom right point: x
//Unit Frame::dispWidth() const { return Widget::dispWidth() + 2*dispBorderWidth(); }
// + dispY: bottom right point: y
//Unit Frame::dispHeight() const { return Widget::dispHeight() + 2*dispBorderWidth(); }

void Frame::draw() {
	if (!shouldDraw())
		return;

	Unit dX = dispX(), dY = dispY();
	Unit w = dispWidth(), h = dispHeight(), bW = dispBorderWidth();

	// Background
	FrameManager::singleton().graphics().drawQuad(
		dX, dY, backgroundColor(),
		dX+w, dY, backgroundColor(),
		dX+w, dY+h, backgroundColor(),
		dX, dY+h, backgroundColor(),
		z()
	);

	// Top
	Color bc = borderColor();
	FrameManager::singleton().graphics().drawQuad(
		dX-bW, dY-bW, bc,
		dX+w+bW, dY-bW, bc,
		dX+w+bW, dY, bc,
		dX-bW, dY, bc,
		z()
	);

	// Right
	FrameManager::singleton().graphics().drawQuad(
		dX+w, dY, bc,
		dX+w+bW, dY, bc,
		dX+w+bW, dY+h, bc,
		dX+w, dY+h, bc,
		z()
	);

	// Bottom
	FrameManager::singleton().graphics().drawQuad(
		dX-bW, dY+h, bc,
		dX+w+bW, dY+h, bc,
		dX+w+bW, dY+h+bW, bc,
		dX-bW, dY+h+bW, bc,
		z()
	);

	// Left
	FrameManager::singleton().graphics().drawQuad(
		dX-bW, dY, bc,
		dX, dY, bc,
		dX, dY+h, bc,
		dX-bW, dY+h, bc,
		z()
	);

	Widget::draw();
}

Color Frame::backgroundColor() const { return defaultBackgroundColor(); }
Color Frame::borderColor() const { return defaultBorderColor(); }
Color Frame::defaultBackgroundColor() const { return pimpl->background; }
Color Frame::defaultBorderColor() const { return pimpl->border; }

Unit Frame::borderWidth() const { return pimpl->borderWidth; }
Unit Frame::dispBorderWidth() const { return borderWidth()*borderFactor(); }
Unit Frame::borderFactor() const { return pimpl->borderFactor; }
void Frame::setBorderFactor(Unit newFactor) { pimpl->borderFactor = newFactor; }

void Frame::setBackgroundColor(Color c) { pimpl->background = c; }
void Frame::setBorderColor(Color c) { pimpl->border = c; }
void Frame::setAllBackgroundColors(Color c) { setBackgroundColor(c); }
void Frame::setAllBorderColors(Color c) { setBorderColor(c); }
void Frame::setBorderWidth(Unit newWidth) { pimpl->borderWidth = newWidth; }

struct ExtFrame::Impl {
	Color disabledBackgroundColor, highlightedBackgroundColor, disabledHighlightBackgroundColor;
	Color disabledBorderColor, highlightedBorderColor, disabledHighlightBorderColor;
};

ExtFrame::ExtFrame(Unit x, Unit y, Unit z, Unit width, Unit height, Color backgroundColor, Color borderColor, Unit borderWidth, Color highlightedBackgroundColor, Color highlightedBorderColor, Color disabledBackgroundColor, Color disabledBorderColor, Color disabledHighlightBackgroundColor, Color disabledHighlightBorderColor):
	Frame(x, y, z, width, height, backgroundColor, borderColor, borderWidth), pimpl(new Impl)
{
	if (disabledBackgroundColor != Colors::none)
		pimpl->disabledBackgroundColor = disabledBackgroundColor;
	else
		pimpl->disabledBackgroundColor = defaultBackgroundColor();

	if (disabledBorderColor != Colors::none)
		pimpl->disabledBorderColor = disabledBorderColor;
	else
		pimpl->disabledBorderColor = defaultBorderColor();
	
	if (highlightedBackgroundColor != Colors::none)
		pimpl->highlightedBackgroundColor = highlightedBackgroundColor;
	else
		pimpl->highlightedBackgroundColor = defaultBackgroundColor();
	
	if (highlightedBorderColor != Colors::none)
		pimpl->highlightedBorderColor = highlightedBorderColor;
	else
		pimpl->highlightedBorderColor = defaultBorderColor();
	
	if (disabledHighlightBackgroundColor != Colors::none)
		pimpl->disabledHighlightBackgroundColor = disabledHighlightBackgroundColor;
	else
		pimpl->disabledHighlightBackgroundColor = this->disabledBackgroundColor();
	
	if (disabledHighlightBorderColor != Colors::none)
		pimpl->disabledHighlightBorderColor = disabledHighlightBackgroundColor;
	else
		pimpl->disabledHighlightBorderColor = this->disabledBorderColor();
}

ExtFrame::~ExtFrame() {}

Color ExtFrame::disabledBackgroundColor() const { return pimpl->disabledBackgroundColor; }
Color ExtFrame::disabledBorderColor() const { return pimpl->disabledBackgroundColor; }
Color ExtFrame::highlightedBackgroundColor() const { return pimpl->highlightedBackgroundColor; }
Color ExtFrame::highlightedBorderColor() const { return pimpl->highlightedBorderColor; }
Color ExtFrame::disabledHighlightBackgroundColor() const { return pimpl->disabledHighlightBackgroundColor; }
Color ExtFrame::disabledHighlightBorderColor() const { return pimpl->disabledHighlightBorderColor; }

void ExtFrame::setDisabledBackgroundColor(Color newColor) { pimpl->disabledBackgroundColor = newColor; }
void ExtFrame::setDisabledBorderColor(Color newColor) { pimpl->disabledBorderColor = newColor; }
void ExtFrame::setHighlightedBackgroundColor(Color newColor) { pimpl->highlightedBackgroundColor = newColor; }
void ExtFrame::setHighlightedBorderColor(Color newColor) { pimpl->highlightedBorderColor = newColor; }
void ExtFrame::setDisabledHighlightBackgroundColor(Color newColor) { pimpl->disabledHighlightBackgroundColor = newColor; }
void ExtFrame::setDisabledHighlightBorderColor(Color newColor) { pimpl->disabledHighlightBorderColor = newColor; }
void ExtFrame::setAllBackgroundColors(Color newColor) {
	Frame::setAllBackgroundColors(newColor);
	setDisabledBackgroundColor(newColor);
	setHighlightedBackgroundColor(newColor);
	setDisabledHighlightBackgroundColor(newColor);
}
void ExtFrame::setAllBorderColors(Color newColor) {
	Frame::setAllBorderColors(newColor);
	setDisabledBackgroundColor(newColor);
	setHighlightedBackgroundColor(newColor);
	setDisabledHighlightBackgroundColor(newColor);
}

Color ExtFrame::backgroundColor() const {
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

Color ExtFrame::borderColor() const {
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