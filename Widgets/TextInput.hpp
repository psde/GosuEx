#ifndef GOSUEX_WIDGETS_TEXTINPUT_HPP
#define GOSUEX_WIDGETS_TEXTINPUT_HPP

/**
 * WHAT DOES THE SCANNER SAY ABOUT ITS POWERLEVEL
	* ITS OVER NINETHOUSAAAAAAAAAAAAAAAAAAAAAAAND
	*/

#include <GosuEx/Widgets/Fwd.hpp>
#include <GosuEx/Widgets/Widget.hpp>
#include <GosuEx/Widgets/FontText.hpp>
#include <Gosu/Input.hpp>
#include <Gosu/TextInput.hpp>

namespace GosuEx {
		namespace Widgets {
				
				class TextInput : public FontText {
						struct Impl;
						boost::scoped_ptr<Impl> pimpl;
				public:
						// CTOOR! CHTOOOOR! DIE SCHWEIZ IST WELTMEISTER!
						TextInput(double x, double y, double z, GosuFontP font, const std::wstring& text = L"");
						virtual ~TextInput();
						
						// foo
						virtual void draw();
						virtual void update();
						virtual void buttonDown(Gosu::Button btn);

						void enableInput();
						void disableInput();
						
						virtual void cleanUp();

						// "get"Foo
						std::wstring text() const;
						bool isInput() const;
						Gosu::Color selectionColor() const;
						Gosu::Color caretColor() const;
						Gosu::TextInput* textInput() const;
						// We use this kind of text input in the back
						// It's the same. Mostly. I think.
						AdvancedGosuTextInput* advancedTextInput() const;

						// "set"Foo
						void setSelectionColor(Gosu::Color color);
						void setCaretColor(Gosu::Color color);
						virtual void setText(const std::wstring& newText);

						/// Callbackz
						// Is fired when the users presses the return key
						// ("new line key") and this widget is an active input widget.
						// Note, after firing the event (or not, it doesn't matter)
						// Whatever, always when the user presses return, the widget
						// will loose its input. If you do not want to have this event
						// fired and somehow a "always active input class", just link 
						// onReturn to enableInput.
						// This event is not fired when using disableInput.
						// It won't be fired if it looses gosuInput().
						boost::function<void(WidgetP)> onReturn;
				};
		}
}
#endif