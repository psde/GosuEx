#include "TextInput.hpp"
#include "AdvancedGosuTextInput.hpp"
#include <Gosu/Timing.hpp>

using namespace GosuEx;
// May we need this if kbReturn does quit the input mode
struct Widgets::TextInput::Impl {
		Gosu::Color selectionColor;
		Gosu::Color caretColor;
		AdvancedGosuTextInput textInput;
};

Widgets::TextInput::TextInput(double x, double y, double z, GosuFontP font, const std::wstring &text):
pimpl(new Impl), FontText(x, y, z, font, text) {
		pimpl->selectionColor = Gosu::Colors::none;
		pimpl->caretColor = Gosu::Colors::none;
		setText(text);
//		pimpl->textInput = new Gosu::TextInput();
}

Widgets::TextInput::~TextInput() {}

void Widgets::TextInput::draw() {
		// "smart check", only widget. We need to draw the font itself later.
		if (!Widget::shouldHandleDraw()) return;
		if (!gosuGraphics()) return;
		double posX, selX;
		{
				std::wstring posText = text().substr(0, textInput()->caretPos());
				posX = font()->textWidth(posText)*factorX();
		}
		//posX = font()->textWidth(text().substr(0, textInput()->caretPos()))*factorX();
		{
				std::wstring selText = text().substr(0, textInput()->selectionStart());
				selX = font()->textWidth(selText)*factorX();
		}
		//selX = font()->textWidth(text().substr(0, textInput()->selectionStart()))*factorX();
		gosuGraphics()->drawQuad(x()+selX, y(), selectionColor(), x()+posX, y(), selectionColor(), x()+selX, y()+height(), selectionColor(), x()+posX, y()+height(), selectionColor(), z());
		if (isInput() && Gosu::milliseconds() % 2000 <= 1000)
				gosuGraphics()->drawLine(x()+posX, y(), caretColor(), x()+posX, y()+height(), caretColor(), z());
		// Draw the font
		FontText::draw();
}

void Widgets::TextInput::update() {
		if (!FontText::shouldHandleUpdate()) return;
		FontText::update();
		// The text could have changed.
		if (isInput()) reset();
}

void Widgets::TextInput::buttonDown(Gosu::Button btn) {
		if (!Widget::shouldHandleButtonDown()) return;
		Widget::buttonDown(btn);
		if (btn == Gosu::kbReturn) {
				if (isInput()) {
						disableInput();
						// allow the callback to enable input again
						if (onReturn)
								onReturn(ptr());
				}
		}
}

void Widgets::TextInput::enableInput() {
		if (!gosuInput()) return;
		gosuInput()->setTextInput(textInput());
}

void Widgets::TextInput::disableInput() {
		if (!gosuInput() || !isInput()) return;
		gosuInput()->setTextInput(NULL);
}

void Widgets::TextInput::cleanUp() {
		Widget::cleanUp();
		onReturn.clear();
}

bool Widgets::TextInput::isInput() const {
		if (!gosuInput()) return false;
		return gosuInput()->textInput() == textInput();
}

Gosu::Color Widgets::TextInput::selectionColor() const { return pimpl->selectionColor; }
Gosu::Color Widgets::TextInput::caretColor() const { return pimpl->caretColor; }
Gosu::TextInput* Widgets::TextInput::textInput() const { return dynamic_cast<Gosu::TextInput*>(&pimpl->textInput); }
Widgets::AdvancedGosuTextInput* Widgets::TextInput::advancedTextInput() const { return &pimpl->textInput; }

std::wstring Widgets::TextInput::text() const { return textInput()->text(); }

void Widgets::TextInput::setSelectionColor(Gosu::Color color) { pimpl->selectionColor = color; }
void Widgets::TextInput::setCaretColor(Gosu::Color color) { pimpl->caretColor = color; }
void Widgets::TextInput::setText(const std::wstring& newText) { textInput()->setText(newText); reset(); }