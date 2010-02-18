#ifndef GOSUEX_WIDGETS_FONTTEXT_HPP
#define GOSUEX_WIDGETS_FONTTEXT_HPP

/**
 * Simple display of a Gosu::Font.
	* The text, however, is stored.
	* It supports factorXY, relXY and colors. The default color is white.
	* You have to pass a boost::shared_ptr<Gosu::Font> to it.
	* You can use the loadFont function for that.
	*
*/

#include <GosuEx/Widgets/Fwd.hpp>
#include <GosuEx/Widgets/Widget.hpp>
#include <GosuEx/Widgets/Colorable.hpp>
#include <Gosu/Font.hpp>

namespace GosuEx {
		typedef boost::shared_ptr<Gosu::Font> GosuFontP;

		inline GosuFontP loadFont(Gosu::Font* newFont) {
				GosuFontP ptr(newFont);
				return ptr;
		}

		namespace Widgets {
				class FontText : public Colorable {
						struct Impl;
						boost::scoped_ptr<Impl> pimpl;
				public:
						/// CTor, DTor, ETor
						FontText(double x, double y, double z, GosuFontP font, const std::wstring& text);
						virtual ~FontText();

						/// foo
						virtual void draw();

						/// "get"Foo
						// Get the font
						GosuFontP font();
						virtual std::wstring text() const;

						/// setFoo
						// Set the font
						void setFont(GosuFontP newFont);
						virtual void setText(const std::wstring& newText);
				protected:
						// Set the size.
						virtual void reset();
				};
		}
}
#endif