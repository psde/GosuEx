#ifndef GOSUEX_FRAMES_FRAMEMANAGER_HPP
#define GOSUEX_FRAMES_FRAMEMANAGER_HPP

#include <GosuEx/Frames/Fwd.hpp>
#include <Gosu/Fwd.hpp>
#include <vector>
#include <string>
#include <map>
#include <boost/shared_ptr.hpp>
// Memory allocation and stuff.

namespace GosuEx {
	namespace Frames {
		class FrameManager {
			struct Impl {
				std::vector<Widget*> widgets;
				Widget* rootWidget;
				Widget* highlightedWidget;
				std::map<std::wstring, Widget*> namedWidgets;
				std::map<std::wstring, boost::shared_ptr<Gosu::Font> > namedFonts;
				Gosu::Graphics* graphics;
				Gosu::Input* input;
			} pimpl;
			static FrameManager* gManager;

			FrameManager(Gosu::Graphics* graphics, Gosu::Input* input);
		public:
			// Get the singleton
			static FrameManager& singleton();
			// Get the root widget
			static Widget& root();
			// Spawn the manager. This is required before any action is performed.
			static bool spawn(Gosu::Graphics* graphics, Gosu::Input* input);
			// Adding a widget
			void addWidget(Widget* widget);
			// Removing a widget, deleting it
			void deleteWidget(Widget* widget);
			// Removing the name of a widget
			void removeNamedWidget(const std::wstring& name);
			// Adding a named widget.
			void addNamedWidget(Widget* widget);
			// Adding a named font
			void addNamedFont(const std::wstring& name, boost::shared_ptr<Gosu::Font> font);
			// Gosu Graphics
			Gosu::Graphics& graphics() const;
			// Gosu Input
			Gosu::Input& input() const;
			// draws
			void draw();
			// updates
			void update();
			// Get a named widget
			Widget& namedWidget(const std::wstring& name) const;
			// Get a named font
			boost::shared_ptr<Gosu::Font> namedFont(const std::wstring& name) const;
		};
	}
}
#endif