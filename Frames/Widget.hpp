#ifndef GOSUEX_FRAMES_WIDGET_HPP
#define GOSUEX_FRAMES_WIDGET_HPP

/**
 * INHERIT FROM HERE.
*/

#include <GosuEx/Frames/Fwd.hpp>
#include <GosuEx/Frames/FrameManager.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <Gosu/Color.hpp>
#include <Gosu/Math.hpp>
#include <boost/foreach.hpp>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

namespace GosuEx {
	namespace Frames {

		typedef double Unit;
		typedef Gosu::Color Color;
		namespace Colors = Gosu::Colors;
		typedef std::size_t Index;

		const Unit SQ2 = sqrt(2.0);

		class Widget : public boost::noncopyable {
			struct Impl {
				// Widget's parent
				Widget* parent;
				// Widget's children
				std::vector<Widget*> children;
				// Name
				std::wstring name;
				// Coordinates
				Unit x, y, z;
				// Size
				Unit width, height;
				// Visibility, enabled
				bool visible, enabled;
				// Highlighted
				bool highlighted;
				// Relation
				double relX, relY;
				// Factors
				double factorX, factorY;
			} pimpl;
		public:
			Widget() {
				pimpl.parent = NULL;
				pimpl.name = L"";
				pimpl.x = pimpl.y = pimpl.z = 0;
				pimpl.width = pimpl.height = 0;
				pimpl.visible = pimpl.enabled = true;
				pimpl.highlighted = false;
				pimpl.relX = pimpl.relY = 0;
				pimpl.factorX = pimpl.factorY = 1;
			}

			Widget(Unit x, Unit y, Unit z, Unit width, Unit height) {
				pimpl.parent = NULL;
				pimpl.name = L"";
				pimpl.x = x;
				pimpl.y = y;
				pimpl.z = z;
				pimpl.width = width;
				pimpl.height = height;
				pimpl.visible = pimpl.enabled = true;
				pimpl.highlighted = false;
				pimpl.relX = pimpl.relY = 0;
				pimpl.factorX = pimpl.factorY = 1;
			}

			virtual ~Widget() {
				// hardcore. erst die parents löschen. dann die children.
				BOOST_FOREACH(Widget* it, pimpl.children) {
					it->pimpl.parent = NULL;
					delete it;
				}
			}
			// Coordinates
			Unit x() const { return pimpl.x; }
			virtual Unit dispX() const { return x()-relX()*dispWidth(); }
			Unit y() const { return pimpl.y; }
			virtual Unit dispY() const { return y()-relY()*dispHeight(); }
			Unit z() const { return pimpl.z; }
			virtual Unit width() const { return pimpl.width; }
			virtual Unit dispWidth() const { return width()*factorX(); }
			virtual Unit height() const { return pimpl.height; }
			virtual Unit dispHeight() const { return height()*factorY(); }

			double relX() const { return pimpl.relX; }
			double relY() const { return pimpl.relY; }
			double factorX() const { return pimpl.factorX; }
			double factorY() const { return pimpl.factorY; }

			// Returns true if enabled, false if not
			bool enabled() const { return pimpl.enabled; }
			// Returns true if visible, false if not
			bool visible() const { return pimpl.visible; }
			// Highlighted?
			bool highlighted() const { return pimpl.highlighted; }

			// Returns the parent
			Widget* parent() const { return pimpl.parent; }
			// Returns the name
			const std::wstring& name() const { return pimpl.name; }
			// Returns all children
			const std::vector<Widget*> children() const { return pimpl.children; }
			// Returns a child (by index)
			// Because names are unique, searching by name is done by FrameManager
			Widget* child(Index index) {
				if (index > children().size())
					return NULL;
				return children().at(index);
			}
			
			// adds an ALREADY EXISTING widget.
			// do NOT use this for new widgets. Use createChild instead.
			void addChild(Widget* child) {
				pimpl.children.push_back(child);
				child->pimpl.parent = this;
			}
			
			// adds an NEW widget. Do not use this if you createChild-ed
			// child already before
			void createChild(Widget* child) {
				addChild(child);
				FrameManager::singleton().addWidget(child);
			}

			// adds an NEW widget and set its name. Do not use this if you createChild-ed
			// child already before
			void createChild(Widget* child, const std::wstring& name) {
				child->pimpl.name = name;
				createChild(child);
			}

			// removes the widget. IT IS NOT DEAD YET.
			// Use this to move widgets around.
			void removeChild(Widget* child) {
				std::vector<Widget*>::iterator it = std::remove(pimpl.children.begin(), pimpl.children.end(), child);
				(**it).pimpl.parent = NULL;
				pimpl.children.erase(it);
			}
		
			// deletes and removes this widget. Use this for memory purposes.
			void deleteChild(Widget* child) {
				removeChild(child);
				FrameManager::singleton().deleteWidget(child);
			}

			// set the parent. may be buggy.
			void setParent(Widget* newParent) {
				if (parent() != NULL)
					parent()->removeChild(this);
				pimpl.parent = newParent;
				parent()->addChild(this);
			}

			// set a widget's name
			void setName(const std::wstring& newName) {
				if (!name().empty())
					FrameManager::singleton().removeNamedWidget(name());
				pimpl.name = newName;
				FrameManager::singleton().addNamedWidget(this);
			}

			// true if should be updated
			// 2do: I've done this shit before. 
			// do this in bool shouldUpdate() const {};
			// update and draw should be void.
			virtual void update() {
				if (!shouldDraw())
					return;
				BOOST_FOREACH(Widget* it, pimpl.children) {
					it->update();
				}
			}

			virtual void draw() {
				if (!shouldDraw())
					return;
				BOOST_FOREACH(Widget* it, pimpl.children) {
					it->draw();
				}
			}

			// "MOUSE IS NOW OVER WIDGET"
			// FrameManager only.
			virtual void hover() {
				pimpl.highlighted = true;
				BOOST_FOREACH(Widget* it, pimpl.children) {
					it->hover();
				}
			}

			// FrameManager only.
			virtual void blur() {
				pimpl.highlighted = false;
				BOOST_FOREACH(Widget* it, pimpl.children) {
					it->blur();
				}
			}

			// Set coordinates
			void setX(Unit newX) { pimpl.x = newX; }
			void setY(Unit newY) { pimpl.y = newY; }
			void setZ(Unit newZ) { pimpl.z = newZ; }
			
			void setWidth(Unit newWidth) { pimpl.width = newWidth; }
			void setHeight(Unit newHeight) { pimpl.height = newHeight; }

			void setRelX(double newRelX) { pimpl.relX = newRelX; }
			void setRelY(double newRelY) { pimpl.relY = newRelY; }

			void setFactorX(double newFactorX) { pimpl.factorX = newFactorX; }
			void setFactorY(double newFactorY) { pimpl.factorY = newFactorY; }

			virtual bool highlightable() const { return visible(); }
		protected:
			virtual bool shouldDraw() const {
				return visible();
			}

			virtual bool shouldUpdate() const {
				return enabled() && visible();
			}
		};
	}
}
#endif