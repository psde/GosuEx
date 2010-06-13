#ifndef GOSUEX_FRAMES_WIDGET_HPP
#define GOSUEX_FRAMES_WIDGET_HPP

/**
 * INHERIT FROM HERE.
*/

#include <GosuEx/Frames/Fwd.hpp>
#include <vector>
#include <set>
#include <string>
#include <Gosu/Color.hpp>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>
#include <Gosu/Input.hpp>

namespace GosuEx {
	namespace Frames {

		typedef double Unit;
		typedef Gosu::Color Color;
		namespace Colors = Gosu::Colors;

		class Widget : public boost::noncopyable {
			struct Impl;
			boost::scoped_ptr<Impl> pimpl;
		public:
			Widget();

			Widget(Unit x, Unit y, Unit z, Unit width, Unit height);

			virtual ~Widget();

			// Coordinates
			Unit x() const;
			virtual Unit dispX() const;
			Unit y() const;
			virtual Unit dispY() const;
			Unit z() const;
			virtual Unit width() const;
			virtual Unit dispWidth() const;
			virtual Unit height() const;
			virtual Unit dispHeight() const;

			double relX() const;
			double relY() const;
			double factorX() const;
			double factorY() const;

			// Returns true if enabled, false if not
			bool enabled() const;
			// Returns true if visible, false if not
			bool visible() const;
			// Highlighted?
			bool highlighted() const;

			// Returns the parent
			Widget* parent() const;
			// Returns the name
			const std::wstring& name() const;
			// Returns all children
			const std::set<Widget*> children() const;
			
			// adds an ALREADY EXISTING widget.
			// do NOT use this for new widgets. Use createChild instead.
			Widget& addChild(Widget* child);
			template<typename T> T& addChild(T* child) {
				addChild(dynamic_cast<Widget*>(child));
				return child;
			}

			// adds an NEW widget. Do not use this if you createChild-ed
			// child already before
			Widget& createChild(Widget* child);
			template<typename T> T& createChild(T* child) {
				createChild(dynamic_cast<Widget*>(child));
				return *child;
			}

			// adds an NEW widget and set its name. Do not use this if you createChild-ed
			// child already before
			Widget& createChild(Widget* child, const std::wstring& name);
			template<typename T> T& createChild(T* child, const std::wstring& name) {
				createChild(dynamic_cast<Widget*>(child), name);
				return *child;
			}

			// removes the widget. IT IS NOT DEAD YET.
			// Use this to move widgets around.
			void removeChild(Widget* child);
		
			// deletes and removes this widget. Use this for memory purposes.
			void deleteChild(Widget* child);

			// set the parent. may be buggy.
			void setParent(Widget* newParent);

			// set a widget's name
			void setName(const std::wstring& newName);

			virtual void update();
			virtual void draw();

			// "MOUSE IS NOW OVER WIDGET"
			// FrameManager only.
			virtual void hover();

			// FrameManager only.
			virtual void blur();

			// Set coordinates
			void setX(Unit newX);
			void setY(Unit newY);
			void setZ(Unit newZ);
			
			// Set the mass
			void setWidth(Unit newWidth);
			void setHeight(Unit newHeight);

			// Set friends
			virtual void setRelX(double newRelX);
			virtual void setRelY(double newRelY);

			// Who's invisible NOW
			void show();
			void hide();
			
			// That's totally unfair
			void enable();
			void disable();

			// Set weight
			void setFactorX(double newFactorX);
			void setFactorY(double newFactorY);

			// Set publicity
			virtual bool highlightable() const;
			void setHighlightable(bool highlightable);

			// Decide over life or death
			virtual void buttonUp(Gosu::Button btn);
			virtual void buttonDown(Gosu::Button btn);

			void setButtonUpHandler(Gosu::Button btn, boost::function<void(Widget*, Gosu::Button btn)> fct);
			void setButtonDownHandler(Gosu::Button btn, boost::function<void(Widget*, Gosu::Button btn)> fct);
			void setClickHandler(Gosu::Button btn, boost::function<void(Widget*, Gosu::Button btn)> fct);
			bool clicked(Gosu::Button btn);

			boost::function<void(Widget*)> onBlur, onHover;
		protected:
			virtual bool shouldDraw() const;

			virtual bool shouldUpdate() const;
		};

		void dumpWidgets(Widget* root, unsigned int depth = 0);
	}
}
#endif