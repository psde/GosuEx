#include "FontText.hpp"

using namespace GosuEx;

struct Widgets::FontText::Impl {
		GosuFontP font;
		std::wstring text;
};

Widgets::FontText::FontText(double x, double y, double z, GosuFontP font, const std::wstring& text):
Colorable(x, y, z, font->textWidth(text), font->height()),
pimpl(new Impl) {
		setFont(font);
		setText(text);
		setColor(Gosu::Colors::white);
}

Widgets::FontText::~FontText() {}

void Widgets::FontText::draw() {
		// We call Widget because Colorable would
		// "probably" draw a rect
		if (!Widget::shouldHandleDraw()) return;
		Widget::draw();
		// Draw it.
		font()->draw(text(), x()-relX()*factorX()*width(), y()-relY()*factorY()*height(), z(), factorX(), factorY(), color(), alphaMode());
}

GosuFontP Widgets::FontText::font() { return pimpl->font; }
std::wstring Widgets::FontText::text() const { return pimpl->text; }

void Widgets::FontText::setFont(GosuFontP newFont) { pimpl->font = newFont; reset(); }
void Widgets::FontText::setText(const std::wstring& newText) { pimpl->text = newText; reset(); }

void Widgets::FontText::reset() {
		setSize(font()->textWidth(text()), font()->height());
}