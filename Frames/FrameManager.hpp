#ifndef GOSUEX_FRAMES_FRAMEMANAGER_HPP
#define GOSUEX_FRAMES_FRAMEMANAGER_HPP

#include <GosuEx/Frames/Fwd.hpp>
#include <Gosu/Fwd.hpp>
#include <Gosu/Input.hpp>
#include <vector>
#include <string>
#include <map>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
// Memory allocation and stuff.

namespace GosuEx {
	namespace Frames {

		class FrameSet {
			struct Impl;
			boost::scoped_ptr<Impl> pimpl;
		public:
			FrameSet(const std::wstring& name);
			// root should really be a valid pointer.
			FrameSet(const std::wstring& name, Widget* root);
			~FrameSet();
			
			void buttonDown(Gosu::Button btn);
			void buttonUp(Gosu::Button btn);
			void draw();
			void update();

			std::wstring name() const;
			void addNamedWidget(Widget* widget);
			void addWidget(Widget* widget);
			void deleteWidget(Widget* widget);
			void removeNamedWidget(const std::wstring& name);
			Widget& namedWidget(const std::wstring& name) const;
			Widget& root();

			void activate();
			void deactivate();

			void enableTextInput(Widget* inputter);
			void disableTextInput(Widget* inputter);
		};

		class FrameManager {
			struct Impl;
			boost::scoped_ptr<Impl> pimpl;
			static FrameManager* gManager;

			FrameManager(Gosu::Graphics* graphics, Gosu::Input* input, const std::wstring& setname);
			~FrameManager();
		public:
			// Get the singleton
			static FrameManager& singleton();
			// Get the root widget
			static Widget& root();
			// Spawn the manager. This is required before any action is performed.
			static bool spawn(Gosu::Graphics* graphics, Gosu::Input* input, const std::wstring& setname = L"default");
			// Call this in ~Window.
			static void despawn();
			// Sets
			void addSet(FrameSet* newSet);
			void deleteSet(const std::wstring& name);
			FrameSet& actualSet() const;
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
			// buttons
			void buttonUp(Gosu::Button btn);
			void buttonDown(Gosu::Button btn);
			// Get a named widget
			Widget& namedWidget(const std::wstring& name) const;
			// Get a named font
			boost::shared_ptr<Gosu::Font> namedFont(const std::wstring& name) const;
			// TextInput
			void enableTextInput(Widget* inputter);
			void disableTextInput(Widget* inputter);
		};
	}
}
#endif