#ifndef GOSUEX_WIDGETS_FWD_HPP
#define GOSUEX_WIDGETS_FWD_HPP

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace GosuEx {
		namespace Widgets {

				// 'Widget' itself
				class Widget;
				// Pointer
				typedef boost::shared_ptr<Widget> WidgetP;
				// Weaky
				typedef boost::weak_ptr<Widget> WidgetWP;

				// 'Colorable', a colored rect
				class Colorable;
				// Pointer
				typedef boost::shared_ptr<Colorable> ColorableP;
				// Weaky
				typedef boost::weak_ptr<Widget> ColorableWP;

				// Cursor
				class Cursor;
				// Pointer
				typedef boost::shared_ptr<Cursor> CursorP;

				// 'FontText', text based on a Gosu::Font
				class FontText;
				// Pointer
				typedef boost::shared_ptr<FontText> FontTextP;

				// 'TextInput', FontText based text input.
				class TextInput;
				// Pointer
				typedef boost::shared_ptr<TextInput> TextInputP;

				// Image - for ... pictures.
				class Image;
				// Pointer
				typedef boost::shared_ptr<Image> ImageP;

				// Buttonz
				class Button;
				// pointer
				typedef boost::shared_ptr<Button> ButtonP;

				// Buttonz with Textz
				class FontTextButton;
				// pointer
				typedef boost::shared_ptr<FontTextButton> FontTextButtonP;

				/// EFFECTS
				namespace Effects {
						// Effect
						class Effect;
						// Pointer
						typedef boost::shared_ptr<Effect> EffectP;
						// Weak Ptr
						typedef boost::weak_ptr<Effect> EffectWP;

						// Fading color
						class FadeColor;
						// Fading color pesos
						typedef boost::shared_ptr<FadeColor> FadeColorP;
				}

				// Overridden TextInput.
				// go, horse, goo!
				class AdvancedGosuTextInput;
		}
}
#endif